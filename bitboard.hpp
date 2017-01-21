#ifndef BITBOARD_HPP
#define BITBOARD_HPP

#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <stack>
#include <cmath>
#include <algorithm>
#include <random>
#include <set>
#include <deque>
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

	uint64_t ROOK_MAGIC[8][8];
	uint64_t BISHOP_MAGIC[8][8];

	uint64_t zobrist[32][BOARD_SIZE][BOARD_SIZE];
	uint64_t hash;

	uint8_t moveNumber, ruleNumber, passant_x, passant_y;
	void preInit();
	std::vector<std::string> splitter(std::string str, char sym);

	uint64_t plus1[65], plus7[65], plus8[65], plus9[65], minus1[65], minus7[65], minus8[65], minus9[65];
	uint64_t vec2_cells[8][8];
	uint64_t vec1_cells[64];
	uint64_t bitboard[32][8][8];
	double kingSecurityArray[64][64];
	int64_t pawnStructureEvalute();

	GoBack newHistory;

	void zobristGenerator();
	uint8_t popcount64(uint64_t value);
	uint8_t firstOne(uint64_t mask);
	uint8_t lastOne(uint64_t mask);
	void clearCell(uint8_t y, uint8_t x);
	void addFigure(uint8_t figure, uint8_t y, uint8_t x);
	void fastClearCell(uint8_t y, uint8_t x);
	void fastAddFigure(uint8_t figure, uint8_t y, uint8_t x);
	void printBitBoard(uint64_t bit_board);
	void magicNumberGenerator();
	void magicInit();
	void magicConstantsSet();

	uint64_t magicGenerator();

	std::deque<GoBack> history;

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
	void fastMove(BitMove& mv);
	void goBack();
	void pushHistory();
	BitMove getRandomMove();
	void makeNullMove();
	void unMakeNullMove();

	bool whiteMove, passant_enable;

	void evaluteAll();
	int64_t evalute;
	int64_t getEvalute();
	void totalStaticEvalute();

	uint8_t getFigure(uint8_t y, uint8_t x);

	int getFiguresCount();

	bool inCheck(uint8_t color);
	bool inCheck(uint8_t color, uint8_t y, uint8_t x);

	void generateHash();
	uint64_t getHash();
	uint64_t getColorHash();

	bool attacked;
	double margin;

	bool testOfDraw();

	int64_t kingSecurity();

	std::multiset<uint64_t> gameHash;

	bool whitePassantMade, blackPassantMade;
};

#endif
