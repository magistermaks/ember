
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include "file.hpp"
#include "elf64.hpp"

static std::ifstream getInputFile(const std::string& path) {
	std::ifstream input {path, std::ios::binary};

	if (!input.is_open()) {
		throw std::runtime_error {"Failed to open input file '" + path + "'"};
	}

	return input;
}

static std::ofstream getOutputFile(const std::string& path) {
	std::ofstream output {path, std::ios::binary};

	if (!output.is_open()) {
		throw std::runtime_error {"Failed to open output file '" + path + "'"};
	}

	return output;
}

static std::vector<uint8_t> getBytes(std::ifstream file) {
	std::vector<uint8_t> content ((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	return content;
}

int main() {

	try {

		std::ofstream output = getOutputFile("library.o");
		std::vector<uint8_t> bytes = getBytes(getInputFile("hello.txt"));

		Elf64 elf;
		elf.createSymbol("test", bytes);
		elf.write(output);

	} catch (std::runtime_error& error) {
		std::cout << error.what() << std::endl;
		return -1;
	}

	return 0;
}
