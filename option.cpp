#include "option.hpp"

Option::Option() : UCI_AnalyseMode(false) {}

void Option::print() {
	std::cout << "option name Clear Hash type button\n";
	std::cout << "option name Hash type spin default 256 min " << min_hash_size << " max "<<  max_hash_size << "\n";
	std::cout << "option name UCI_AnalyseMode type check default false\n";
	std::cout << "option name Move Overhead type spin default 30 min " << minMoveOverhead << " max " << maxMoveOverhead << "\n";
}
