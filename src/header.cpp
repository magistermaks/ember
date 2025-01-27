
#include "header.hpp"

/*
 * HeaderFile
 */

HeaderFile::HeaderFile(const std::string& path, bool writeable)
: OutputGenerator(path), writeable(writeable) {
	stream << "#pragma once\n\n";
}

void HeaderFile::createSymbol(const Symbol& symbol) {
	stream << "// Contents of file '" << symbol.file << "'\n";
	stream << "extern ";

	if (!writeable) {
		stream << "const ";
	}

	stream << "char " << symbol.name << "[" << symbol.data.size() << "];\n\n";
}

void HeaderFile::flush() {
	stream.flush();
}