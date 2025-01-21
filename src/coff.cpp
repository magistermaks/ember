
#include "coff.hpp"
#include <cstring>

#define IMAGE_FILE_MACHINE_UNKNOWN 0x00
#define IMAGE_SYM_CLASS_EXTERNAL 2
#define IMAGE_SYM_DTYPE_ARRAY 3
#define IMAGE_SYM_TYPE_CHAR 2

#pragma pack(push, 1)
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
	uint8_t  name[8]; // symbol name
	uint32_t value;   // offset in the section for 'EXTERNAL' symbols
	uint16_t section; // section where the symbol is stored
	uint8_t  base;    // base type
	uint8_t  complex; // complex type
	uint8_t  storage; // indicates what kind of definition a symbol represents
	uint8_t  auxs;    // number of auxiliary symbols that follow
} Coff_Symb;
#pragma pack(pop)

static_assert(sizeof(Coff_Symb) == 18);

void Coff::createHeader() {

	auto buffer = appendBuffer(1);
	Coff_Head* head = &buffer->resize(sizeof(Coff_Head)).as<Coff_Head>();

	head->machine = 0x14c;
	head->timdat = (uint32_t) time(nullptr); // idk
	head->opthdr = 0; // object files don't have the optional header
	head->flags = 0;

	buffer->addLink([this] (ByteBuffer& buffer) {
		Coff_Head* head = &buffer.as<Coff_Head>();
		head->seccnt = section_count;
		head->symcnt = symbol_count;
		head->symoff = symbols->getStartOffset();
	});

}

long Coff::createSection(const std::string& name, uint32_t flags, std::shared_ptr<ByteBuffer> data) {

	int limit = std::min(name.size(), 8ul);
	int index = section_count;

	Coff_Sect& sect = sections->resize(sizeof(Coff_Sect)).as<Coff_Sect>(index);

	memset(sect.name, 0, 8);
	memcpy(sect.name, name.c_str(), limit);

	// make it so none of our sections actually exist in memory
	sect.vaddr = 0;
	sect.vsize = 0;

	sect.rloff = 0;
	sect.rlcnt = 0;
	sect.lnoff = 0;
	sect.lncnt = 0;
	sect.flags = flags;

	section_count ++;

	sections->addLink([=] (ByteBuffer& buffer) {
		Coff_Sect& sect = buffer.as<Coff_Sect>(index);
		sect.offset = data->getStartOffset();
		sect.size = data->size();
	});

	return index;
}

Coff::Coff(const std::string& path)
: SegmentedFile(path) {

	createHeader();

	this->sections = appendBuffer(1);
	this->rodata = appendBuffer(8);

	this->rdata_index = createSection(".rdata", 0x400000, rodata);

	// keep symbols at the end
	this->symbols = appendBuffer(1);

	// ... and strings after symbols
	this->strings = appendBuffer(1);
	this->strings->resize(4);

	this->strings->addLink([] (ByteBuffer& buffer) {
		buffer.as<uint32_t>(0) = buffer.size();
	});

}

void Coff::createSymbol(const char* name, const std::vector<uint8_t>& data) {

	int index = symbol_count;

	Coff_Symb& symb = symbols->resize(sizeof(Coff_Symb)).as<Coff_Symb>(index);

	memcpy(symb.name, "_test", 6);

	symb.value = rodata->size();
	symb.section = rdata_index + 1;
	symb.storage = IMAGE_SYM_CLASS_EXTERNAL;
	symb.base = 0;//IMAGE_SYM_TYPE_CHAR;
	symb.complex = IMAGE_SYM_DTYPE_ARRAY;
	symb.auxs = 0;

	symbol_count ++;
	rodata->append(data);

}