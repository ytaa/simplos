#ifndef _ELF32_LOADER_H
#define _ELF32_LOADER_H

#include <kernel/kernel_utils.h>
#include <stdbool.h>
#include <stdint.h>

#define ELF32_IDENT_NBYTES 16

//elf32 header expected values
#define ELF32_IDENT_MAG0 0x7F
#define ELF32_IDENT_MAG1 'E'
#define ELF32_IDENT_MAG2 'L'
#define ELF32_IDENT_MAG3 'F'
#define ELF32_IDENT_DATA_LE (1)      // little Endian data allign
#define ELF32_IDENT_CLASS_32BIT (1)  // 32-bit architecture
#define ELF32_MACHINE_386 (3)        // x86

#define ELF32_SECTION_UNDEFINED (0x00)

//return status values
#define ELF32_STATUS_INVALID (-2)
#define ELF32_STATUS_UNSUPPORTED (-3)

typedef uint16_t elf32_half;
typedef uint32_t elf32_offset;
typedef uint32_t elf32_addres;
typedef uint32_t elf32_word;
typedef int32_t elf32_sword;

enum elf32_ident_index {
    ELF32_IDENT_MAG0_INDEX = 0,
    ELF32_IDENT_MAG1_INDEX = 1,
    ELF32_IDENT_MAG2_INDEX = 2,
    ELF32_IDENT_MAG3_INDEX = 3,
    ELF32_IDENT_CLASS_INDEX = 4,
    ELF32_IDENT_DATA_INDEX = 5,
    ELF32_IDENT_VERSION_INDEX = 6,
    ELF32_IDENT_OSABI_INDEX = 7,
    ELF32_IDENT_ABIVERSION_INDEX = 8,
    ELF32_IDENT_PAD_INDEX = 9
};

enum elf32_type {
    ELF32_TYPE_NONE = 0,  // unknown
    ELF32_TYPE_REL = 1,   // relocatable
    ETF32_TYPE_EXEC = 2   // executable
};

enum elf32_section_type {
    ELF32_SECTION_NULL = 0,      // null section
    ELF32_SECTION_PROGBITS = 1,  // program information
    ELF32_SECTION_SYMTAB = 2,    // symbol table
    ELF32_SECTION_STRTAB = 3,    // string table
    ELF32_SECTION_RELA = 4,      // relocation (with addend)
    ELF32_SECTION_NOBITS = 8,    // not present in file
    ELF32_SECTION_REL = 9,       // relocation (no addend)
};

enum elf32_section_attribute {
    SECTION_ATTR_WRITE = 0x01,  // Writable section
    SECTION_ATTR_ALLOC = 0x02   // Exists in memory
};

enum elf32_program_section_type {
    PT_NULL = 0,
    PT_LOAD = 1,
    PT_DYNAMIC = 2,
    PT_INTERP = 3,
    PT_NOTE = 4,
    PT_SHLIB = 5,
    PT_PHDR = 6,
    PT_TLS = 7,
    PT_LOOS = 0x60000000,
    PT_SUNW_UNWIND = 0x6464e550,
    PT_SUNW_EH_FRAME = 0x6474e550,
    PT_LOSUNW = 0x6ffffffa,
    PT_SUNWBSS = PT_LOSUNW,
    PT_SUNWSTACK = 0x6ffffffb,
    PT_SUNWDTRACE = 0x6ffffffc,
    PT_SUNWCAP = 0x6ffffffd,
    PT_HISUNW = 0x6fffffff,
    PT_HIOS = PT_HISUNW,
    PT_LOPROC = 0x70000000,
    PT_HIPROC = 0x7fffffff
};

typedef struct {
    uint8_t ident[ELF32_IDENT_NBYTES];
    elf32_half type;
    elf32_half machine;
    elf32_word version;
    elf32_addres entry;
    elf32_offset phoff;
    elf32_offset shoff;
    elf32_word flags;
    elf32_half ehsize;
    elf32_half phentsize;
    elf32_half phnum;
    elf32_half shentsize;
    elf32_half shnum;
    elf32_half shstrndx;
} elf32_header;

typedef struct {
    elf32_word sh_name;
    elf32_word sh_type;
    elf32_word sh_flags;
    elf32_addres sh_addr;
    elf32_offset sh_offset;
    elf32_word sh_size;
    elf32_word sh_link;
    elf32_word sh_info;
    elf32_word sh_addralign;
    elf32_word sh_entsize;
} elf32_section_header;

typedef struct {
    elf32_word p_type;
    elf32_offset p_offset;
    elf32_addres p_vaddr;
    elf32_addres p_paddr;
    elf32_word p_filesz;
    elf32_word p_memsz;
    elf32_word p_flags;
    elf32_word p_align;
} elf32_program_header;

typedef struct {
    uint32_t base_vaddr;
    uint32_t entry_vaddr;
} elf32_info;

kernel_return_t elf32_load_exec(void *file, uint32_t physical_address, elf32_info *program_info);

bool elf32_is_valid(elf32_header *header);

bool elf32_is_suported(elf32_header *header);

#endif
