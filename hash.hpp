#ifndef HASH_HPP
#define HASH_HPP

#include <cstdint>
#include "bitmove.hpp"

class BitMove;

class Hash {
public:
	bool enable;
	uint64_t hash;
	BitMove move;
	int depth, ply;
	double evalute, alpha, beta;
	int type_mv;
	double 	staticEval;

	Hash();
	Hash(uint64_t hsh, BitMove mv, int dpth, int pl, double eval, double a, double b, int type, double seval);
};

#endif
