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
	double evalute;
	int type_mv;

	Hash();
	Hash(uint64_t hsh, Move mv, int dpth, double eval, int type);
};
#endif
