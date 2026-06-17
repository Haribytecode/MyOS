#include "paging.h"

extern void kprint(const char* str);
#define MAX_FRAMES 1024
uint8_t frame_bitmap[MAX_FRAMES];

uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t first_page_table[1024] __attribute__((aligned(4096)));
uint32_t second_page_table[1024] __attribute__((aligned(4096)));
uint32_t kernel_page_table[1024] __attribute__((aligned(4096)));
void paging_init() {

    // ❌ DO NOT use 0x2
    for (int i = 0; i < 1024; i++) {
        page_directory[i] = 0x0;  // correct
    }

    // Identity map first 4MB
    for (int i = 0; i < 1024; i++) {
        first_page_table[i] = (i * 0x1000) | 3;
    }
    for(int i=0;i<1024;i++){
        second_page_table[i]=(i * 0x1000 + 0x400000) | 3; //Allocating 4kb chunk of memory for each entry of table 2 but from 4mb, because until 4mb it is for Table 1
    }

    page_directory[0] = ((uint32_t)first_page_table) | 3;
    page_directory[1] = ((uint32_t)second_page_table) | 3;
    page_directory[768] = ((uint32_t)first_page_table) | 3;
    page_directory[769] = ((uint32_t)second_page_table) | 3;

    for(int i=0;i<MAX_FRAMES;i++){
        frame_bitmap[i]=0;

    }
    page_directory[1023]=((uint32_t)page_directory) | 3;
   
}
void paging_enable() {
    uint32_t pd = (uint32_t)page_directory;

    asm volatile(
        "mov %0, %%cr3\n"        // load page directory
        "mov %%cr0, %%eax\n"
        "or $0x80000000, %%eax\n" // set PG bit
        "mov %%eax, %%cr0\n"
        "jmp 1f\n"               // flush pipeline
        "1:\n"
        :
        : "r"(pd)
        : "eax"
    );

    // DEBUG CHECK
    uint32_t cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));

    if (cr0 & 0x80000000)
        kprint("PAGING ON\n");
    else
        kprint("PAGING OFF\n");
}
    uint32_t alloc_frame(){
            for(int i=0;i<MAX_FRAMES;i++){
                if(frame_bitmap[i]==0){
                    frame_bitmap[i]=1;
                    return 0x800000 +(i*0x1000);
                }
            }
            return 0;
        }
        void free_frame(uint32_t phys){
            uint32_t index=(phys-0x800000)/0x1000;
            if(index<MAX_FRAMES){      // safety check to ensure the frame number actually fits inside your bitmap array
                frame_bitmap[index]=0;
            }
     
        }
   
void map_page(uint32_t virt,uint32_t phys){
    uint32_t directory_index=(virt>>22) & 0x3FF;
    uint32_t table_index=(virt>>12) & 0x3FF;
    uint32_t *target_table_virt=(uint32_t *)(0XFFC00000+(directory_index*0x1000));
        
    if(page_directory[directory_index]==0){     
                  // Here that means the page directory's entry does not have its page table created yet, so create new page table!
        uint32_t phys_table=alloc_frame();
        if(phys_table==0) {
            kprint("KERNEL PANIC:OUT OF MEMORY!\n");
            while(1);

        }
       

        page_directory[directory_index]=phys_table | 3;
        asm volatile("mov %%cr3, %%eax\n mov %%eax, %%cr3\n" ::: "eax");
        for(int i=0;i<1024;i++){
            target_table_virt[i]=0;
        }



    } 
    target_table_virt[table_index]=phys | 3;
      
        

    asm volatile(
        "mov %%cr3, %%eax\n"
        "mov %%eax, %%cr3\n "
        :
        :
        :"eax"
    );
    
}
