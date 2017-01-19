#ifndef HASH_HPP
#define HASH_HPP

#include <cstdint>
#include <stack>
#include "bitmove.hpp"
#include "pv.hpp"

class BitMove;

class Hash {
public:
	std::stack<BitMove>* prev;

	uint8_t depth;
	int64_t score;
	uint8_t flag;
	uint64_t key;
	BitMove move;

	Hash();
	~Hash();
	bool back();
	void recordPrev();
	void clean();
};

#endif
