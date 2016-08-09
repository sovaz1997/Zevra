#ifndef DEBUTE_HPP
#define DEBUTE_HPP

#include <cstdint>
#include "move.hpp"

class Debute {
public:
	uint64_t hash;
	Move move;

	Debute();
	Debute(uint64_t hsh, Move mv);

	bool operator>(Debute& deb);
	bool operator<(Debute& deb);
};

#endif
