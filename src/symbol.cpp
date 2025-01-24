
#include "symbol.hpp"
#include "util.hpp"
#include <iostream>
#include <filesystem>

void Symbol::print() {
	std::cout << "symbol '" << name << "' from '" << file << "'\n";
}

void Symbol::setFile(const std::string& file) {
	this->file = file;

	std::filesystem::path path {file};
	std::string stem = path.stem();

	// construct *some* default symbol name
	if (name.empty()) {
		for (char chr : stem) {
			if (isalnum(chr) || chr == '_') {
				name.push_back(chr);
			}
		}
	}

}

void Symbol::setName(const std::string& name) {
	for (char chr : name) {
		if (!isalnum(chr) && chr != '_') {
			throw std::runtime_error {"Non [a-zA-Z_] character in symbol identifier '" + name + "' found!"};
		}
	}

	this->name = name;
}

void Symbol::resolve(const std::string& store) {

	std::filesystem::path path {this->file};
	std::filesystem::path parent {store};

	this->data = getBytes(getInputFile(parent / path));
}
