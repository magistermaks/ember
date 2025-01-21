
#include "elf64.hpp"
#include <cstring>
#include <utility>

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

typedef struct {
	uint32_t namesz;
	uint32_t descsz;
	uint32_t type;
	char name[4];
} Gnu64_Property;

void Elf64::createHeader() {

	auto buffer = appendBuffer(1);
	Elf64_Ehdr* ehdr = &buffer->resize(sizeof(Elf64_Ehdr)).as<Elf64_Ehdr>();
	Elf64_Ident* ident = &ehdr->ident;

	// magic bytes
	ident->magic[0] = 0x7f;
	ident->magic[1] = 'E';
	ident->magic[2] = 'L';
	ident->magic[3] = 'F';

	// rest of the identification
	ident->clazz = ELFCLASS64;
	ident->data = ELFDATA2LSB;
	ident->version = EV_CURRENT;
	ident->abi = ELFOSABI_NONE;
	ident->abi_version = 0;
	std::memset(ident->pad, 0, 7);

	// now on with the rest of the header
	ehdr->type = ET_REL; // relocatable file
	ehdr->machine = EM_X86_64; // not sure if this is valid but we don't care about the machine arch here
	ehdr->version = EV_CURRENT;
	ehdr->entry = 0; // this file won't be executable
	ehdr->phoff = 0; // no program header table
	ehdr->shoff = sizeof(Elf64_Ehdr);
	ehdr->flags = 0; // no flags
	ehdr->ehsize = sizeof(Elf64_Ehdr); // header size
	ehdr->phentsize = 0; // size of one entry in program header table FIXME?
	ehdr->phnum = 0; // number of entries in program header table
	ehdr->shentsize = sizeof(Elf64_Shdr); // size of section header
	ehdr->shnum = 0; // number of sections in the file
	ehdr->shstrndx = 0; // index of the string table in section table

	buffer->addLink([&] (ByteBuffer& buffer) {
		Elf64_Ehdr& ehdr = buffer.as<Elf64_Ehdr>();

		ehdr.shnum = sections.size();
		ehdr.shstrndx = SECTION_SHSTRTAB;
	});

}

void Elf64::createExtras() {

	std::string signature = "EMBER Resource File 1.0";

	// ignored, but required
	auto comment = appendBuffer(1);
	comment->appendBytes(signature.data(), signature.size());
	createSection(SHT_PROGBITS, SHF_MERGE | SHF_STRINGS, ".comment", comment);

}

Elf64Section Elf64::createSection(int type, int flags, const char* name, std::shared_ptr<ByteBuffer> buffer, int link) {

	const int index = sections.size();
	Elf64Section section;
	section.buffer = std::move(buffer);
	section.index = index;

	sections.emplace_back(section);

	auto& shdr = shdrtab->resize(sizeof(Elf64_Shdr)).as<Elf64_Shdr>(section.index);
	std::memset(&shdr, 0, sizeof(Elf64_Shdr));

	// predefined data
	shdr.flags = flags;
	shdr.type = type;
	shdr.addralign = section.buffer ? section.buffer->getAlignment() : (index == 0 ? 0 : 1);
	shdr.name = shstrtab->size();
	shdr.link = link;

	// link with section content
	shdrtab->addLink([this, index] (ByteBuffer& buffer) {
		Elf64Section& section = sections[index];

		if (!section.buffer) {
			return;
		}

		auto& shdr = shdrtab->as<Elf64_Shdr>(index);
		shdr.offset = section.buffer->getStartOffset();
		shdr.size = section.buffer->size();
		shdr.entsize = section.entry_size;
	});

	// copy name into name buffer
	shstrtab->appendBytes(name, strlen(name) + 1);

	return section;

}

void Elf64::createSymbol(const char* name, const std::vector<uint8_t>& data) {

	int index = symtab.buffer->size() / sizeof(Elf64_Sym);
	auto& sym = symtab.buffer->resize(sizeof(Elf64_Sym)).as<Elf64_Sym>(index);

	sym.st_shndx = rodata.index;
	sym.st_name = strtab.buffer->size();
	sym.st_value = rodata.buffer->size();
	sym.st_size = data.size();

	sym.st_info = 0x11; // STT_OBJECT (1<<4), STB_GLOBAL (1)
	sym.st_other = 0; // STV_DEFAULT

	// copy name into symbol name buffer
	strtab.buffer->appendBytes(name, strlen(name) + 1);

	// copy data into rodata buffer
	rodata.buffer->append(data);

}

Elf64::Elf64() {

	createHeader();

	this->shdrtab = appendBuffer(sizeof(Elf64_Shdr));
	this->shstrtab = appendBuffer(1);
	this->shstrtab->appendBytes("", 1); // first byte in shstrtab must be 0

	createSection(SHT_NULL, 0, "", nullptr);
	createSection(SHT_STRTAB, 0, ".shstrtab", shstrtab);
	createSection(SHT_PROGBITS, SHF_ALLOC | SHF_EXECINSTR, ".text", nullptr);

	this->rodata = createSection(SHT_PROGBITS, SHF_ALLOC, ".rodata", appendBuffer(1));
	this->strtab = createSection(SHT_STRTAB, 0, ".strtab", appendBuffer(1));
	this->symtab = createSection(SHT_SYMTAB, 0, ".symtab", appendBuffer(8), strtab.index);

	// prepare for symbols
	Elf64_Sym undefined {};
	strtab.buffer->appendBytes("", 1); // start with \0
	symtab.buffer->appendBytes(&undefined, sizeof(undefined));
	sections[symtab.index].entry_size = sizeof(undefined);

	shdrtab->addLink([&] (ByteBuffer& buffer) {
		buffer.as<Elf64_Shdr>(symtab.index).info = 1;
	});

	// some metadata
	createExtras();

}
