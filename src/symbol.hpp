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

		void print();

	public:

		void setFile(const std::string& file);
		void setName(const std::string& name);
		void resolve(const std::string& path);
};
