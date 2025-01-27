#pragma once

#include "output.hpp"

class HeaderFile : public OutputGenerator {

	private:

		bool writeable;

	public:

		HeaderFile(const std::string& path, bool writeable);

		void createSymbol(const Symbol& symbol) override;
		void flush() override;

};
