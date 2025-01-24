
#include "output.hpp"
#include "util.hpp"
#include "symbol.hpp"

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

void OutputDispatcher::addSymbol(const Symbol& symbol) {
	for (auto& generator : generators) generator->createSymbol(symbol);
}

void OutputDispatcher::flush() {
	for (auto& generator : generators) generator->flush();
}