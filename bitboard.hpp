#ifndef BITBOARD_HPP
#define BITBOARD_HPP

#include <iostream>
#include <cstdint>
#include <mutex>
#include <string>
#include <vector>
#include <stack>
#include <cmath>
#include <algorithm>
#include <random>
#include <set>
#include <deque>
#include <bitset>
#include "constants.hpp"
#include "movearray.hpp"
#include "goback.hpp"
#include "magic.hpp"

class BitBoard {
public:
	uint64_t horizontal[8];
	uint64_t vertical[8];

	int hash_width;
	uint64_t hash_cutter;

	uint64_t ROOK_MAGIC[8][8];
	uint64_t BISHOP_MAGIC[8][8];

	uint64_t zobrist[32][BOARD_SIZE][BOARD_SIZE];
	uint64_t wscZobrist, wlcZobrist, bscZobrist, blcZobrist;
	uint64_t passantZobrist[8][8];

	void preInit();

	std::vector<std::string> splitter(std::string str, char sym);

	uint64_t plus1[65], plus7[65], plus8[65], plus9[65], minus1[65], minus7[65], minus8[65], minus9[65];
	uint64_t vec2_cells[8][8];
	uint64_t vec1_cells[64];
	uint64_t bitboard[32][8][8];
	double kingSecurityArray[64][64];
	std::string startpos_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

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

	uint64_t capturePawnMap[32][64];

	BitMove getMove(uint8_t fromY, uint8_t fromX, uint8_t toY, uint8_t toX, bool replaced, uint8_t replacedFigure, bool& enable);

	uint8_t metric[64][8][8];

	std::vector<GoBack> history;
	GoBack currentState;
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
public:
	BitBoard();
	~BitBoard();

	void setFen(std::string fen);
	std::string getFen();
	void clear();
	size_t stress;

	void bitBoardMoveGenerator(MoveArray& moveArray, size_t& counter_moves);
	void bitBoardAttackMoveGenerator(MoveArray& moveArray, size_t& counter_moves);
	void move(BitMove& mv);
	void goBack();
	void pushHistory();
	BitMove getRandomMove();
	void makeNullMove();
	void unMakeNullMove();
	int64_t getEvaluate();
	void totalStaticEvaluate();
	uint8_t getFigure(uint8_t y, uint8_t x);
	int getFiguresCount();
	bool inCheck(uint8_t color);
	bool inCheck(uint8_t color, uint8_t y, uint8_t x);
	void generateHash();
	uint64_t getHash();
	uint64_t getColorHash();

	bool testOfDraw();

	std::multiset<uint64_t> gameHash;


	int promotePawnBonus[8] = {0, 10, 20, 30, 40, 60, 80, 0};

	//std::vector<std::vector<std::vectorstd::vector<std::vector<BitMove>>>>> standardMovesStorage; //хранилище всевозможных ходов (+рокировка) [ходящая_фигура][fx][fy][tx][ty]
	//std::vector<std::vector<std::vector<BitMove>>> promoteMovesStorage; //хранилище всевозможных превращений пешки [фигура_превращения][fx][fy][tx][ty]
	//std::vector<std::vector<std::vector<BitMove>>> passantMovesStorage; //хранилище всевозможных взятий на проходе 

	std::vector<uint8_t> isolated_pawn_map;
	
	double newEvaluateAll();

	MoveArray moveArray;

	uint8_t compressVertical(uint64_t value);

	uint64_t whitePawnCheckCells[64];
	uint64_t blackPawnCheckCells[64];
};

#endif
