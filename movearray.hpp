#ifndef MOVEARRAY_HPP
#define MOVEARRAY_HPP

#include "move.hpp"

class Move;

class MoveArray {
public:
	Move* moveArray;
	MoveArray();
	~MoveArray();
	int count;
	void clear();
	void addMove(Move mv);
};

#endif
