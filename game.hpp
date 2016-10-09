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
#include <omp.h>
#include "board.hpp"
#include "move.hpp"
#include "movelist.hpp"
#include "constants.hpp"
#include "killer.hpp"
#include "hash.hpp"
#include "debute.hpp"
#include "point.hpp"
#include "timer.hpp"

class Game {
private:
	Board game_board;
	MoveList move_list[32][BOARD_SIZE][BOARD_SIZE];
	void moveListGenerator();
	std::vector<Move> generatePositionMoves(Board & b, bool & shah, bool withCastling, int depth);
	unsigned long long movesCounter = 0;
	double evalute(Board & b);

	const int WHITE_WIN = 100000000;
	const int BLACK_WIN = -100000000;

	int max_depth;

	uint64_t cells[BOARD_SIZE][BOARD_SIZE];
	uint64_t bitboard[32][BOARD_SIZE][BOARD_SIZE];
	uint64_t zobrist[32][BOARD_SIZE][BOARD_SIZE];

	void initEngine();
	bool uciHandler(std::string str);
	void go();
	void goFixedDepth();
	void goFixedTime(int time);
	void clearCash();
	double whiteUp = BLACK_WIN, blackUp = WHITE_WIN;
	std::vector<Move> pv_best;
	std::vector<Move> pv_tmp;

	bool inZugzwang(Board & b, uint8_t color);

	double whiteHistorySort[BOARD_SIZE][BOARD_SIZE][BOARD_SIZE][BOARD_SIZE];
	double blackHistorySort[BOARD_SIZE][BOARD_SIZE][BOARD_SIZE][BOARD_SIZE];

	bool hasBestMove = false;

	int popcount64(uint64_t value);

	//фиксированное время
	Timer timer;
	Move bestMove;
	int time;
public:
	Game();
	double minimax_white(Board b, double alpha, double beta, int depth, int real_depth, std::vector<uint64_t> hash, bool basis, std::vector<Move>pv, bool usedNullMove, int rule);
	double minimax_black(Board b, double alpha, double beta, int depth, int real_depth, std::vector<uint64_t> hash, bool basis, std::vector<Move>pv, bool usedNullMove, int rule);
	double negamax(Board & b, double alpha, double beta, int depth, int real_depth, std::vector<uint64_t> hash, bool basis, std::vector<Move>pv, bool usedNullMove, int rule, bool capture);
	bool insufficientMaterial(std::vector<uint64_t>figureMask);

  double quies(Board & b, double alpha, double beta, int rule);

	int startGame();
	std::vector<std::string> getStringArray(std::string str);
	bool move(std::string mv);
	void printBitBoard(uint64_t bit_board);
	void cache_gen();
	void zobristGenerate();
	uint64_t getHash(Board & b);
	uint64_t getColorHash(Board & b);
	uint64_t getColorHash(Board & b, uint64_t hash);

	std::vector<uint64_t>gameHash;
	std::vector<Hash>boardHash;
	uint64_t hash_cutter;

	std::vector<std::string>variant;
	std::vector<Killer>whiteKiller;
	std::vector<Killer>blackKiller;

	void printVariant();

	//flags - begin
	bool nullMoveEnable = false;
	bool hashEnable = true;
	bool print_variant_enable = false;
	//flags - end

	bool inCheck(Board & b, uint8_t color);
	bool inCheck(Board b, uint8_t color, int yPos, int xPos);
	double getPriceCell(Board & b, int y, int x);
	double evalute_cells_size[BOARD_SIZE][BOARD_SIZE][BOARD_SIZE][BOARD_SIZE];
	void cleanWhiteHistory();
	void cleanBlackHistory();
	void flattenHistory();

	uint64_t pawnAttackCutter[BOARD_SIZE];
	void createPawnAttackCutter();

	double getForcastEvalute(Board & b);
	double getFigureEval(Board & b, int y, int x);

	bool isEndGame(Board& b);

	Move bestmove;
	//uint8_t getFigureAttacks(Board & b, uint8_t color);

	double margin = PAWN_EV / 2;
};

#endif
