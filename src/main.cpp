
#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include "elf64.hpp"
#include "args.hpp"
#include "util.hpp"

int main(int argc, char** argv) {

	try {

		Args args = Args::load(argc, argv);

		std::vector<uint8_t> bytes = getBytes(getInputFile("hello.txt"));

		OutputDispatcher dispatcher;
		dispatcher.addGenerator(new Elf64 {args.file + ".o"});
		dispatcher.addSymbol("test", bytes);
		dispatcher.flush();

	} catch (std::runtime_error& error) {
		std::cout << error.what() << std::endl;
		return -1;
	}

	return 0;
}
