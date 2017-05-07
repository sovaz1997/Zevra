#include "option.hpp"

Option::Option() : nullMovePrunningEnable(false),
				   lmrEnable(true), 
				   razoring(true),
				   futility_prunning(true),
				   checkExtensions(true)
				   {}

void Option::print() {
	std::cout << "option name nullmove type check default false\n";
	std::cout << "option name LMR type check default true\n";
	std::cout << "option name razoring type check default true\n";
	std::cout << "option name futility_prunning type check default true\n";
	std::cout << "option name checkExtensions type check default true\n";
}
