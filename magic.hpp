#ifndef MAGIC_HPP
#define MAGIC_HPP

#include <iostream>
#include <vector>
#include <cstdint>

class Magic {
public:
	std::vector<uint64_t> result;

	Magic();
	Magic(std::vector<uint64_t> _result, uint64_t _magic, uint8_t _count);
	
	uint64_t getPossibleMoves(uint64_t mask);

private:
	uint64_t magic;
	uint8_t count;
};

#endif
