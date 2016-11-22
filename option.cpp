#include "option.hpp"

Option::Option() : nullMovePrunningEnable(false),
									 lmrEnable(false) {
	
}

void Option::print() {
	std::cout << "option name nullmove type check default false\n";
	std::cout << "option name LMR type check default false\n";
}
