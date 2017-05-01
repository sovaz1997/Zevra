#ifndef OPTION_HPP
#define OPTION_HPP

#include <iostream>

class Option {
public:
	bool nullMovePrunningEnable;
	bool lmrEnable;
public:
	Option();
	void print();
};

#endif
