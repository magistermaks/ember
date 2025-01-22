
#include "header.hpp"

/*
 * HeaderFile
 */

HeaderFile::HeaderFile(const std::string& path)
: OutputGenerator(path) {
	stream << "#pragma once\n\n";
}

void HeaderFile::createSymbol(const std::string& name, const std::vector<uint8_t>& data) {
	stream << "// Contents of file '/" << name << "'\n";
	stream << "extern const char " << name << "[" << data.size() << "];\n\n";
}

void HeaderFile::flush() {
	stream.flush();
}