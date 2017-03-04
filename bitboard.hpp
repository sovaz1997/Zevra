/*
  Zevra, a UCI chess playing engine
  Copyright (C) 2016-2017 Oleg Smirnov (author)
  Zevra is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version.
  Zevra is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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

	uint8_t moveNumber, ruleNumber, passant_x, passant_y;
	void preInit();
	std::vector<std::string> splitter(std::string str, char sym);

	uint64_t plus1[65], plus7[65], plus8[65], plus9[65], minus1[65], minus7[65], minus8[65], minus9[65];
	uint64_t vec2_cells[8][8];
	uint64_t vec1_cells[64];
	uint64_t bitboard[32][8][8];
	double kingSecurityArray[64][64];

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
	int getAllFiguresWeight();
	double getPositionStage();

	uint64_t magicGenerator();

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

	uint8_t distance[8][8][8][8];

	std::vector<int> third_repeat;
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
	int64_t getEvalute();

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

	std::multiset<uint64_t> gameHash;

	bool hash_enable = true;
	bool in_null_move = false;
	uint64_t hash;
	bool whitePassantMade, blackPassantMade;
};

#endif
