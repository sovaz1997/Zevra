#include "option.hpp"

Option::Option() : nullMovePruningEnable(false),
				   razoring(true),
				   futility_pruning(true),
				   checkExtensions(true),
				   UCI_AnalyseMode(false)
				   {}

void Option::print() {
	std::cout << "option name nullmove type check default false\n";
	std::cout << "option name razoring type check default true\n";
	std::cout << "option name futility_pruning type check default true\n";
	std::cout << "option name checkExtensions type check default true\n";
	std::cout << "option name Clear Hash type button\n";
	std::cout << "option name Hash type spin default 256 min " << min_hash_size << " max "<<  max_hash_size << "\n";
	std::cout << "option name UCI_AnalyseMode type check default false\n";
}
