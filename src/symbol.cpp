
#include "symbol.hpp"
#include "util.hpp"
#include <iostream>
#include <filesystem>

void Symbol::print() {
	std::string limited = (this->limit != -1) ? std::to_string(this->limit) : "unlimited";
	std::cout << "Embedding symbol '" << name << "' from '" << file << "' [" << limited << "]\n";
}

void Symbol::setFile(const std::string& file) {
	this->file = file;

	std::filesystem::path path {file};
	std::string stem = path.stem().generic_string();

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
	print();

	std::filesystem::path path {this->file};
	std::filesystem::path parent {store};

	for (int i : prefix) {
		std::cout << i << "\n";

	}

	this->data.insert(data.end(), prefix.begin(), prefix.end());
	loadBytes(this->data, getInputFile(parent / path), 0, limit, -1);
	this->data.insert(data.end(), suffix.begin(), suffix.end());
}
