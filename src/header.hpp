#pragma once

#include "output.hpp"

class HeaderFile : public OutputGenerator {

	public:

		HeaderFile(const std::string& path);

		void createSymbol(const Symbol& symbol) override;
		void flush() override;

};
