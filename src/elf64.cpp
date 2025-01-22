
#include "elf64.hpp"
#include <cstring>
#include <utility>
#include "struct/elf64.hpp"

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

	// identify file origin
	std::string signature = "EMBER Resource File 1.0";
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

void Elf64::createSymbol(const std::string& name, const std::vector<uint8_t>& data) {

	int index = symtab.buffer->size() / sizeof(Elf64_Sym);
	auto& sym = symtab.buffer->resize(sizeof(Elf64_Sym)).as<Elf64_Sym>(index);

	sym.st_shndx = rodata.index;
	sym.st_name = strtab.buffer->size();
	sym.st_value = rodata.buffer->size();
	sym.st_size = data.size();

	sym.st_info = 0x11; // STT_OBJECT (1<<4), STB_GLOBAL (1)
	sym.st_other = 0; // STV_DEFAULT

	// copy name into symbol name buffer
	strtab.buffer->appendBytes(name.c_str(), name.size() + 1);

	// copy data into rodata buffer
	rodata.buffer->appendVector(data);

}

Elf64::Elf64(const std::string& path)
: SegmentedFile(path) {

	createHeader();

	this->shdrtab = appendBuffer(sizeof(Elf64_Shdr));
	this->shstrtab = appendBuffer(1);
	this->shstrtab->appendBytes("", 1); // first byte in shstrtab must be 0

	createSection(SHT_NULL, 0, "", nullptr);
	createSection(SHT_STRTAB, 0, ".shstrtab", shstrtab);

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
