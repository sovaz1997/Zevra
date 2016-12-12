#ifndef EXTENDEDMOVE_HPP
#define EXTENDEDMOVE_HPP

#include <cstdint>
#include "bitmove.hpp"

class ExtendedMove {
public:
	ExtendedMove();
	ExtendedMove(BitMove mv, int64_t eval);

	BitMove move;
	int64_t evalute;
	
	bool operator<(ExtendedMove& right);
	bool operator>(ExtendedMove& right);

};

#endif
