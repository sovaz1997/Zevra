#ifndef HASH_HPP
#define HASH_HPP

#include <cstdint>
#include "bitmove.hpp"

class BitMove;

class Hash {
public:
	bool enable = false;
	uint64_t hash;
	int depth;
	BitMove move;
	double evalute, alpha, beta;
	int type_mv;

	Hash();
	Hash(uint64_t hsh, BitMove mv, int dpth, double eval, double a, double b, int type);
};

#endif
