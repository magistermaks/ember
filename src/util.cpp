
#include "util.hpp"
#include <stdexcept>

std::ifstream getInputFile(const std::string& path) {
	std::ifstream input {path, std::ios::binary};

	if (!input.is_open()) {
		throw std::runtime_error {"Failed to open input file '" + path + "'"};
	}

	return input;
}

std::ofstream getOutputFile(const std::string& path) {
	std::ofstream output {path, std::ios::binary};

	if (!output.is_open()) {
		throw std::runtime_error {"Failed to open output file '" + path + "'"};
	}

	return output;
}

std::vector<uint8_t> getBytes(std::ifstream file) {
	std::vector<uint8_t> content ((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	return content;
}