
#include "util.hpp"
#include <stdexcept>
#include <filesystem>
#include <algorithm>

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

std::vector<uint8_t> getBytes(std::ifstream file, uint64_t start, uint64_t limit, uint64_t end) {

	uint64_t offset = 0;
	std::vector<uint8_t> content;

	// real limit cap
	uint64_t dist = end - start;
	limit = std::min(dist, limit);

	// don't resize the buffer to limit
	// even if limit is less than -1 to avoid fun errors

	for (std::istreambuf_iterator<char> file_iter(file), file_end; file_iter != file_end; ++ file_iter) {
		uint8_t b = *file_iter;

		if (offset < start) {
			continue;
		}

		if (offset >= limit) {
			break;
		}

		content.push_back(b);
		offset ++;
	}

	return content;
}