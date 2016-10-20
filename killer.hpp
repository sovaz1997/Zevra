#ifndef KILLER_HPP
#define KILLER_HPP

#include "move.hpp"

class Move;

class Killer {
public:
	Move move;
	bool enable;

	Killer(Move mv);
	Killer();
};

#endif
