#ifndef OPTION_HPP
#define OPTION_HPP

#include <iostream>

class Option {
public:
	int hash_size = 256;
	int moveOverhead = 30, maxMoveOverhead = 10000, minMoveOverhead = 0;
	int min_hash_size = 1, max_hash_size = 16384;
	bool UCI_AnalyseMode;
public:
	Option();
	void print();
};

#endif
