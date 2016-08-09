#include "movelist.hpp"

MoveList::MoveList() {
	first = NULL;
}

MoveItem* MoveList::next(MoveItem* it) {
	if(it != NULL) {
		if(it->next != NULL) {
			it = it->next;
		} else {
			it = it->specialNext;
		}
	}

	return it;
}

bool MoveList::hasNext(MoveItem* it) {
	return (it->next != NULL || it->specialNext != NULL);
}

MoveItem* MoveList::add(Move mv) {
	if(first == NULL) {
		first = new MoveItem(mv);
		return first;
	} else {
		MoveItem* it = first;

		while(hasNext(it)) {
			it = next(it);
		}

	MoveItem* new_item = new MoveItem(mv);
	it->next = new_item;
	it->specialNext = new_item;

	return new_item;
	}
}

MoveItem* MoveList::add(Move mv, MoveItem* special) {
	MoveItem* it = add(mv);
	it->specialNext = special;

	return it;
}

void MoveList::print() {
	if(first != NULL) {
		MoveItem* it = first;

		while(hasNext(it)) {
			it->move.print();
			if(it->specialNext != NULL) {
				std::cout << " - ";
				it->specialNext->move.print();
			}
			it = next(it);
			std::cout << "\n";
		}

		if(it != NULL) {
			it->move.print();
			std::cout << "\n";
		}
	} else {
		std::cout << "Ходов нет\n";
	}
}

int MoveList::size() {
	int count = 0;

	if(first != NULL) {
		MoveItem* it = first;
		while(hasNext(it)) {
			++count;
			it = next(it);
		}
		++count;
	}

	return count;
}
