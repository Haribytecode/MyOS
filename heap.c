#include "heap.h"
#include "paging.h"
#define KHEAP_START 0xD0000000
#define PAGE_SIZE 4096
static uint32_t heap_next=KHEAP_START;
static heap_block_t *heap_head=0;
extern void kprint(const char* str);
void heap_init(void){
    heap_next=KHEAP_START; ///this heap_init is for telling the bootloader i.e during boot the heap_next is initiated always with starting address of the kernel heap address kheap!!
    heap_head=0;
}
void *kmalloc_page(void){  //* is for memory access pointer allowing the kernel to read and write without it it only gets a plain number
    uint32_t phys=alloc_frame();
    if(!phys)
    return 0;
    else 
    map_page(heap_next,phys); // Mapping kernel heap address (virtual) with the physical address using page table logic (similarly) //
    void *result=(void *)heap_next; //getting the address into generic pointer variable >result
    heap_next+=0x1000; //Allows the heap_next to receive only memory address that is only 4kb to prevent memory leaks and maintain isolation of memory
    return result; //finally the kernel getting its requested or needed block of memory space as a address only here i.e kernel heap virtual address

} 
  void *kmalloc(uint32_t size){
    if(heap_head==0){
        void *page=kmalloc_page();
        if(!page){
            return 0;
        }
    
        heap_head=(heap_block_t *)page;   //Actually here heap_head always at 0xD0000000 now here for example only//
        heap_head->size=PAGE_SIZE-sizeof(heap_block_t); //here 4096-size of heap payload block is here 4084 so heap_head size is 12 bytes also dont confuse page size with heap_block because page is 4096 but heap_bloc_t is 12 bytes actually which could be done in heap.h //
        heap_head->is_free=1; //which means this current page block i.e the payload is free to use by the kernel //
        heap_head->next=0; //becuase there is no block after this current block actually now so next block's address is empty(0) which means no block and 0 means memory pointer address is 0//
    }

    heap_block_t *current=heap_head; //current is temp variable to avoid modification of heap_head while the loop goes on
    while(current!=0){ //here it means "No existing block can satisfy request." if current =0 actually if not 0 then heap block highway exists
        if(current->is_free==1 && current->size>=size){  //As this size is the one that is requested by the kernel actually 
            
            /* 📐 DIRECT SAFETY BOUNDARY CHECK: Does the left-over space physically fit a new header? */
             if (current->size > (size + sizeof(heap_block_t)))  //Now my ego touch this production allocator because of you 
            {  //current->size must be strictly greater than size + sizeof(heap_block_t) before you even attempt to cut a new block//

                
                // 🪓 THE KNIFE CUT: Directly carve the new remainder block out of thin air
                heap_block_t *new_block =(heap_block_t *)((uint32_t)current + sizeof(heap_block_t) + size);          
                
                // Slicing the math parameters exactly matching your flow layout
                new_block->size = current->size - size - sizeof(heap_block_t);
                new_block->is_free = 1;
                new_block->next = current->next;
                
                // Re-routing current's parameters before hitting the return pointer
                current->size = size;
                current->next = new_block;
            }
            /* 🛑 ELSE: Leftover sliver cannot fit a header! Skip split completely to prevent corruption */

            current->is_free=0; //Here it is changed to 0 to tell the kmalloc that it is already occupied
            
            void *payload=(void *)((uint32_t)current+sizeof(heap_block_t));  //here current refers to 0xD0000000 and heap-block_t refers to 12 bytes so payload gets his required address to give it to kernel
            return payload;
        }
        current=current->next; //Moving the pointer to next block 
    }
    kprint("HEAP HIGHWAY EXHAUSTED\n"); 
    return 0;
}
void kfree(void *ptr){
    if(ptr==0){
        return;
    }
    heap_block_t *block=(heap_block_t *)((uint32_t)ptr-sizeof(heap_block_t));   
    block->is_free=1;
    //coalescing//
    if(block->next!=0 && block->next->is_free==1){
        block->size+=block->next->size+sizeof(heap_block_t);
        block->next=block->next->next; //here block 1's next pointer points to block 3 
    }
    heap_block_t *prev=heap_head;
    while(prev!=0){
        if(prev->next==block){
            if(prev->is_free==1){
                prev->size+=block->size+sizeof(heap_block_t);
                prev->next=block->next;
            }
            break;
        }
        prev=prev->next;
    }


}
