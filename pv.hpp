#ifndef PV_HPP
#define PV_HPP

#include "bitmove.hpp"

class PV {
public:
	int cmove;
	BitMove argmove[100];
	PV();
};

#endif
