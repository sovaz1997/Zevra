#ifndef OPTION_HPP
#define OPTION_HPP

#include <iostream>

class Option {
public:
	bool nullMovePruningEnable;
	bool razoring;
	bool futility_pruning;
	bool checkExtensions;
	int hash_size = 256;
	int min_hash_size = 1, max_hash_size = 16384;
public:
	Option();
	void print();
};

#endif
