#ifndef BOARD_HPP
#define BOARD_HPP

#include <cstdint>
#include <vector>
#include <stack>
#include <string>
#include <iostream>
#include "move.hpp"
#include "constants.hpp"
#include "boardinfo.hpp"
#include "point.hpp"


class Move;

class Board {
private:
	std::string savedFen;

	void setFen(std::string FEN);
	void setCastling(std::string FEN);
	void setPassant(std::string FEN);

	std::stack<BoardInfo> history;
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
	void unMove();

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
	/* ------*/

	void setBoardInfo();
	void getBoardInfo();

	//std::vector<Point>figureList;
};

#endif
