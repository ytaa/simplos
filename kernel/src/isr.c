#include <kernel/isr.h>
#include <kernel/pit/pit_timer.h>
#include <kernel/ps2/keyboard.h>
#include <sys/io.h>

struct IDT_entry IDT[256];

void isr_idt_init() {
    extern int isr_idt_load();
    extern int isr_irq0();
    extern int isr_irq1();

    unsigned long irq0_address;
    unsigned long irq1_address;

    unsigned long idt_address;
    unsigned long idt_ptr[2];

    //remapping the PIC
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 40);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);

    isr_clear_mask(0);
    isr_clear_mask(1);
    isr_set_mask(2);
    isr_set_mask(3);
    isr_set_mask(4);
    isr_set_mask(5);
    isr_set_mask(6);
    isr_set_mask(7);
    isr_set_mask(8);
    isr_set_mask(9);
    isr_set_mask(10);
    isr_set_mask(11);
    isr_set_mask(12);
    isr_set_mask(13);
    isr_set_mask(14);
    isr_set_mask(15);

    irq0_address = (unsigned long)isr_irq0;
    IDT[32].offset_lowerbits = irq0_address & 0xffff;
    IDT[32].selector = 0x08;
    IDT[32].zero = 0;
    IDT[32].type_attr = 0x8e;
    IDT[32].offset_higherbits = (irq0_address & 0xffff0000) >> 16;

    irq1_address = (unsigned long)isr_irq1;
    IDT[33].offset_lowerbits = irq1_address & 0xffff;
    IDT[33].selector = 0x08;
    IDT[33].zero = 0;
    IDT[33].type_attr = 0x8e;
    IDT[33].offset_higherbits = (irq1_address & 0xffff0000) >> 16;

    /* fill the IDT descriptor */
    idt_address = (unsigned long)IDT;
    idt_ptr[0] = (sizeof(struct IDT_entry) * 256) + ((idt_address & 0xffff) << 16);
    idt_ptr[1] = idt_address >> 16;
    isr_idt_load(idt_ptr);
}

void isr_irq0_handler(void) {
    pitt_handle_irq();
    outb(0x20, 0x20);  //EOI
}

void isr_irq1_handler(void) {
    uint8_t key = inb(0x60);
    ps2k_handle_irq1(key);
    outb(0x20, 0x20);  //EOI
}

void isr_set_mask(unsigned char IRQline) {
    uint16_t port;
    uint8_t value;

    if (IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = inb(port) | (1 << IRQline);
    outb(port, value);
}

void isr_clear_mask(unsigned char IRQline) {
    uint16_t port;
    uint8_t value;

    if (IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = inb(port) & ~(1 << IRQline);
    outb(port, value);
}
