#pragma once

#include <cinttypes>

#define IMAGE_FILE_MACHINE_UNKNOWN 0x00
#define IMAGE_SYM_CLASS_EXTERNAL 2
#define IMAGE_SYM_DTYPE_ARRAY 3
#define IMAGE_SYM_TYPE_CHAR 2

#define IMAGE_SCN_CNT_INITIALIZED_DATA 0x00000040
#define IMAGE_SCN_CNT_UNINITIALIZED_DATA 0x00000080
#define IMAGE_SCN_ALIGN_1BYTES 0x00100000
#define IMAGE_SCN_ALIGN_2BYTES 0x00200000
#define IMAGE_SCN_ALIGN_4BYTES 0x00300000
#define IMAGE_SCN_ALIGN_8BYTES 0x00400000
#define IMAGE_SCN_MEM_EXECUTE 0x20000000
#define IMAGE_SCN_MEM_READ 0x40000000
#define IMAGE_SCN_MEM_WRITE 0x80000000

#pragma pack(push, 1)
typedef struct {
	union {
		uint8_t name[8];

		struct {
			uint32_t zero;
			uint32_t offset;
		};
	};
} Coff_Ident;

typedef struct {
	uint16_t machine; // machine number
	uint16_t seccnt;  // number of sections
	uint32_t timdat;  // time stamp
	uint32_t symoff;  // symbol table offset from file start
	uint32_t symcnt;  // number of symbols
	uint16_t opthdr;  // size of the "optional header"
	uint16_t flags;   // flags
} Coff_Head;

typedef struct {
	uint8_t  name[8]; // section name
	uint32_t vsize;   // virtual memory size
	uint32_t vaddr;   // virtual memory address
	uint32_t size;    // section size in file
	uint32_t offset;  // file offset to section data
	uint32_t rloff;   // file offset to the relocation table for this section
	uint32_t lnoff;   // File offset to the line number table for this section
	uint16_t rlcnt;   // number of relocations
	uint16_t lncnt;   // number of line numbers
	uint32_t flags;   // flags
} Coff_Sect;

typedef struct {
	Coff_Ident id;    // symbol name
	uint32_t value;   // offset in the section for 'EXTERNAL' symbols
	uint16_t section; // section where the symbol is stored
	uint8_t  base;    // base type
	uint8_t  complex; // complex type
	uint8_t  storage; // indicates what kind of definition a symbol represents
	uint8_t  auxs;    // number of auxiliary symbols that follow
} Coff_Symb;
#pragma pack(pop)