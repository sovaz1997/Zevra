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
	int threads_count = 1;
	int min_hash_size = 1, max_hash_size = 16384;
	int min_threads_count = 1, max_threads_count = 64;
	bool UCI_AnalyseMode;
	int threads;
public:
	Option();
	void print();
};

#endif
