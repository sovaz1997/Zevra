#ifndef KILLER_HPP
#define KILLER_HPP

#include "bitmove.hpp"

class Killer {
public:
	BitMove move;
	bool enable;

	Killer(BitMove mv);
	Killer();
};

#endif
