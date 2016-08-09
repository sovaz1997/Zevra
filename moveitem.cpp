#include "moveitem.hpp"

MoveItem::MoveItem() {
	init();
}

MoveItem::MoveItem(Move mv) {
	move = mv;
	init();
}

void MoveItem::init() {
	next = NULL;
	specialNext = NULL;
}
