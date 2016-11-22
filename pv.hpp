#ifndef PV_HPP
#define PV_HPP

#include "bitmove.hpp"

class PV {
public:
	int count;
	BitMove line[100];
	
	PV();
};

#endif
