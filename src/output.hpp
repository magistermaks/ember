#pragma once

#include <vector>
#include <cinttypes>
#include <memory>
#include <fstream>

class OutputGenerator {

	protected:

		std::ofstream stream;

	public:

		OutputGenerator(const std::string& path);

		virtual ~OutputGenerator();
		virtual void createSymbol(const std::string& name, const std::vector<uint8_t>& data) = 0;
		virtual void flush() = 0;

};

class OutputDispatcher  {

	private:

		std::vector<std::unique_ptr<OutputGenerator>> generators;

	public:

		void addGenerator(OutputGenerator* generator);
		void addSymbol(const std::string& name, const std::vector<uint8_t>& data);
		void flush();

};
