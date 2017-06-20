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
public:
	uint64_t figures[7];
	uint64_t horizontal[8];
	uint64_t vertical[8];
	uint64_t white_bit_mask, black_bit_mask;
	uint64_t castlingMap;
	uint64_t recapture_position;

	int hash_width;
	uint64_t hash_cutter;

	uint64_t ROOK_MAGIC[8][8];
	uint64_t BISHOP_MAGIC[8][8];

	uint64_t zobrist[32][BOARD_SIZE][BOARD_SIZE];

	uint8_t moveNumber, ruleNumber, passant_x, passant_y;
	void preInit();
	std::vector<std::string> splitter(std::string str, char sym);

	uint64_t plus1[65], plus7[65], plus8[65], plus9[65], minus1[65], minus7[65], minus8[65], minus9[65];
	uint64_t vec2_cells[8][8];
	uint64_t vec1_cells[64];
	uint64_t bitboard[32][8][8];
	double kingSecurityArray[64][64];
	std::string startpos_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	int64_t pawnStructureEvalute();

	void zobristGenerator();
	uint8_t popcount64(uint64_t value);
	uint8_t firstOne(uint64_t mask);
	uint8_t lastOne(uint64_t mask);
	void clearCell(uint8_t y, uint8_t x);
	void addFigure(uint8_t figure, uint8_t y, uint8_t x) ;
	void printBitBoard(uint64_t bit_board);
	void magicNumberGenerator();
	void magicInit();
	void magicConstantsSet();

	uint64_t magicGenerator();

	BitMove getMove(uint8_t fromY, uint8_t fromX, uint8_t toY, uint8_t toX, bool replaced, uint8_t replacedFigure, bool& enable);

	uint8_t metric[64][8][8];

	std::vector<GoBack> history;
	int history_iterator = 0;

	bool wsc();
	bool wlc();
	bool bsc();
	bool blc();

	Magic rookMagic[8][8];
	Magic bishopMagic[8][8];

	uint64_t rookMagicMask[8][8];
	uint64_t bishopMagicMask[8][8];

	std::vector<int> third_repeat;

	/*--- evalution fields ---*/
	/*--- evalution fields ---*/
public:
	BitBoard();
	~BitBoard();

	void setFen(std::string fen);
	std::string getFen();
	void clear();
	int64_t stress;

	void bitBoardMoveGenerator(MoveArray& moveArray);
	void bitBoardAttackMoveGenerator(MoveArray& moveArray);
	void move(BitMove& mv);
	void goBack();
	void pushHistory();
	BitMove getRandomMove();
	void makeNullMove();
	void unMakeNullMove();

	bool whiteMove, passant_enable;

	void evaluteAll();
	int64_t evalute;
	int64_t kingEvalute();
	int64_t getEvalute();
	int64_t getLazyEvalute();
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

	bool hash_enable = true;
	uint64_t hash;
	bool whitePassantMade, blackPassantMade;

	/*--- evalution functions ---*/

	std::vector<int>basic_king_figures_rom;

	int attackWeight[8] = {0, 0, 50, 75, 88, 94, 97, 99};

	int SafetyTable[100] = {
    0,  0,   1,   2,   3,   5,   7,   9,  12,  15,
	18,  22,  26,  30,  35,  39,  44,  50,  56,  62,
	68,  75,  82,  85,  89,  97, 105, 113, 122, 131,
	140, 150, 169, 180, 191, 202, 213, 225, 237, 248,
	260, 272, 283, 295, 307, 319, 330, 342, 354, 366,
	377, 389, 401, 412, 424, 436, 448, 459, 471, 483,
	494, 500, 500, 500, 500, 500, 500, 500, 500, 500,
	500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
	500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
	500, 500, 500, 500, 500, 500, 500, 500, 500, 500
	};

	int promotePawnBonus[8] = {0, 10, 20, 30, 40, 55, 70, 0};
	
	double newEvaluteAll();
	double basicKingSafety();
	void attackedField(uint8_t color, uint8_t y, uint8_t x, std::vector<int>& figure_array, uint64_t& pieces_mask);

	MoveArray moveArray;

	/*--- evalution functions ---*/
};

#endif
