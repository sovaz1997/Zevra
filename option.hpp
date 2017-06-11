#ifndef OPTION_HPP
#define OPTION_HPP

#include <iostream>

class Option {
public:
	bool nullMovePruningEnable;
	bool razoring;
	bool futility_pruning;
	bool checkExtensions;
public:
	Option();
	void print();
};

#endif
