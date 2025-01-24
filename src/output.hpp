#pragma once

#include <vector>
#include <cinttypes>
#include <memory>
#include <fstream>
#include "symbol.hpp"

class OutputGenerator {

	protected:

		std::ofstream stream;

	public:

		OutputGenerator(const std::string& path);

		virtual ~OutputGenerator();
		virtual void createSymbol(const Symbol& symbol) = 0;
		virtual void flush() = 0;

};

class OutputDispatcher  {

	private:

		std::vector<std::unique_ptr<OutputGenerator>> generators;

	public:

		void addGenerator(OutputGenerator* generator);
		void addSymbol(const Symbol& symbol);
		void flush();

};
