
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
		OutputDispatcher dispatcher;

		if (args.elf64.enabled) {
			dispatcher.addGenerator(new Elf64 {args.elf64.value, args.writeable});
		}

		if (args.coff32.enabled) {
			dispatcher.addGenerator(new Coff {args.coff32.value, args.writeable});
		}

		if (args.header.enabled) {
			dispatcher.addGenerator(new HeaderFile {args.header.value, args.writeable});
		}

		for (Symbol& symbol : args.symbols) {
			dispatcher.addSymbol(symbol);
		}

		dispatcher.flush();

	} catch (std::runtime_error& error) {
		std::cout << error.what() << std::endl;
		return -1;
	}

	return 0;
}
