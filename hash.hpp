#ifndef HASH_HPP
#define HASH_HPP

#include <cstdint>
#include "move.hpp"

class Hash {
public:
	bool enable = false;
	uint64_t hash;
	int depth;
	Move move;
	double evalute, alpha, beta;
	int type_mv;

	Hash();
	Hash(uint64_t hsh, Move mv, int dpth, double eval, double a, double b, int type);
};
#endif
