
#include "args.hpp"
#include <stdexcept>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>

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

std::string Args::StringFlag::getOrDefault(const std::string& fallback) {
	return enabled ? value : fallback;
}

/*
 * Args
 */

void Args::beginSymbolDefinition() {
	this->expect = InputFlag::SYMBOL_FILE;
	this->mode = InputMode::SYMBOLS;
	symbols.emplace_back();
}

Symbol& Args::getCurrentSymbol() {
	if (symbols.empty()) {
		raiseError("no symbol present");
	}

	return symbols.back();
}

std::vector<uint8_t> Args::parseAsArray(const std::string& list) {
	std::vector<uint8_t> array;
	std::string token;

	int minor = 0; // char
	int major = 0; // string

	auto submit = [&] () {
		if (!token.empty()) {
			array.push_back(parseAsNumber(token));
		}
	};

	for (char chr : list) {
		if (major == 1) {
			if (chr == '"') {
				major = 0;
				continue;
			}

			array.push_back(chr);
			continue;
		}

		if (minor == 2) {
			if (chr != '\'') {
				raiseError("Expected single quotation (') in affix \"" + list + "\"");
			}

			minor = 0;
			continue;
		}

		if (minor == 1) {
			array.push_back(chr);
			minor = 2;
			continue;
		}

		if (chr == ',') {
			submit();
			token = "";
			continue;
		}

		if (chr == '\'') {
			submit();
			minor = 1;
			continue;
		}

		if (chr == '"') {
			submit();
			major = 1;
			continue;
		}

		token += chr;
	}

	if (major != 0) {
		raiseError("Expected quotation mark (\") but end of argument \"" + list + "\" was reached");
	}

	if (minor != 0) {
		raiseError("Expected single quotation (') but end of argument \"" + list + "\" was reached");
	}

	submit();
	return array;
}

int64_t Args::parseAsNumber(const std::string& number) {

	int base = 10;
	const char* string = number.c_str();

	if ((number.size() >= 2) && (number[0] == '0')) {
		if (number[1] == 'x') {
			base = 16;
			string += 2;
			goto parse;
		}

		if (number[1] == 'o') {
			base = 8;
			string += 2;
			goto parse;
		}

		if (number[1] == 'b') {
			base = 2;
			string += 2;
			goto parse;
		}
	}

	parse:

	char* end = nullptr;
	int64_t value = strtoll(string, &end, base);

	if (end != number.c_str() + number.size()) {
		throw std::runtime_error {"Failed to fully parse '" + number + "' as a number!"};
	}

	return value;

}

void Args::nextOptionFlag(const std::string& flg) {

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

	if (flg == "-s" || flg == "--store") {
		this->expect = InputFlag::STORE_PATH;
		return;
	}

	if (flg == "-f" || flg == "--file") {
		beginSymbolDefinition();
		return;
	}

	if (flg == "-m" || flg == "--mutable") {
		writeable = true;
		return;
	}

	raiseError("unknown option flag '" + flg + "'");

}

void Args::nextSymbolFlag(const std::string& flg) {

	if (flg == "-f" || flg == "--file") {
		beginSymbolDefinition();
		return;
	}

	if (flg == "-n" || flg == "--name") {
		this->expect = InputFlag::SYMBOL_NAME;
		return;
	}

	if (flg == "-l" || flg == "--limit") {
		this->expect = InputFlag::SYMBOL_LIMIT;
		return;
	}

	if (flg == "-b" || flg == "--prefix") {
		this->expect = InputFlag::SYMBOL_PREFIX;
		return;
	}

	if (flg == "-e" || flg == "--suffix") {
		this->expect = InputFlag::SYMBOL_SUFFIX;
		return;
	}

}

void Args::raiseError(const std::string& message) {
	throw std::runtime_error {"Syntax error, " + message + "!\nSee 'ember --help' for more info"};
}

void Args::nextFlag(std::string flg) {

	// this is to protect against invalid short flag chaining
	if (this->expect != InputFlag::NONE) {
		raiseError("expected flag value");
	}

	if (mode == InputMode::OPTIONS) {
		nextOptionFlag(flg);
		return;
	}

	nextSymbolFlag(flg);

}

bool Args::accept(InputFlag flag) {
	if (this->expect == flag) {
		this->expect = InputFlag::NONE;
		return true;
	}

	return false;
}

void Args::parseArgument(const char* arg) {

	std::cout << "Parsing argument: " << arg << "\n";

	if (accept(InputFlag::ELF_64_PATH)) {
		elf64.setValue(arg);
		return;
	}

	if (accept(InputFlag::COFF_32_PATH)) {
		coff32.setValue(arg);
		return;
	}

	if (accept(InputFlag::HEADER_PATH)) {
		header.setValue(arg);
		return;
	}

	if (accept(InputFlag::STORE_PATH)) {
		store.setValue(arg);
		return;
	}

	if (accept(InputFlag::SYMBOL_FILE)) {
		getCurrentSymbol().setFile(arg);
		return;
	}

	if (accept(InputFlag::SYMBOL_NAME)) {
		getCurrentSymbol().setName(arg);
		return;
	}

	if (accept(InputFlag::SYMBOL_LIMIT)) {
		getCurrentSymbol().limit = parseAsNumber(arg);
		return;
	}

	if (accept(InputFlag::SYMBOL_PREFIX)) {
		getCurrentSymbol().prefix = parseAsArray(arg);
		return;
	}

	if (accept(InputFlag::SYMBOL_SUFFIX)) {
		getCurrentSymbol().suffix = parseAsArray(arg);
		return;
	}

	if (arg[0] == '-') {

		// starts with '--'
		if (std::strlen(arg) > 1 && arg[1] == '-') {
			nextFlag(arg);
			return;
		}

		// starts with '-'
		for (int k = 1; arg[k] != 0; k ++) {
			std::string flag {arg[k]};
			nextFlag('-' + flag);
		}

		return;
	}

	raiseError("invalid argument '" + std::string(arg) + "'");

}

Args Args::load(int argc, char** argv) {

	Args args;

	for (int i = 1; i < argc; i ++) {
		args.parseArgument(argv[i]);
	}

	if (args.help) {
		printf("Usage: ember [option flags] [-f|--file <path> [symbol flags]]...\n");
		printf("Create linkable resource files\n\n");

		printf("Valid option flags:\n");
		printf("  -?, --help            Show this help page\n");
		printf("  -m, --mutable         Should the resources be non-const\n");
		printf("  -H, --header  <path>  Generate C header file\n");
		printf("  -E, --elf     <path>  Generate ELF (64 bit) file\n");
		printf("  -C, --coff    <path>  Generate COFF (32 bit) file\n");
		printf("  -s, --store   <path>  Resource store path\n");
		printf("  -f, --file    <path>  Resource file, can be relative to store path\n\n");

		printf("Once the --file flag is used program enters into\n");
		printf("symbol definition mode and only symbol flags can be used,\n");
		printf("at any point the --file flag can be used again to begin another symbol.\n\n");

		printf("Valid symbol flags:\n");
		printf("  -n, --name    <name>  Valid C identifier of the symbol\n");
		printf("  -l, --limit   <size>  Limit the number of bytes read from file\n");
		printf("  -b, --prefix  <byte>, List of bytes to prepend\n");
		printf("  -e, --suffix  <byte>, List of bytes to append\n");
		exit(0);
	}

	for (Symbol& symbol : args.symbols) {
		symbol.resolve(args.store.getOrDefault("./"));
	}

	return args;

}