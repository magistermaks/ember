
#include "args.hpp"
#include <stdexcept>
#include <cstdio>

void Args::raiseError(const std::string& message) {
	throw std::runtime_error {"Syntax error, " + message + "!\nSee 'ember -h' for more info"};
}

void Args::nextFlag(char flg) {

	if (this->flag != InputFlag::NONE) {
		raiseError("expected flag value");
	}

	if (flg == 'h' || flg == '?') {
		this->h = true;
		return;
	}

	if (flg == 'c') {
		this->c = true;
		return;
	}

	if (flg == 'o') {
		this->flag = InputFlag::OUTPUT;
		return;
	}

	raiseError("unknown flag");
}

Args Args::load(int argc, char** argv) {

	Args args;

//	if (argc == 1) {
//		raiseError("file or option expected");
//	}

	for (int i = 1; i < argc; i ++) {
		const char* arg = argv[i];

		if (arg[0] == '-') {
			for (int k = 1; arg[k] != 0; k ++) {
				args.nextFlag(arg[k]);
			}

			continue;
		}

		if (args.flag == InputFlag::OUTPUT) {
			args.file = arg;
			args.flag = InputFlag::NONE;
			continue;
		}

	}

	if (args.h) {
		printf("Usage: ember [options]\n");
		printf("Create linkable resource files\n\n");

		printf("Valid option flags:\n");
		printf("  -h, -?      Show this help page\n");
		printf("  -c          Generate C header file\n");
		printf("  -o <name>   Output file name\n");
		exit(0);
	}

	return args;

}