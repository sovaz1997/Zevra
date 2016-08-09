#ifndef KILLER_HPP
#define KILLER_HPP

#include "move.hpp"

class Killer {
public:
	Move move;
	bool enable;

	Killer(Move mv);
	Killer();
};

#endif
