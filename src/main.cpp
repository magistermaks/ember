
#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include "elf64.hpp"
#include "args.hpp"
#include "util.hpp"
#include "header.hpp"
#include "coff.hpp"

int main(int argc, char** argv) {

	try {

		Args args = Args::load(argc, argv);

		std::vector<uint8_t> bytes = getBytes(getInputFile("hello.txt"));

		OutputDispatcher dispatcher;

		if (args.elf64.enabled) {
			dispatcher.addGenerator(new Elf64 {args.elf64.value});
		}

		if (args.coff32.enabled) {
			dispatcher.addGenerator(new Coff {args.coff32.value});
		}

		if (args.header.enabled) {
			dispatcher.addGenerator(new HeaderFile {args.header.value});
		}

		dispatcher.addSymbol("test", bytes);
		dispatcher.flush();

	} catch (std::runtime_error& error) {
		std::cout << error.what() << std::endl;
		return -1;
	}

	return 0;
}
