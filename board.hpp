#ifndef BOARD_HPP
#define BOARD_HPP

#include <cstdint>
#include <vector>
#include <stack>
#include <cmath>
#include <string>
#include <iostream>
#include "move.hpp"
#include "constants.hpp"
#include "boardinfo.hpp"
#include "point.hpp"
#include "goback.hpp"
#include "figurecell.hpp"


class Move;
class GoBack;

class Board {
private:
	std::string savedFen;
	void setFen(std::string FEN);
	void setCastling(std::string FEN);
	void setPassant(std::string FEN);

	//std::stack<BoardInfo> history;
	std::stack<GoBack>history;
	double evaluteAll();

	//BitBoard's

	//uint64_t white_occupied_squares, black_occupied_squares;
	uint64_t plus1[64], plus7[64], plus8[64], plus9[64], minus1[64], minus7[64], minus8[64], minus9[64];
	uint64_t vec2_cells[8][8];
	uint64_t vec1_cells[64];
	uint64_t bitboard[32][BOARD_SIZE][BOARD_SIZE];
	uint64_t figures[32];
public:
	Board();
	uint8_t createFigure(uint8_t fig, uint8_t type);
	uint8_t createFigure(char FEN_byte);
	void changeFigureType(uint8_t & fig, uint8_t type);
	void changeFigureColor(uint8_t & fig, uint8_t color);
	void setFenPosition(std::string FEN);
	std::string getFenPosition();
	void putFigureOnBoard(uint8_t figure, int y, int x);
	void printBoard();
	void cleanBoard();
	void printStringBoard();
	void goBack();

	//uint8_t getFigure(int y, int x);
	void setFigure(uint8_t figure, int y, int x);
	void move(Move & mv);
	bool isWhiteMove();

	//* ------ Ситуация на доске ------*/
	bool passant_enable;
	int passant_x, passant_y;
	bool whiteMove = true;

	bool blackShortCastleEnable;
	bool blackLongCastleEnable;
	bool whiteShortCastleEnable;
	bool whiteLongCastleEnable;

	std::vector<std::vector<uint8_t> > board;
	int numHalfMove;
	int move_rule_num;
	void printScore(double score);

	std::vector<Move>bitBoardMoveGenerator();
	void preInitBoard();
	void initBoard();

	//оценочная функция
	double evalute;

	int popcount64(uint64_t value);
	void printBitBoard(uint64_t bit_board);

	uint64_t white_bit_mask, black_bit_mask;
	uint64_t whiteBitMask();
	uint64_t blackBitMask();

	uint8_t firstOne(uint64_t mask);
	uint8_t lastOne(uint64_t mask);
	/* ------*/

	//std::vector<Point>figureList;

	int stress;
};

#endif
