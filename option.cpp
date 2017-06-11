#include "option.hpp"

Option::Option() : nullMovePruningEnable(false),
				   razoring(true),
				   futility_pruning(true),
				   checkExtensions(true)
				   {}

void Option::print() {
	std::cout << "option name nullmove type check default false\n";
	std::cout << "option name razoring type check default true\n";
	std::cout << "option name futility_pruning type check default true\n";
	std::cout << "option name checkExtensions type check default true\n";
}
