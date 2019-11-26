#include <elf/elf32_loader.h>
#include <stdio.h>
#include <string.h>

static elf32_program_header *elf32_get_program_header(elf32_header *header, uint32_t ph_index) {
    if (ph_index >= header->phnum) {
        return 0;
    }
    elf32_program_header *program_headers = (elf32_program_header *)((uint32_t)header + header->phoff);
    return &(program_headers[ph_index]);
}

static elf32_section_header *elf32_get_section_header(elf32_header *header, uint32_t section_index) {
    if (section_index >= header->shnum) {
        return 0;
    }
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

static void print_debug_info(elf32_header *header) {
    printf("ELF DEBUG INFO ------------------------------\n\n");
    //check magic number
    printf("Valid: %s\n", elf32_is_valid(header) ? "true" : "false");
    //check if elf is supported
    printf("Supported: %s\n", elf32_is_suported(header) ? "true" : "false");
    printf("Entry vaddr: %d\n", header->entry);

    elf32_section_header *section;
    elf32_program_header *program_header;

    printf("\nSections:\n");

    for (int i = 0; i < header->shnum; i++) {
        section = elf32_get_section_header(header, i);
        printf(" - sh%d: %s\n", i, elf32_get_section_name(header, section->sh_name));
    }

    printf("\nProgram headers:\n");

    for (int i = 0; i < header->phnum; i++) {
        program_header = elf32_get_program_header(header, i);
        printf(" - ph%d: type %d, fileSize %d, memSize %d, offset %d, vaddr %d\n",
               i, program_header->p_type,
               program_header->p_filesz,
               program_header->p_memsz,
               program_header->p_offset,
               program_header->p_vaddr);
    }

    printf("\n------------------------------------------------\n");
}

kernel_return_t elf32_load_exec(void *file, uint32_t physical_address, elf32_info *program_info) {
    elf32_header *header = file;
    uint8_t *target_process_space = (uint8_t *)physical_address;

    //print_debug_info(header);

    //check magic number
    if (!elf32_is_valid(header)) {
        return ELF32_STATUS_INVALID;
    }
    //check if elf is supported
    if (!elf32_is_suported(header)) {
        return ELF32_STATUS_UNSUPPORTED;
    }

    //load program segments
    elf32_program_header *program_header;

    uint32_t program_base_vaddr = elf32_get_program_header(header, 0)->p_vaddr;
    uint32_t program_prev_vaddr = program_base_vaddr;

    for (int i = 0; i < header->phnum; i++) {
        program_header = elf32_get_program_header(header, i);
        if (program_header->p_type == PT_LOAD) {
            target_process_space += program_header->p_vaddr - program_prev_vaddr;
            program_prev_vaddr = program_header->p_vaddr;
            memcpy(target_process_space, (void *)((uint32_t)header + program_header->p_offset), program_header->p_filesz);
            if (program_header->p_filesz < program_header->p_memsz) {
                memset(target_process_space + program_header->p_filesz, 0, program_header->p_memsz - program_header->p_filesz);
            }
        }
    }

    program_info->base_vaddr = program_base_vaddr;
    program_info->entry_vaddr = header->entry;

    return KERNEL_STATUS_SUCCESS;
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