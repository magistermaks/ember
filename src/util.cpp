
#include "util.hpp"
#include <stdexcept>
#include <filesystem>

std::ifstream getInputFile(const std::filesystem::path& path) {
	std::ifstream input {path, std::ios::binary};

	if (!input.is_open()) {
		throw std::runtime_error {"Failed to open input file '" + path.generic_string() + "'!"};
	}

	return input;
}

std::ofstream getOutputFile(const std::filesystem::path& path) {
	std::ofstream output {path, std::ios::binary};

	if (!output.is_open()) {
		throw std::runtime_error {"Failed to open output file '" + path.generic_string() + "'!"};
	}

	return output;
}

std::vector<uint8_t> getBytes(std::ifstream file) {
	std::vector<uint8_t> content ((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	return content;
}