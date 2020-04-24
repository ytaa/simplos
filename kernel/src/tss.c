#include <boot/gdt.h>
#include <boot/tss.h>
#include <kernel/kernel_utils.h>
#include <kernel/scheduler.h>
#include <string.h>

extern void tss_flush(void);

tss_entry_t __attribute__((section(".tss"))) tss_entry;

void printbits(uint8_t *bits, int nbytes) {
    for (int i = 0; i < nbytes; i++) {
        for (int j = 7; j >= 0; j--) {
            if (bits[i] & 1 << j) {
                kuts_printk("1");
            } else {
                kuts_printk("0");
            }
        }
        kuts_printk(" ");
    }
    kuts_printk("\n");
}

void testtt() {
    struct gdt_entry_bits gdt[6];
    //....insert your ring 0 segments here or whatever
    struct gdt_entry_bits *code;
    struct gdt_entry_bits *data;
    //I assume your ring 0 segments are in gdt[1] and gdt[2] (0 is null segment)
    code = (void *)&gdt[3];  //gdt is a static array of gdt_entry_bits or equivalent
    data = (void *)&gdt[4];
    code->limit_low = 0xFFFF;
    code->base_low = 0;
    code->accessed = 0;
    code->read_write = 1;  //make it readable for code segments
    code->conforming = 0;  //don't worry about this..
    code->code = 1;        //this is to signal it's a code segment
    code->always_1 = 1;
    code->DPL = 3;  //set it to ring 3
    code->present = 1;
    code->limit_high = 0xF;
    code->available = 1;
    code->always_0 = 0;
    code->big = 1;   //signal it's 32 bits
    code->gran = 1;  //use 4k page addressing
    code->base_high = 0;
    *data = *code;   //copy it all over, cause most of it is the same
    data->code = 0;  //signal it's not code; so it's data.

    printbits((uint8_t *)data, sizeof(*data));
    printbits((uint8_t *)code, sizeof(*code));

    struct gdt_entry_bits *g = &gdt[5];
    // Firstly, let's compute the base and limit of our entry into the GDT.
    uint32_t base = (uint32_t)&tss_entry;
    uint32_t limit = sizeof(tss_entry);

    // Now, add our TSS descriptor's address to the GDT.
    g->limit_low = limit & 0xFFFF;
    g->base_low = base & 0xFFFFFF;            //isolate bottom 24 bits
    g->accessed = 1;                          //This indicates it's a TSS and not a LDT. This is a changed meaning
    g->read_write = 0;                        //This indicates if the TSS is busy or not. 0 for not busy
    g->conforming = 0;                        //always 0 for TSS
    g->code = 1;                              //For TSS this is 1 for 32bit usage, or 0 for 16bit.
    g->always_1 = 0;                          //indicate it is a TSS
    g->DPL = 3;                               //same meaning
    g->present = 1;                           //same meaning
    g->limit_high = (limit & 0xF0000) >> 16;  //isolate top nibble
    g->available = 0;
    g->always_0 = 0;                           //same thing
    g->big = 0;                                //should leave zero according to manuals. No effect
    g->gran = 0;                               //so that our computed GDT limit is in bytes, not pages
    g->base_high = (base & 0xFF000000) >> 24;  //isolate top byte.

    kuts_printk("limit: %u, base: %u\n", limit, base);
    printbits((uint8_t *)g, sizeof(*g));

    while (1)
        ;
}

void tss_init() {
    //testtt();
    /*
    // Firstly, let's compute the base and limit of our entry into the GDT.
    uint32_t base = (uint32_t)&tss_entry;
    uint32_t limit = sizeof(tss_entry);

    // Now, add our TSS descriptor's address to the GDT.
    uint16_t limit_low = limit & 0xFFFF;
    uint32_t base_low = base & 0xFFFFFF;  //isolate bottom 24 bits
    uint8_t attr = 0b10010111;
    uint8_t flagstop = (limit & 0xF0000) >> 16;  //isolate top nibble
    uint8_t flagsbot = 0b0000;
    uint8_t base_high = (base & 0xFF000000) >> 24;  //isolate top byte.

    kuts_printk("%u\n", (uint32_t)&tss_entry);
    kuts_printk("%u %u %u %u %u %u\n", limit_low, base_low, attr, flagstop, flagsbot, base_high);
    */

    // Ensure the TSS is initially zero'd.
    bzero(&tss_entry, sizeof(tss_entry));

    tss_entry.ss0 = 0x0010;  // Set the kernel stack segment.
    //tss_entry.esp0 = (uint32_t) & (idle_pcb.kernel_stack) + SCH_KERNEL_STACK_SIZE;  // Set the kernel stack pointer.                                                                    //note that CS is loaded from the IDT entry and should be the regular kernel code segment
    tss_entry.iomap_base = sizeof(tss_entry);

    tss_flush();
}