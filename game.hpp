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
#include "move.hpp"
#include "movelist.hpp"
#include "constants.hpp"
#include "killer.hpp"
#include "hash.hpp"
#include "debute.hpp"
#include "point.hpp"
#include "timer.hpp"
#include "bitboard.hpp"
#include "pv.hpp"
#include "movearray.hpp"

class Game {
private:
	BitBoard game_board;
	MoveList move_list[32][BOARD_SIZE][BOARD_SIZE];
	void moveListGenerator();
	//std::vector<Move> generatePositionMoves(Board & b, bool & shah, bool withCastling, int depth);
	unsigned long long movesCounter = 0;
	double evalute(BitBoard & b);

	PV best_pv;

	const int WHITE_WIN = 100000000;
	const int BLACK_WIN = -100000000;

	int max_depth;

	uint64_t cells[BOARD_SIZE][BOARD_SIZE];
	uint64_t bitboard[32][BOARD_SIZE][BOARD_SIZE];
	uint64_t zobrist[32][BOARD_SIZE][BOARD_SIZE];

	void initEngine();
	bool uciHandler(std::string str);
	void goFixedDepth();
	void goFixedTime(int time);
	void clearCash();
	double whiteUp = BLACK_WIN, blackUp = WHITE_WIN;
	std::vector<Move> pv_best;
	std::vector<Move> pv_tmp;

	bool inZugzwang(BitBoard & b, uint8_t color);

	double whiteHistorySort[BOARD_SIZE][BOARD_SIZE][BOARD_SIZE][BOARD_SIZE];
	double blackHistorySort[BOARD_SIZE][BOARD_SIZE][BOARD_SIZE][BOARD_SIZE];

	bool hasBestMove = false;

	int popcount64(uint64_t value);

	std::stack<BitMove>pv_line;

	//фиксированное время
	Timer timer;
	BitMove bestMove;
	double bestScore;
	int time;
	double start_timer, end_timer;
	int stress;
public:
	Game();
	//PV negamax(BitBoard & b, double alpha, double beta, int depth, int real_depth, std::vector<uint64_t> hash, bool basis, std::vector<Move>pv, bool usedNullMove, int rule, bool capture);
	double negamax(BitBoard & b, double alpha, double beta, int depth, int real_depth, int rule);
	uint64_t perft(int depth);
	bool insufficientMaterial(std::vector<uint64_t>figureMask);
  //double quies(BitBoard & b, double alpha, double beta, int rule, int real_depth);
	int startGame();
	void printScore(double score);
	std::vector<std::string> getStringArray(std::string str);
	bool move(std::string mv);
	void printBitBitBoard(uint64_t bit_BitBoard);
	void cache_gen();
	void zobristGenerate();
	//uint64_t getHash(BitBoard & b);
	//uint64_t getColorHash(BitBoard & b);
	//uint64_t getColorHash(BitBoard & b, uint64_t hash);

	std::vector<uint64_t>gameHash;
	std::vector<Hash>boardHash;
	uint64_t hash_cutter;

	std::vector<std::string>variant;
	std::vector<Killer>whiteKiller;
	std::vector<Killer>blackKiller;

	std::vector<MoveArray> moveArray;

	void printVariant();

	//flags - begin
	bool nullMoveEnable = false;
	bool hashEnable = true;
	bool print_variant_enable = false;
	//flags - end

	//bool inCheck(BitBoard & b, uint8_t color);
	//bool inCheck(BitBoard b, uint8_t color, int yPos, int xPos);
	//double getPriceCell(BitBoard & b, int y, int x);
	double evalute_cells_size[BOARD_SIZE][BOARD_SIZE][BOARD_SIZE][BOARD_SIZE];
	void cleanWhiteHistory();
	void cleanBlackHistory();
	void flattenHistory();

	uint64_t pawnAttackCutter[BOARD_SIZE];
	void createPawnAttackCutter();

	//double getForcastEvalute(BitBoard & b);
	//double getFigureEval(BitBoard & b, int y, int x);

	//bool isEndGame(BitBoard& b);

	BitMove bestmove;
	//uint8_t getFigureAttacks(BitBoard & b, uint8_t color);

	double margin = PAWN_EV / 2;

	void sortMoves(std::vector<Move>& result, int depth);
};

#endif
