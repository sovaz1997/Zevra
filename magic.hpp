#ifndef MAGIC_HPP
#define MAGIC_HPP

#include <vector>
#include <cstdint>

class Magic {
	uint64_t magic;
public:
	std::vector<uint64_t> combination;
	std::vector<uint64_t> result;

	Magic();
	Magic(std::vector<uint64_t> _combination, std::vector<uint64_t> _result, uint64_t _magic);
};

#endif
