#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <cinttypes>

struct Symbol {

	public:

		std::vector<uint8_t> data;
		std::string file;
		std::string name;

		std::vector<uint8_t> prefix;
		std::vector<uint8_t> suffix;

		uint64_t limit = -1;

		void print();

	public:

		void setFile(const std::string& file);
		void setName(const std::string& name);
		void resolve(const std::string& path);
};
