
#include "coff.hpp"
#include <cstring>
#include <algorithm>
#include "struct/coff.hpp"

static_assert(sizeof(Coff_Symb) == 18);

void Coff::createHeader() {

	auto buffer = appendBuffer(1);
	Coff_Head head {};

	// COFF files with machine=0 are not recognized on linux but work on windows
	// albeit with a warning if this object file is the first one given to the linker
	head.machine = 0;

	head.timdat = (uint32_t) time(nullptr); // idk
	head.opthdr = 0; // object files don't have the optional header
	head.flags = 0;

	buffer->addLink([this] (ByteBuffer& buffer) {
		Coff_Head* head = &buffer.as<Coff_Head>();
		head->seccnt = section_count;
		head->symcnt = symbol_count;
		head->symoff = symbols->getStartOffset();
	});

	buffer->appendObject(head);

}

long Coff::createSection(const std::string& name, uint32_t flags, std::shared_ptr<ByteBuffer> data) {

	int upper = 8;
	int limit = std::min((int) name.size(), upper);
	int index = section_count;

	Coff_Sect& sect = sections->resize(sizeof(Coff_Sect)).as<Coff_Sect>(index);

	memset(sect.name, 0, upper);
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
		auto& sect = buffer.as<Coff_Sect>(index);
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

	const uint32_t flags = IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_ALIGN_1BYTES | IMAGE_SCN_ALIGN_2BYTES | IMAGE_SCN_MEM_READ;
	this->rdata_index = createSection(".rdata", flags, rodata);

	// keep symbols at the end
	this->symbols = appendBuffer(4);

	// ... and strings after symbols
	this->strings = appendBuffer(1);
	this->strings->resize(4);

	this->strings->addLink([] (ByteBuffer& buffer) {
		buffer.as<uint32_t>(0) = buffer.size();
	});

}

void Coff::createSymbol(const Symbol& symbol) {

	const int index = symbol_count;
	Coff_Symb& symb = symbols->resize(sizeof(Coff_Symb)).as<Coff_Symb>(index);

	// don't mangle the name, more research is necessary
	std::string mangled = symbol.name;

	// technically we can do up to 8, but don't bother
	if (mangled.size() <= 7) {
		memcpy(symb.id.name, mangled.c_str(), mangled.size() + 1);
	} else {
		symb.id.zero = 0;
		symb.id.offset = strings->size();
		strings->appendString(mangled);
	}

	symb.value = rodata->size();
	symb.section = rdata_index + 1;
	symb.storage = IMAGE_SYM_CLASS_EXTERNAL;
	symb.base = IMAGE_SYM_TYPE_CHAR;
	symb.complex = IMAGE_SYM_DTYPE_ARRAY;
	symb.auxs = 0;

	symbol_count ++;
	rodata->appendVector(symbol.data);

}