#include "paging.h"
#include "console.h"
#include <stdint.h>
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
   
void map_page(uint32_t virt, uint32_t phys) {
    uint32_t directory_index = (virt >> 22) & 0x3FF;
    uint32_t table_index = (virt >> 12) & 0x3FF;
    
    // Determine target flags based on address ownership
    // If the virtual address is below 3GB (0xC0000000), apply the User flag (0x04)
    uint32_t flags = 3; // Present (1) | Writable (2)
    if (virt < 0xC0000000) {
        flags |= 4; // Add User/Supervisor flag (4)
    }

    if (page_directory[directory_index] == 0) {     
        uint32_t phys_table = alloc_frame();
        if (phys_table == 0) {
            kprint("KERNEL PANIC: OUT OF MEMORY!\n");
            while(1);
        }
        // Securely apply our dynamically selected flag context to the directory entry
        page_directory[directory_index] = phys_table | flags;
    }

    // Capture the target page table via the virtual mapping window
    uint32_t *target_table_virt = (uint32_t *)(0xFFC00000 + (directory_index * 0x1000));
    
    // Set the individual page table entry with our matching flags
    target_table_virt[table_index] = phys | flags;

    // Critical: Flush the CPU's TLB cache for this specific address 
    // to force the processor to reload the updated hardware privilege flags!
    asm volatile("invlpg (%0)" : : "r"(virt) : "memory");
}


/* Master kernel page directory */
// Master kernel page directory from your system
extern uint32_t page_directory[PAGE_ENTRIES];
extern uint32_t alloc_frame(void);
void* phys_to_virt(uint32_t phys)
{
    // Add the higher-half kernel base offset to translate the raw physical frame
    return (void*)(phys + KERNEL_BASE);
}

uint32_t* create_process_address_space(void)
{
    // 1. Allocate a physical frame for the new process directory
    uint32_t pd_phys = alloc_frame();
    if (pd_phys == 0) return 0;

    // 2. Convert to a safe, writable virtual address pointer
    uint32_t *pd = (uint32_t *)phys_to_virt(pd_phys);

    // 3. Clear lower 3GB user space entries completely
    for (int i = 0; i < 768; i++) {
        pd[i] = 0;
    }

    // 4. Share upper 1GB kernel space entries identically from the master directory
    for (int i = 768; i < PAGE_ENTRIES; i++) {
        pd[i] = page_directory[i];
    }

    return pd;
}
