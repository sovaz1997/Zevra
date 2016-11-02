#ifndef BITBOARD_HPP
#define BITBOARD_HPP

#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>
#include <random>
#include "constants.hpp"
#include "movearray.hpp"
#include "goback.hpp"
#include "magic.hpp"

class BitBoard {
private:
	uint64_t figures[7];
	uint64_t horizontal[8];
	uint64_t vertical[8];
	uint64_t white_bit_mask, black_bit_mask;
	uint64_t castlingMap;

	uint64_t zobrist[32][BOARD_SIZE][BOARD_SIZE];
	uint64_t hash;

	uint8_t moveNumber, ruleNumber, passant_x, passant_y;
	void preInit();
	std::vector<std::string> splitter(std::string str, char sym);

	uint64_t plus1[65], plus7[65], plus8[65], plus9[65], minus1[65], minus7[65], minus8[65], minus9[65];
	uint64_t vec2_cells[8][8];
	uint64_t vec1_cells[64];
	uint64_t bitboard[32][8][8];

	void zobristGenerator();
	uint8_t popcount64(uint64_t value);
	uint8_t firstOne(uint64_t mask);
	uint8_t lastOne(uint64_t mask);
	void clearCell(uint8_t y, uint8_t x);
	void addFigure(uint8_t figure, uint8_t y, uint8_t x) ;
	void printBitBoard(uint64_t bit_board);
	void magicNumberGenerator();
	void magicInit();
	
	uint64_t magicGenerator();

	std::stack<GoBack> history;

	bool wsc();
	bool wlc();
	bool bsc();
	bool blc();
	
	Magic rookMagic[8][8];
	Magic bishopMagic[8][8];
	
	uint64_t rookMagicMask[8][8];
	uint64_t bishopMagicMask[8][8];
public:
	BitBoard();
	~BitBoard();

	void setFen(std::string fen);
	std::string getFen();
	void clear();
	int stress;

	void bitBoardMoveGenerator(MoveArray& moveArray);
	void bitBoardAttackMoveGenerator(MoveArray& moveArray);
	void move(BitMove& mv);
	void goBack();
	void pushHistory();

	bool whiteMove, passant_enable;

	void evaluteAll();
	double evalute;
	double getEvalute();
	void totalStaticEvalute();

	uint8_t getFigure(uint8_t y, uint8_t x);

	bool inCheck(uint8_t color);
	bool inCheck(uint8_t color, uint8_t y, uint8_t x);

	void generateHash();
	uint64_t getHash();
	uint64_t getColorHash();
};

#endif
