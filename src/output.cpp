
#include "output.hpp"
#include "util.hpp"

/*
 * OutputGenerator
 */

OutputGenerator::OutputGenerator(const std::string& path) {
	stream = getOutputFile(path);
}

OutputGenerator::~OutputGenerator() {
	// noop
}

/*
 * OutputDispatcher
 */

void OutputDispatcher::addGenerator(OutputGenerator* generator) {
	generators.emplace_back(generator);
}

void OutputDispatcher::addSymbol(const char* name, const std::vector<uint8_t>& data) {
	for (auto& generator : generators) generator->createSymbol(name, data);
}

void OutputDispatcher::flush() {
	for (auto& generator : generators) generator->flush();
}