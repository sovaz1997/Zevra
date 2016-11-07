#include "magic.hpp"


Magic::Magic() {}
Magic::Magic(std::vector<uint64_t> _result, uint64_t _magic, uint8_t _count) : result(_result), magic(_magic), count(_count) {}

uint64_t Magic::getPossibleMoves(uint64_t mask) {
	return (result[(mask * magic) >> count]);
}
