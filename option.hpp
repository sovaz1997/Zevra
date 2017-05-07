#ifndef OPTION_HPP
#define OPTION_HPP

#include <iostream>

class Option {
public:
	bool nullMovePrunningEnable;
	bool lmrEnable;
	bool razoring;
	bool futility_prunning;
	bool checkExtensions;
public:
	Option();
	void print();
};

#endif
