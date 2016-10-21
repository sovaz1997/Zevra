#ifndef BITBOARD_HPP
#define BITBOARD_HPP

#include <cstdint>
#include <string>
#include <vector>
#include "constants.hpp"

class BitBoard {
private:
	uint64_t figures[7];
	uint64_t white_bit_mask, black_bit_mask;

	uint8_t moveNumber, ruleNumber, passant_cell;

	bool whiteMove, wsc, wlc, bsc, blc, passant_enable;

	void preInit();
	std::vector<std::string> splitter(std::string str);

	uint64_t plus1[64], plus7[64], plus8[64], plus9[64], minus1[64], minus7[64], minus8[64], minus9[64];
	uint64_t vec2_cells[8][8];
	uint64_t vec1_cells[64];
	uint64_t bitboard[32][8][8];

	uint8_t popcount64(uint64_t value);
	uint8_t firstOne(uint64_t mask);
	uint8_t lastOne(uint64_t mask);
public:
	BitBoard();
	~BitBoard();

	void setFen(std::string fen);
	std::string getFen();
	void clear();
};

#endif
