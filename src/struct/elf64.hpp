#pragma once

#include <cinttypes>

#define ET_REL 1
#define EM_NONE 0
#define EM_386 3
#define EM_X86_64 62
#define EV_CURRENT 1
#define ELFCLASS64 2
#define ELFDATA2LSB 1
#define ELFOSABI_NONE 0

#define SHF_WRITE 0x1
#define SHF_ALLOC 0x2
#define SHF_EXECINSTR 0x4
#define SHF_MERGE 0x10
#define SHF_STRINGS 0x20
#define SHF_INFO_LINK 0x40
#define SHF_LINK_ORDER 0x80
#define SHF_OS_NONCONFORMING 0x100
#define SHF_GROUP 0x200
#define SHF_TLS 0x400

#define SHT_NULL 0
#define SHT_PROGBITS 1
#define SHT_SYMTAB 2
#define SHT_STRTAB 3
#define SHT_RELA 4
#define SHT_HASH 5
#define SHT_DYNAMIC 6
#define SHT_NOTE 7
#define SHT_NOBITS 8
#define SHT_REL 9
#define SHT_SHLIB 10
#define SHT_DYNSYM 11
#define SHT_INIT_ARRAY 14
#define SHT_FINI_ARRAY 15
#define SHT_PREINIT_ARRAY 16
#define SHT_GROUP 17
#define SHT_SYMTAB_SHNDX 18

#define SECTION_NULL 0
#define SECTION_SHSTRTAB 1

#pragma pack(push, 1)
typedef struct {
	uint8_t magic[4];
	uint8_t clazz;
	uint8_t data;
	uint8_t version;
	uint8_t abi;
	uint8_t abi_version;
	uint8_t pad[7];
} Elf64_Ident;

typedef struct {
	Elf64_Ident ident;
	uint16_t type;
	uint16_t machine;
	uint32_t version;
	uint64_t entry;
	uint64_t phoff;
	uint64_t shoff;
	uint32_t flags;
	uint16_t ehsize;
	uint16_t phentsize;
	uint16_t phnum;
	uint16_t shentsize;
	uint16_t shnum;
	uint16_t shstrndx;
} Elf64_Ehdr;

typedef struct {
	uint32_t name;      // offset into the shstrtab
	uint32_t type;      // section type
	uint64_t flags;     // section flags
	uint64_t addr;      // where the section should be placed in process memory, or 0
	uint64_t offset;    // offset from file start to the data buffer
	uint64_t size;      // size of the section in the file
	uint32_t link;      // extra linked section (type dependant)
	uint32_t info;      // extra info section (type dependant)
	uint64_t addralign; // alignment
	uint64_t entsize;   // if section holds a struct array, this is the size of one entry, or 0
} Elf64_Shdr;

typedef struct {
	uint32_t st_name;   // index into the symbol string table (pointer by link)
	uint8_t  st_info;   // symbol type and binding attributes
	uint8_t  st_other;  // symbol visibility
	uint16_t st_shndx;  // section index where the symbol data is located
	uint64_t st_value;  // symbol value (in our case this is the offset)
	uint64_t st_size;   // symbol size in bytes
} Elf64_Sym;
#pragma pack(pop)