#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <ctime>
#include <string>
#include <cstdlib>
#include <random>
#include <thread>
#include <cstdio>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include "board.hpp"
#include "move.hpp"
#include "movelist.hpp"
#include "constants.hpp"
#include "killer.hpp"
#include "hash.hpp"
#include "debute.hpp"

class Game {
private:
	Board game_board;
	MoveList move_list[32][BOARD_SIZE][BOARD_SIZE];
	void moveListGenerator();
	std::vector<Move> generatePositionMoves(Board b, bool & shah, bool withCastling, int depth);
	unsigned long long movesCounter = 0;
	double evalute(Board b);

	const int WHITE_WIN = 100000000;
	const int BLACK_WIN = -100000000;

	int max_depth;

	uint64_t cells[BOARD_SIZE][BOARD_SIZE];
	uint64_t bitboard[32][BOARD_SIZE][BOARD_SIZE];
	uint64_t zobrist[32][BOARD_SIZE][BOARD_SIZE];

	void initEngine();
	bool uciHandler(std::string str);
	void go();
	double whiteUp = BLACK_WIN, blackUp = WHITE_WIN;
	std::vector<Move> pv_best;
public:
	Game();
	double minimax_white(Board b, double alpha, double beta, int depth, int max_depth, int real_depth, std::vector<uint64_t> hash, bool basis, std::vector<Move>pv, bool usedNullMove);
	double minimax_black(Board b, double alpha, double beta, int depth, int max_depth, int real_depth, std::vector<uint64_t> hash, bool basis, std::vector<Move>pv, bool usedNullMove);

  double quies(Board & b, double alpha, double beta);

	int startGame();
	std::vector<std::string> getStringArray(std::string str);
	bool move(std::string mv);
	void printBitBoard(uint64_t bit_board);
	void cache_gen();
	void zobristGenerate();
	bool shahIsNot(Board & b, uint8_t color);
	uint64_t getHash(Board & b);
	uint64_t getColorHash(Board & b);

	std::vector<uint64_t>gameHash;
	std::vector<Hash>boardHash;
	uint64_t hash_cutter;

	std::vector<std::string>variant;
	std::vector<Killer>whiteKiller;
	std::vector<Killer>blackKiller;

	void printVariant();

	//flags
	bool nullMoveEnable = true;
	bool hashEnable = true;
	bool print_variant_enable = false;
	bool debute = false;

	double getPriceCell(Board & b, int y, int x);

	double evalute_cells_size[BOARD_SIZE][BOARD_SIZE][BOARD_SIZE][BOARD_SIZE];
};

#endif
