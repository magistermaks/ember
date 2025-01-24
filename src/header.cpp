
#include "header.hpp"

/*
 * HeaderFile
 */

HeaderFile::HeaderFile(const std::string& path)
: OutputGenerator(path) {
	stream << "#pragma once\n\n";
}

void HeaderFile::createSymbol(const Symbol& symbol) {
	stream << "// Contents of file '" << symbol.file << "'\n";
	stream << "extern const char " << symbol.name << "[" << symbol.data.size() << "];\n\n";
}

void HeaderFile::flush() {
	stream.flush();
}