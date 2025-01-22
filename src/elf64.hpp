#pragma once

#include "file.hpp"
#include "output.hpp"

struct Elf64Section {

	std::shared_ptr<ByteBuffer> buffer;
	int index;
	int entry_size = 0;

};

class Elf64 : public SegmentedFile {

	private:

		std::vector<Elf64Section> sections;
		std::shared_ptr<ByteBuffer> shdrtab;
		std::shared_ptr<ByteBuffer> shstrtab;

		// the thing we want to create
		Elf64Section symtab;
		Elf64Section strtab;
		Elf64Section rodata;
		Elf64Section shstab;

		void createHeader();
		void createExtras();

		Elf64Section createSection(int type, int flags, const char* name, std::shared_ptr<ByteBuffer> buffer, int link = 0);

	public:

		Elf64(const std::string& path);

		void createSymbol(const std::string& name, const std::vector<uint8_t>& data) override;

};
