#include <elf/elf32_loader.h>
#include <stdio.h>

static elf32_section_header *elf32_get_section_header(elf32_header *header, uint32_t section_index) {
    elf32_section_header *sections = (elf32_section_header *)((uint32_t)header + header->shoff);
    return &(sections[section_index]);
}

static const char *elf32_get_section_name(elf32_header *header, uint32_t offset) {
    if (header->shstrndx == ELF32_SECTION_UNDEFINED) {
        return 0;
    }
    const char *section_string_table = (char *)header + elf32_get_section_header(header, header->shstrndx)->sh_offset;
    return section_string_table + offset;
}

void elf32_load_exec(void *file) {
    elf32_header *header = file;

    //check magic number
    if (!elf32_is_valid(header)) {
        return;
    }
    //check if elf is supported
    if (!elf32_is_suported(header)) {
        return;
    }

    elf32_section_header *section;

    for (int i = 0; i < header->shnum; i++) {
        section = elf32_get_section_header(header, i);
        printf("s%d: %s\n", i, elf32_get_section_name(header, section->sh_name));
    }
}

bool elf32_is_valid(elf32_header *header) {
    return header->ident[ELF32_IDENT_MAG0_INDEX] == ELF32_IDENT_MAG0 &&
           header->ident[ELF32_IDENT_MAG1_INDEX] == ELF32_IDENT_MAG1 &&
           header->ident[ELF32_IDENT_MAG2_INDEX] == ELF32_IDENT_MAG2 &&
           header->ident[ELF32_IDENT_MAG3_INDEX] == ELF32_IDENT_MAG3;
}

bool elf32_is_suported(elf32_header *header) {
    if (header->ident[ELF32_IDENT_CLASS_INDEX] != ELF32_IDENT_CLASS_32BIT) {
        return false;
    }
    if (header->ident[ELF32_IDENT_DATA_INDEX] != ELF32_IDENT_DATA_LE) {
        return false;
    }
    if (header->machine != ELF32_MACHINE_386) {
        return false;
    }
    if (header->type != ETF32_TYPE_EXEC) {
        return false;
    }
    return true;
}