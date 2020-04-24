#ifndef _GDT_H
#define _GDT_H

struct gdt_entry_bits {
    unsigned int limit_low : 16;
    unsigned int base_low : 24;
    //attribute byte split into bitfields
    unsigned int accessed : 1;
    unsigned int read_write : 1;  //readable for code, writable for data
    unsigned int conforming : 1;  //conforming for code, expand down for data
    unsigned int code : 1;        //1 for code, 0 for data
    unsigned int always_1 : 1;    //should be 1 for everything but TSS and LDT
    unsigned int DPL : 2;         //priviledge level
    unsigned int present : 1;
    //and now into granularity
    unsigned int limit_high : 4;
    unsigned int available : 1;
    unsigned int always_0 : 1;  //should always be 0
    unsigned int big : 1;       //32bit opcodes for code, uint32_t stack for data
    unsigned int gran : 1;      //1 to use 4k page addressing, 0 for byte addressing
    unsigned int base_high : 8;
} __attribute__((packed));

#endif