#pragma once

#include "file.hpp"

class Coff : public SegmentedFile {

	private:

		int section_count = 0;
		int symbol_count = 0;
		int rdata_index = 0;

		std::shared_ptr<ByteBuffer> sections;
		std::shared_ptr<ByteBuffer> symbols;
		std::shared_ptr<ByteBuffer> rodata;
		std::shared_ptr<ByteBuffer> strings;

		void createHeader();
		long createSection(const std::string& name, uint32_t flags, std::shared_ptr<ByteBuffer> data);

	public:

		Coff(const std::string& path);

		void createSymbol(const std::string& name, const std::vector<uint8_t>& data) override;

};
