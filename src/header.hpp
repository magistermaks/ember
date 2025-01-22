#pragma once

#include "output.hpp"

class HeaderFile : public OutputGenerator {

	public:

		HeaderFile(const std::string& path);

		void createSymbol(const std::string& name, const std::vector<uint8_t>& data) override;
		void flush() override;

};
