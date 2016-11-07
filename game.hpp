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
#include <stack>
#include <omp.h>
#include "bitboard.hpp"
#include "constants.hpp"
#include "killer.hpp"
#include "hash.hpp"
#include "point.hpp"
#include "timer.hpp"
#include "movearray.hpp"

class Game {
private:
	BitBoard game_board;
	unsigned long long nodesCounter = 0;
	double evalute(BitBoard & b);

	const int WHITE_WIN = 100000000;
	const int BLACK_WIN = -100000000;

	int max_depth;

	void initEngine();
	bool uciHandler(std::string str);
	void goFixedDepth();
	void goFixedTime(int time);
	void goTournament();
	void clearCash();
	double whiteUp = BLACK_WIN, blackUp = WHITE_WIN;

	bool inZugzwang(BitBoard & b, uint8_t color);

	double whiteHistorySort[BOARD_SIZE][BOARD_SIZE][BOARD_SIZE][BOARD_SIZE];
	double blackHistorySort[BOARD_SIZE][BOARD_SIZE][BOARD_SIZE][BOARD_SIZE];

	bool hasBestMove = false;

	int popcount64(uint64_t value);

	std::stack<BitMove>pv_line;
	int stress;
	BitMove bestMove;
	double bestScore;
	
	//фиксированное время
	Timer timer;
	int time;
	double start_timer, end_timer;
	
	//турнирный режим
	
	double wtime, btime, winc, binc;
public:
	Game();
	double negamax(BitBoard & b, double alpha, double beta, int depth, int real_depth, int rule, bool inNullMove);
	uint64_t perft(int depth);
	bool insufficientMaterial(std::vector<uint64_t>figureMask);
  	double quies(BitBoard & b, double alpha, double beta, int rule, int real_depth);
	int startGame();
	void printScore(double score);
	std::vector<std::string> getStringArray(std::string str);
	bool move(std::string mv);
	void printBitBitBoard(uint64_t bit_BitBoard);

	std::vector<uint64_t>gameHash;
	std::vector<Hash>boardHash;
	uint64_t hash_cutter;

	std::vector<std::string>variant;
	std::vector<Killer>whiteKiller;
	std::vector<Killer>blackKiller;

	std::vector<MoveArray> moveArray;

	void sortAttacks(MoveArray& moves);

	void printVariant();

	//flags - begin
	bool nullMoveEnable = false;
	bool hashEnable = true;
	bool print_variant_enable = false;
	//flags - end

	double evalute_cells_size[BOARD_SIZE][BOARD_SIZE][BOARD_SIZE][BOARD_SIZE];
	void cleanWhiteHistory();
	void cleanBlackHistory();
	void flattenHistory();

	BitMove bestmove;

	double margin = PAWN_EV / 2;
	void sortMoves(MoveArray& result, int depth);
};

#endif
