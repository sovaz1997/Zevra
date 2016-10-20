#include "bitboard.hpp"

BitBoard::BitBoard() : moveNumber(0), ruleNumber(0) {
	preInit();
	clear();
}

BitBoard::~BitBoard() {

}

void BitBoard::setFen(std::string fen) {
	for(int i = 0; i < fen.size() && )
}

std::string BitBoard::getFen() {

}

void BitBoard::clear() {
	for(unsigned int i = 0; i < 7; ++i) {
		figures[i] = 0;
	}

	white_bit_mask = 0;
	black_bit_mask = 0;
	moveNumber = 0;
	ruleNumber = 0;

	wsc = false;
	wlc = false;
	bsc = false;
	passant_cell = false;

}

void BitBoard::preInit() {

}
