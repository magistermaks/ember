#pragma once

#include <string>

class Args {

	private:

		enum InputFlag {
			NONE,
			ELF_64_PATH,
			COFF_32_PATH,
			HEADER_PATH,
		};

		struct StringFlag {
			bool enabled;
			std::string value;

			StringFlag();
			void setValue(const std::string& path);
		};

	private:

		InputFlag expect = InputFlag::NONE;

		static void raiseError(const std::string& message);
		void nextFlag(std::string flg);
		bool accept(InputFlag flag);

	public:

		static Args load(int argc, char** argv);

		StringFlag header;
		StringFlag elf64;
		StringFlag coff32;

		bool help = false;

};
