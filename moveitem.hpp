#ifndef MOVEITEM_HPP
#define MOVEITEM_HPP

#include "move.hpp"

class MoveItem {
private:
	void init();
public:
	Move move;
	MoveItem* next;
	MoveItem* specialNext;

	MoveItem();
	MoveItem(Move mv);
};

#endif
