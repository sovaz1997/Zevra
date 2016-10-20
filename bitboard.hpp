#ifndef BITBOARD_HPP
#define BITBOARD_HPP

#include <cstdint>
#include <string>
#include <vector>

class BitBoard {
private:
	uint64_t figures[7];
	uint64_t white_bit_mask, black_bit_mask;

	uint8_t moveNumber, ruleNumber, passant_cell;

	bool whiteMove, wsc, wlc, bsc, blc, passant_enable;

	void preInit();
	std::string<std::vector> splitter(std::string str);
public:
	BitBoard();
	~BitBoard();

	void setFen(std::string fen);
	std::string getFen();
	void clear();
};

#endif
