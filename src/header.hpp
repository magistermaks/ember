#pragma once

#include "output.hpp"

class HeaderFile : public OutputGenerator {

	public:

		HeaderFile(const std::string& path);

		void createSymbol(const char* name, const std::vector<uint8_t>& data);
		void flush();

};
