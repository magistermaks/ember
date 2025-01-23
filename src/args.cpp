
#include "args.hpp"
#include <stdexcept>
#include <cstdio>
#include <cstring>

/*
 * Args::StringFlag
 */

Args::StringFlag::StringFlag() {
	this->enabled = false;
}

void Args::StringFlag::setValue(const std::string& value) {
	this->enabled = true;
	this->value = value;
}

/*
 * Args
 */

void Args::raiseError(const std::string& message) {
	throw std::runtime_error {"Syntax error, " + message + "!\nSee 'ember --help' for more info"};
}

void Args::nextFlag(std::string flg) {

	// this is to protect against invalid short flag chaining
	if (this->expect != InputFlag::NONE) {
		raiseError("expected flag value");
	}

	if (flg == "-?" || flg == "--help") {
		this->help = true;
		return;
	}

	if (flg == "-H" || flg == "--header") {
		this->expect = InputFlag::HEADER_PATH;
		return;
	}

	if (flg == "-E" || flg == "--elf") {
		this->expect = InputFlag::ELF_64_PATH;
		return;
	}

	if (flg == "-C" || flg == "--coff") {
		this->expect = InputFlag::COFF_32_PATH;
		return;
	}

	raiseError("unknown flag '" + flg + "'");
}

bool Args::accept(InputFlag flag) {
	if (this->expect == flag) {
		this->expect = InputFlag::NONE;
		return true;
	}

	return false;
}

Args Args::load(int argc, char** argv) {

	Args args;

	for (int i = 1; i < argc; i ++) {
		const char* arg = argv[i];

		if (args.accept(InputFlag::ELF_64_PATH)) {
			args.elf64.setValue(arg);
			continue;
		}

		if (args.accept(InputFlag::COFF_32_PATH)) {
			args.coff32.setValue(arg);
			continue;
		}

		if (args.accept(InputFlag::HEADER_PATH)) {
			args.header.setValue(arg);
			continue;
		}

		if (arg[0] == '-') {

			// starts with '--'
			if (std::strlen(arg) > 1 && arg[1] == '-') {
				args.nextFlag(arg);
				continue;
			}

			// starts with '-'
			for (int k = 1; arg[k] != 0; k ++) {
				std::string flag {arg[k]};
				args.nextFlag('-' + flag);
			}

			continue;
		}

	}

	if (args.help) {
		printf("Usage: ember [options]\n");
		printf("Create linkable resource files\n\n");

		printf("Valid option flags:\n");
		printf("  -?, --help           Show this help page\n");
		printf("  -H, --header <path>  Generate C header file\n");
		printf("  -E, --elf    <path>  Generate ELF (64 bit) file\n");
		printf("  -C, --coff   <path>  Generate COFF (32 bit) file\n");
		exit(0);
	}

	return args;

}