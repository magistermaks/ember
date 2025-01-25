#pragma once

#include <string>
#include <vector>
#include <cinttypes>
#include "symbol.hpp"

class Args {

	private:

		enum InputMode {
			OPTIONS,
			SYMBOLS,
		};

		enum InputFlag {
			NONE,
			ELF_64_PATH,
			COFF_32_PATH,
			HEADER_PATH,
			STORE_PATH,

			SYMBOL_FILE,
			SYMBOL_NAME,
			SYMBOL_LIMIT,
		};

		struct StringFlag {
			bool enabled;
			std::string value;

			StringFlag();
			void setValue(const std::string& path);
			std::string getOrDefault(const std::string& fallback);
		};

		void beginSymbolDefinition();
		Symbol& getCurrentSymbol();

		int64_t parseAsNumber(const std::string& number);
		void nextOptionFlag(const std::string& flg);
		void nextSymbolFlag(const std::string& flg);

	private:

		InputMode mode = InputMode::OPTIONS;
		InputFlag expect = InputFlag::NONE;

		static void raiseError(const std::string& message);
		void nextFlag(std::string flg);
		bool accept(InputFlag flag);

		void parseArgument(const char* arg);

	public:

		static Args load(int argc, char** argv);

		std::vector<Symbol> symbols;

		StringFlag store;
		StringFlag header;
		StringFlag elf64;
		StringFlag coff32;

		bool writeable = false;
		bool help = false;

};
