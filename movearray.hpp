#ifndef MOVEARRAY_HPP
#define MOVEARRAY_HPP

#include <vector>
#include "bitmove.hpp"

class BitMove;

class MoveArray {
private:
	const int MAX_SIZE = 288;
public:
	std::vector<BitMove> moveArray;
	MoveArray();
	~MoveArray();
	unsigned int count, num_attacks;
	void clear();
	void addMove(BitMove mv);
};

#endif
