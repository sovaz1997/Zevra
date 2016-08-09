#ifndef MOVELIST_HPP
#define MOVELIST_HPP

#include "moveitem.hpp"

/*
* MoveList - полностью открытый класс, созданный для хранения списков ходов.
* Служит для ускорения перебора в несколько десятков раз.
*/

class MoveList {
public:
	MoveItem* first;
	MoveList();
	MoveItem* add(Move mv);
	MoveItem* add(Move mv, MoveItem* special);
	void print();
	MoveItem* next(MoveItem* it);
	bool hasNext(MoveItem* it);
	int size();
};

#endif
