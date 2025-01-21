#pragma once

#include <string>

enum struct InputFlag {
	NONE,
	OUTPUT
};

class Args {

	private:

		InputFlag flag = InputFlag::NONE;

		static void raiseError(const std::string& message);
		void nextFlag(char flg);

	public:

		static Args load(int argc, char** argv);

		std::string file = "out";

		bool h = false; // help
		bool c = false; // .c header

};
