#include "bitboard.hpp"

BitBoard::BitBoard() : moveNumber(0), ruleNumber(0) {
	preInit();
	clear();
}

BitBoard::~BitBoard() {}

void BitBoard::setFen(std::string fen) {
	clear();

	std::vector<std::string> fenArray = splitter(fen, ' ');
	std::vector<std::string> notation = splitter(fenArray[0], '/');

	std::reverse(notation.begin(), notation.end());

	for(unsigned int y = 0; y < 8; ++y) {
		int x = 0;
		for(unsigned int i = 0; i < notation[y].size(); ++i) {
			if(notation[y][i] == 'K') {
				figures[KING] |= vec2_cells[y][x];
				white_bit_mask |= vec2_cells[y][x];
				++x;
			} else if(notation[y][i] == 'Q') {
				figures[QUEEN] |= vec2_cells[y][x];
				white_bit_mask |= vec2_cells[y][x];
				++x;
			} else if(notation[y][i] == 'R') {
				figures[ROOK] |= vec2_cells[y][x];
				white_bit_mask |= vec2_cells[y][x];
				++x;
			} else if(notation[y][i] == 'B') {
				figures[BISHOP] |= vec2_cells[y][x];
				white_bit_mask |= vec2_cells[y][x];
				++x;
			} else if(notation[y][i] == 'N') {
				figures[KNIGHT] |= vec2_cells[y][x];
				white_bit_mask |= vec2_cells[y][x];
				++x;
			} else if(notation[y][i] == 'P') {
				figures[PAWN] |= vec2_cells[y][x];
				white_bit_mask |= vec2_cells[y][x];
				++x;
			} else if(notation[y][i] == 'k') {
				figures[KING] |= vec2_cells[y][x];
				black_bit_mask |= vec2_cells[y][x];
				++x;
			} else if(notation[y][i] == 'q') {
				figures[QUEEN] |= vec2_cells[y][x];
				black_bit_mask |= vec2_cells[y][x];
				++x;
			} else if(notation[y][i] == 'r') {
				figures[ROOK] |= vec2_cells[y][x];
				black_bit_mask |= vec2_cells[y][x];
				++x;
			} else if(notation[y][i] == 'b') {
				figures[BISHOP] |= vec2_cells[y][x];
				black_bit_mask |= vec2_cells[y][x];
				++x;
			} else if(notation[y][i] == 'n') {
				figures[KNIGHT] |= vec2_cells[y][x];
				black_bit_mask |= vec2_cells[y][x];
				++x;
			} else if(notation[y][i] == 'p') {
				figures[PAWN] |= vec2_cells[y][x];
				black_bit_mask |= vec2_cells[y][x];
				++x;
			} else if(notation[y][i] >= '0' && notation[y][i] <= '9') {
				x += (notation[y][i] - '0');
			}
		}
	}

	if(fenArray[1] == "w") {
		whiteMove = true;
	} else {
		whiteMove = false;
	}

	for(unsigned int i = 0; i < fenArray[2].size() && fenArray[2] != "-"; ++i) {
		if(fenArray[2][i] == 'K') {
			castlingMap |= (vec2_cells[0][4] | vec2_cells[0][7]);
		} else if(fenArray[2][i] == 'Q') {
			castlingMap |= (vec2_cells[0][4] | vec2_cells[0][0]);
		} else if(fenArray[2][i] == 'k') {
			castlingMap |= (vec2_cells[7][4] | vec2_cells[7][7]);
		} else if(fenArray[2][i] == 'q') {
			castlingMap |= (vec2_cells[7][4] | vec2_cells[7][0]);
		}
	}

	if(fenArray[3] != "-") {
		uint8_t x = (fenArray[3][0] - 'a');
		uint8_t y = (fenArray[3][1] - '1');

		passant_enable = true;
		passant_x = x;
		passant_y = y;
	}

	ruleNumber = std::stoi(fenArray[4]);
	moveNumber = std::stoi(fenArray[5]);

	generateHash();

	evaluteAll();
}

std::string BitBoard::getFen() {
	std::string res;

	for(int y = 7; y >= 0; --y) {
		int space = 0;
		for(int x = 0; x < 8; ++x) {
			if(white_bit_mask & vec2_cells[y][x]) {
				if(space != 0) {
					res.push_back('0' + space);
					space = 0;
				}

				if(figures[KING] & vec2_cells[y][x]) {
					res.push_back('K');
				} else if(figures[QUEEN] & vec2_cells[y][x]) {
					res.push_back('Q');
				} else if(figures[ROOK] & vec2_cells[y][x]) {
					res.push_back('R');
				} else if(figures[BISHOP] & vec2_cells[y][x]) {
					res.push_back('B');
				} else if(figures[KNIGHT] & vec2_cells[y][x]) {
					res.push_back('N');
				} else if(figures[PAWN] & vec2_cells[y][x]) {
					res.push_back('P');
				}
			} else if(black_bit_mask & vec2_cells[y][x]) {
				if(space != 0) {
					res.push_back('0' + space);
					space = 0;
				}

				if(figures[KING] & vec2_cells[y][x]) {
					res.push_back('k');
				} else if(figures[QUEEN] & vec2_cells[y][x]) {
					res.push_back('q');
				} else if(figures[ROOK] & vec2_cells[y][x]) {
					res.push_back('r');
				} else if(figures[BISHOP] & vec2_cells[y][x]) {
					res.push_back('b');
				} else if(figures[KNIGHT] & vec2_cells[y][x]) {
					res.push_back('n');
				} else if(figures[PAWN] & vec2_cells[y][x]) {
					res.push_back('p');
				}
			} else {
				++space;
			}
		}

		if(space > 0) {
			res.push_back('0' + space);
		}

		if(y > 0) {
			res.push_back('/');
		}
	}

	res.push_back(' ');

	if(whiteMove) {
		res.push_back('w');
	} else {
		res.push_back('b');
	}

	res.push_back(' ');

	if(!wsc() && !wlc() && !bsc() && !blc()) {
		res.push_back('-');
	} else {
		if(wsc()) {
			res.push_back('K');
		}
		if(wlc()) {
			res.push_back('Q');
		}
		if(bsc()) {
			res.push_back('k');
		}
		if(blc()) {
			res.push_back('q');
		}
	}

	res.push_back(' ');

	if(passant_enable) {
		res.push_back(passant_x + 'a');
		res.push_back(passant_y + '1');
	} else {
		res.push_back('-');
	}


	res.push_back(' ');
	res += std::to_string(ruleNumber);
	res.push_back(' ');
	res += std::to_string(moveNumber);

	return res;
}

void BitBoard::clear() {
	hash = 0;

	for(unsigned int i = 0; i < 7; ++i) {
		figures[i] = 0;
	}

	white_bit_mask = 0;
	black_bit_mask = 0;
	moveNumber = 0;
	ruleNumber = 0;
	passant_enable = false;
	whiteMove = true;
	castlingMap = 0;
}

std::vector<std::string> BitBoard::splitter(std::string str, char sym) {
	std::vector<std::string> result;
	std::string tmp;
	for(unsigned int i = 0; i < str.size(); ++i) {
		if(str[i] == sym) {
			result.push_back(tmp);
			tmp.clear();
			continue;
		}

		tmp.push_back(str[i]);
	}

	result.push_back(tmp);

	return result;
}

uint8_t BitBoard::popcount64(uint64_t value) {
	return /*_mm_popcnt_u64(value);*/ __builtin_popcountll(value);
}

uint8_t BitBoard::firstOne(uint64_t mask) {
	if(!mask) { return 64; }
	return __builtin_ctzll(mask);
}

uint8_t BitBoard::lastOne(uint64_t mask) {
	if(!mask) { return 64; }
	return 63 - __builtin_clzll(mask);
}

void BitBoard::preInit() {
	zobristGenerator();

	castlingMap = 0;

	uint64_t mul = 1;
	for(unsigned int y = 0; y < BOARD_SIZE; ++y) {
		for(unsigned int x = 0; x < BOARD_SIZE; ++x) {
			vec2_cells[y][x] = mul;
			mul *= 2;
		}
	}

	for(int i = 0; i < 32; ++i) {
		for(int j = 0; j < BOARD_SIZE; ++j) {
			for(int k = 0; k < BOARD_SIZE; ++k) {
				bitboard[i][j][k] = 0;
			}
		}
	}

	for(int i = 0; i < 65; ++i) {
		plus1[i] = 0;
		plus7[i] = 0;
		plus8[i] = 0;
		plus9[i] = 0;
		minus1[i] = 0;
		minus7[i] = 0;
		minus8[i] = 0;
		minus9[i] = 0;
	}

	uint64_t it = 1;
	for(int i = 0; i < 64; ++i) {
		vec1_cells[i] = it;
		vec2_cells[i / 8][i % 8] = it;
		it <<= 1;
	}

	for(int y = 0; y < 8; ++y) {
		for(int x = 0; x < 8; ++x) {
			for(int k = 0; k < 8; ++k) {
				if(k != x) {
					bitboard[ROOK | WHITE][y][x] += vec2_cells[y][k];
					bitboard[ROOK | BLACK][y][x] += vec2_cells[y][k];
					bitboard[QUEEN | WHITE][y][x] += vec2_cells[y][k];
					bitboard[QUEEN | BLACK][y][x] += vec2_cells[y][k];
				}
			}

			for(int k = 0; k < 8; ++k) {
				if(k != y) {
					bitboard[ROOK | WHITE][y][x] += vec2_cells[k][x];
					bitboard[ROOK | BLACK][y][x] += vec2_cells[k][x];
					bitboard[QUEEN | WHITE][y][x] += vec2_cells[k][x];
					bitboard[QUEEN | BLACK][y][x] += vec2_cells[k][x];
				}
			}

			for(int i = y + 1, j = x + 1; i < 8 && j < 8; ++i, ++j) {
				bitboard[QUEEN | WHITE][y][x] += vec2_cells[i][j];
				bitboard[QUEEN | BLACK][y][x] += vec2_cells[i][j];
				bitboard[BISHOP | WHITE][y][x] += vec2_cells[i][j];
				bitboard[BISHOP | BLACK][y][x] += vec2_cells[i][j];
			}

			for(int i = y + 1, j = x - 1; i < 8 && j >= 0; ++i, --j) {
				bitboard[QUEEN | WHITE][y][x] += vec2_cells[i][j];
				bitboard[QUEEN | BLACK][y][x] += vec2_cells[i][j];
				bitboard[BISHOP | WHITE][y][x] += vec2_cells[i][j];
				bitboard[BISHOP | BLACK][y][x] += vec2_cells[i][j];
			}

			for(int i = y - 1, j = x + 1; i >= 0 && j < 8; --i, ++j) {
				bitboard[QUEEN | WHITE][y][x] += vec2_cells[i][j];
				bitboard[QUEEN | BLACK][y][x] += vec2_cells[i][j];
				bitboard[BISHOP | WHITE][y][x] += vec2_cells[i][j];
				bitboard[BISHOP | BLACK][y][x] += vec2_cells[i][j];
			}

			for(int i = y - 1, j = x - 1; i >= 0 && j >= 0; --i, --j) {
				bitboard[QUEEN | WHITE][y][x] += vec2_cells[i][j];
				bitboard[QUEEN | BLACK][y][x] += vec2_cells[i][j];
				bitboard[BISHOP | WHITE][y][x] += vec2_cells[i][j];
				bitboard[BISHOP | BLACK][y][x] += vec2_cells[i][j];
			}

			if(y < BOARD_SIZE - 2 && x < BOARD_SIZE - 1) {
				bitboard[KNIGHT | WHITE][y][x] |= vec2_cells[y+2][x+1];
				bitboard[KNIGHT | BLACK][y][x] |= vec2_cells[y+2][x+1];
			}

			if(y < BOARD_SIZE - 2 && x > 0) {
				bitboard[KNIGHT | WHITE][y][x] |= vec2_cells[y+2][x-1];
				bitboard[KNIGHT | BLACK][y][x] |= vec2_cells[y+2][x-1];
			}

			if(y > 1 && x < BOARD_SIZE - 1) {
				bitboard[KNIGHT | WHITE][y][x] |= vec2_cells[y-2][x+1];
				bitboard[KNIGHT | BLACK][y][x] |= vec2_cells[y-2][x+1];
			}

			if(y > 1 && x > 0) {
				bitboard[KNIGHT | WHITE][y][x] |= vec2_cells[y-2][x-1];
				bitboard[KNIGHT | BLACK][y][x] |= vec2_cells[y-2][x-1];
			}

			if(y < BOARD_SIZE - 1 && x < BOARD_SIZE - 2) {
				bitboard[KNIGHT | WHITE][y][x] |= vec2_cells[y+1][x+2];
				bitboard[KNIGHT | BLACK][y][x] |= vec2_cells[y+1][x+2];
			}

			if(y < BOARD_SIZE - 1 && x > 1) {
				bitboard[KNIGHT | WHITE][y][x] |= vec2_cells[y+1][x-2];
				bitboard[KNIGHT | BLACK][y][x] |= vec2_cells[y+1][x-2];
			}

			if(y > 0 && x < BOARD_SIZE - 2) {
				bitboard[KNIGHT | WHITE][y][x] |= vec2_cells[y-1][x+2];
				bitboard[KNIGHT | BLACK][y][x] |= vec2_cells[y-1][x+2];
			}

			if(y > 0 && x > 1) {
				bitboard[KNIGHT | WHITE][y][x] |= vec2_cells[y-1][x-2];
				bitboard[KNIGHT | BLACK][y][x] |= vec2_cells[y-1][x-2];
			}




			if(y > 0 && x > 0) {
				bitboard[KING | WHITE][y][x] |= vec2_cells[y-1][x-1];
				bitboard[KING | BLACK][y][x] |= vec2_cells[y-1][x-1];
			}

			if(y > 0) {
				bitboard[KING | WHITE][y][x] |= vec2_cells[y-1][x];
				bitboard[KING | BLACK][y][x] |= vec2_cells[y-1][x];
			}

			if(y > 0 && x < BOARD_SIZE - 1) {
				bitboard[KING | WHITE][y][x] |= vec2_cells[y-1][x+1];
				bitboard[KING | BLACK][y][x] |= vec2_cells[y-1][x+1];
			}

			if(x > 0) {
				bitboard[KING | WHITE][y][x] |= vec2_cells[y][x-1];
				bitboard[KING | BLACK][y][x] |= vec2_cells[y][x-1];
			}

			if(x < BOARD_SIZE - 1) {
				bitboard[KING | WHITE][y][x] |= vec2_cells[y][x+1];
				bitboard[KING | BLACK][y][x] |= vec2_cells[y][x+1];
			}

			if(y < BOARD_SIZE - 1 && x > 0) {
				bitboard[KING | WHITE][y][x] |= vec2_cells[y+1][x-1];
				bitboard[KING | BLACK][y][x] |= vec2_cells[y+1][x-1];
			}

			if(y < BOARD_SIZE - 1) {
				bitboard[KING | WHITE][y][x] |= vec2_cells[y+1][x];
				bitboard[KING | BLACK][y][x] |= vec2_cells[y+1][x];
			}

			if(y < BOARD_SIZE - 1 && x < BOARD_SIZE - 1) {
				bitboard[KING | WHITE][y][x] |= vec2_cells[y+1][x+1];
				bitboard[KING | BLACK][y][x] |= vec2_cells[y+1][x+1];
			}

			if(y > 0 && y < 7) {
				bitboard[PAWN | WHITE][y][x] |= vec2_cells[y+1][x];
				bitboard[PAWN | BLACK][y][x] |= vec2_cells[y-1][x];

				if(y == 1) {
					bitboard[PAWN | WHITE][y][x] |= vec2_cells[y+2][x];
				}
				if(y == 6) {
					bitboard[PAWN | BLACK][y][x] |= vec2_cells[y-2][x];
				}
			}
		}
	}

	for(int i = 0; i < 64; ++i) {
		for(int y = i / 8 + 1, x = i % 8 + 1; y < 8 && x < 8; ++y, ++x) {
			plus9[i] |= vec2_cells[y][x];
		}

		for(int y = i / 8 + 1, x = i % 8 - 1; y < 8 && x >= 0; ++y, --x) {
			plus7[i] |= vec2_cells[y][x];
		}

		for(int y = i / 8 - 1, x = i % 8 - 1; y >= 0 && x >= 0; --y, --x) {
			minus9[i] |= vec2_cells[y][x];
		}

		for(int y = i / 8 - 1, x = i % 8 + 1; y >= 0 && x < 8; --y, ++x) {
			minus7[i] |= vec2_cells[y][x];
		}

		for(int y = i / 8 + 1, x =  i % 8; y < 8; ++y) {
			plus8[i] |= vec2_cells[y][x];
		}

		for(int y = i / 8, x =  i % 8 + 1; x < 8; ++x) {
			plus1[i] |= vec2_cells[y][x];
		}

		for(int y = i / 8 - 1, x =  i % 8; y >= 0; --y) {
			minus8[i] |= vec2_cells[y][x];
		}

		for(int y = i / 8, x = i % 8 - 1; x >= 0; --x) {
			minus1[i] |= vec2_cells[y][x];
		}
	}

	uint64_t res = 255;
	for(unsigned int i = 0; i < 8; ++i) {
		horizontal[i] = res;
		res <<= 8;
	}

	res = 0;
	for(int i = 0; i < 8; ++i) {
		res |= vec2_cells[i][0];
	}

	for(unsigned int i = 0; i < 8; ++i) {
		vertical[i] = res;
		res <<= 1;
	}
}

void BitBoard::bitBoardMoveGenerator(MoveArray& moveArray) {
	moveArray.clear();
	bitBoardAttackMoveGenerator(moveArray);
	uint64_t possibleMoves, mask, emask;

	uint8_t color, enemyColor;
	if(whiteMove) {
		color = WHITE;
		enemyColor = BLACK;
		mask = white_bit_mask;
		emask = black_bit_mask;
	} else {
		color = BLACK;
		enemyColor = WHITE;
		mask = black_bit_mask;
		emask = white_bit_mask;
	}

	//Ладьи
	uint64_t rook = figures[ROOK] & mask;
	while(rook != 0) {
		possibleMoves = 0;
		uint8_t pos = firstOne(rook);
		possibleMoves |= (minus1[pos] & (UINT64_MAX ^ minus1[lastOne(minus1[pos] & (white_bit_mask | black_bit_mask))]));
		possibleMoves |= (minus8[pos] & (UINT64_MAX ^ minus8[lastOne(minus8[pos] & (white_bit_mask | black_bit_mask))]));
		possibleMoves |= ( plus1[pos] & (UINT64_MAX ^  plus1[firstOne(plus1[pos] & (white_bit_mask | black_bit_mask))]));
		possibleMoves |= ( plus8[pos] & (UINT64_MAX ^  plus8[firstOne(plus8[pos] & (white_bit_mask | black_bit_mask))]));
		possibleMoves &= (mask ^ UINT64_MAX);
		possibleMoves &= ((figures[KING] ^ emask) ^ UINT64_MAX);
		possibleMoves &= (UINT64_MAX ^ emask);
		rook &= (UINT64_MAX ^ vec1_cells[pos]);
		stress += popcount64(possibleMoves);

		while(possibleMoves != 0) {
			uint64_t to = firstOne(possibleMoves);
			moveArray.addMove(BitMove(ROOK | color, pos / 8, pos % 8, to / 8, to % 8));
			possibleMoves &= (UINT64_MAX ^ vec1_cells[to]);
		}
	}

	//Слоны
	uint64_t bishop = figures[BISHOP] & mask;
	while(bishop != 0) {
		possibleMoves = 0;
		uint8_t pos = firstOne(bishop);
		possibleMoves |= (minus7[pos] & (UINT64_MAX ^ minus7[lastOne(minus7[pos] & (white_bit_mask | black_bit_mask))]));
		possibleMoves |= (minus9[pos] & (UINT64_MAX ^ minus9[lastOne(minus9[pos] & (white_bit_mask | black_bit_mask))]));
		possibleMoves |= ( plus7[pos] & (UINT64_MAX ^  plus7[firstOne(plus7[pos] & (white_bit_mask | black_bit_mask))]));
		possibleMoves |= ( plus9[pos] & (UINT64_MAX ^  plus9[firstOne(plus9[pos] & (white_bit_mask | black_bit_mask))]));
		possibleMoves &= (mask ^ UINT64_MAX);
		possibleMoves &= ((figures[KING] ^ emask) ^ UINT64_MAX);
		possibleMoves &= (UINT64_MAX ^ emask);
		bishop &= (UINT64_MAX ^ vec1_cells[pos]);
		stress += popcount64(possibleMoves);

		while(possibleMoves != 0) {
			uint64_t to = firstOne(possibleMoves);
			moveArray.addMove(BitMove(BISHOP | color, pos / 8, pos % 8, to / 8, to % 8));
			possibleMoves &= (UINT64_MAX ^ vec1_cells[to]);
		}
	}

	//Ферзи
	uint64_t queen = figures[QUEEN] & mask;
	while(queen != 0) {
		possibleMoves = 0;
		uint8_t pos = firstOne(queen);
		possibleMoves |= (minus1[pos] & (UINT64_MAX ^ minus1[lastOne(minus1[pos] & (white_bit_mask | black_bit_mask))]));
		possibleMoves |= (minus8[pos] & (UINT64_MAX ^ minus8[lastOne(minus8[pos] & (white_bit_mask | black_bit_mask))]));
		possibleMoves |= ( plus1[pos] & (UINT64_MAX ^  plus1[firstOne(plus1[pos] & (white_bit_mask | black_bit_mask))]));
		possibleMoves |= ( plus8[pos] & (UINT64_MAX ^  plus8[firstOne(plus8[pos] & (white_bit_mask | black_bit_mask))]));
		possibleMoves |= (minus7[pos] & (UINT64_MAX ^ minus7[lastOne(minus7[pos] & (white_bit_mask | black_bit_mask))]));
		possibleMoves |= (minus9[pos] & (UINT64_MAX ^ minus9[lastOne(minus9[pos] & (white_bit_mask | black_bit_mask))]));
		possibleMoves |= ( plus7[pos] & (UINT64_MAX ^  plus7[firstOne(plus7[pos] & (white_bit_mask | black_bit_mask))]));
		possibleMoves |= ( plus9[pos] & (UINT64_MAX ^  plus9[firstOne(plus9[pos] & (white_bit_mask | black_bit_mask))]));
		possibleMoves &= (mask ^ UINT64_MAX);
		possibleMoves &= ((figures[KING] ^ emask) ^ UINT64_MAX);
		possibleMoves &= (UINT64_MAX ^ emask);
		queen &= (UINT64_MAX ^ vec1_cells[pos]);
		stress += popcount64(possibleMoves);

		while(possibleMoves != 0) {
			uint64_t to = firstOne(possibleMoves);
			moveArray.addMove(BitMove(QUEEN | color, pos / 8, pos % 8, to / 8, to % 8));
			possibleMoves &= (UINT64_MAX ^ vec1_cells[to]);
		}
	}

	//Кони
	uint64_t knight = figures[KNIGHT] & mask;
	while(knight != 0) {
		uint8_t pos = firstOne(knight);
		possibleMoves = bitboard[KNIGHT | color][pos / 8][pos % 8] & (UINT64_MAX ^ mask) & (UINT64_MAX ^ (figures[KING] & emask));
		knight &= (UINT64_MAX ^ vec1_cells[pos]);
		possibleMoves &= (UINT64_MAX ^ emask);
		stress += popcount64(possibleMoves);

		while(possibleMoves != 0) {
			uint64_t to = firstOne(possibleMoves);
			moveArray.addMove(BitMove(KNIGHT | color, pos / 8, pos % 8, to / 8, to % 8));
			possibleMoves &= (UINT64_MAX ^ vec1_cells[to]);
			possibleMoves &= ((figures[KING] & emask) ^ UINT64_MAX);
		}
	}

	//Короли
	uint64_t king = figures[KING] & mask;
	while(king != 0) {
		uint8_t pos = firstOne(king);
		possibleMoves = bitboard[KING | color][pos / 8][pos % 8] & (UINT64_MAX ^ mask) & (UINT64_MAX ^ (figures[KING] & emask));
		king &= (UINT64_MAX ^ vec1_cells[pos]);
		possibleMoves &= (UINT64_MAX ^ emask);
		stress += popcount64(possibleMoves);

		while(possibleMoves != 0) {
			uint64_t to = firstOne(possibleMoves);
			moveArray.addMove(BitMove(KING | color, pos / 8, pos % 8, to / 8, to % 8));
			possibleMoves &= (UINT64_MAX ^ vec1_cells[to]);
		}
	}

	if(whiteMove) {
		if(wsc()) {
			if(((white_bit_mask | black_bit_mask) & (vec2_cells[0][5] | vec2_cells[0][6])) == 0) {
				if(!inCheck(WHITE, 0, 4) && !inCheck(WHITE, 0, 5) && !inCheck(WHITE, 0, 6)) {
					moveArray.addMove(BitMove(KING | color, 0, 4, 0, 6));
				}
			}
		}
		if(wlc()) {
			if(((white_bit_mask | black_bit_mask) & (vec2_cells[0][3] | vec2_cells[0][2] | vec2_cells[0][1])) == 0) {
				if(!inCheck(WHITE, 0, 4) && !inCheck(WHITE, 0, 3) && !inCheck(WHITE, 0, 2)) {
					moveArray.addMove(BitMove(KING | color, 0, 4, 0, 2));
				}
			}

		}
	} else {
		if(bsc()) {
			if(((white_bit_mask | black_bit_mask) & (vec2_cells[7][5] | vec2_cells[7][6])) == 0) {
				if(!inCheck(BLACK, 7, 4) && !inCheck(BLACK, 7, 5) && !inCheck(BLACK, 7, 6)) {
					moveArray.addMove(BitMove(KING | color, 7, 4, 7, 6));
				}
			}
		}
		if(blc()) {
			if(((white_bit_mask | black_bit_mask) & (vec2_cells[7][3] | vec2_cells[7][2] | vec2_cells[7][1])) == 0) {
				if(!inCheck(BLACK, 7, 4) && !inCheck(BLACK, 7, 3) && !inCheck(BLACK, 7, 2)) {
					moveArray.addMove(BitMove(KING | color, 7, 4, 7, 2));
				}
			}

		}
	}

	//Пешки
	if(whiteMove) {
		uint64_t pawn = figures[PAWN] & mask & (UINT64_MAX^horizontal[6]);
		while(pawn != 0) {
			uint8_t pos = firstOne(pawn);
			possibleMoves = bitboard[PAWN | WHITE][pos / 8][pos % 8];
			possibleMoves &= ( plus8[pos] & (UINT64_MAX ^  plus8[firstOne(plus8[pos] & (white_bit_mask | black_bit_mask))]));
			possibleMoves &= UINT64_MAX ^ (vec1_cells[firstOne(plus8[pos] & (white_bit_mask | black_bit_mask))]);
			possibleMoves &= ((mask | emask) ^ UINT64_MAX);
			pawn &= (UINT64_MAX ^ vec1_cells[pos]);
			stress += popcount64(possibleMoves);

			while(possibleMoves != 0) {
				uint64_t to = firstOne(possibleMoves);
				moveArray.addMove(BitMove(PAWN | color, pos / 8, pos % 8, to / 8, to % 8));
				possibleMoves &= (UINT64_MAX ^ vec1_cells[to]);
			}
		}

		pawn = figures[PAWN] & mask & (horizontal[6]);
		while(pawn != 0) {
			uint8_t pos = firstOne(pawn);
			possibleMoves = bitboard[PAWN | WHITE][pos / 8][pos % 8];
			possibleMoves &= ( plus8[pos] & (UINT64_MAX ^  plus8[firstOne(plus8[pos] & (white_bit_mask | black_bit_mask))]));
			possibleMoves &= UINT64_MAX ^ (vec1_cells[firstOne(plus8[pos] & (white_bit_mask | black_bit_mask))]);
			possibleMoves &= ((mask | emask) ^ UINT64_MAX);
			pawn &= (UINT64_MAX ^ vec1_cells[pos]);
			stress += popcount64(possibleMoves);

			while(possibleMoves != 0) {
				uint64_t to = firstOne(possibleMoves);

				BitMove replaced(PAWN | color, pos / 8, pos % 8, to / 8, to % 8);
				replaced.setReplaced(KNIGHT | color);
				moveArray.addMove(replaced);

				replaced.setReplaced(BISHOP | color);
				moveArray.addMove(replaced);

				replaced.setReplaced(ROOK | color);
				moveArray.addMove(replaced);

				replaced.setReplaced(QUEEN | color);
				moveArray.addMove(replaced);

				possibleMoves &= (UINT64_MAX ^ vec1_cells[to]);
			}
		}
	} else {
		uint64_t pawn = figures[PAWN] & mask & (UINT64_MAX^horizontal[1]);
		while(pawn != 0) {
			uint8_t pos = firstOne(pawn);
			possibleMoves = bitboard[PAWN | BLACK][pos / 8][pos % 8];
			possibleMoves &= ( minus8[pos] & (UINT64_MAX ^ minus8[lastOne(minus8[pos] & (white_bit_mask | black_bit_mask))]));
			possibleMoves &= UINT64_MAX ^ (vec1_cells[lastOne(minus8[pos] & (white_bit_mask | black_bit_mask))]);
			possibleMoves &= ((mask | emask) ^ UINT64_MAX);
			pawn &= (UINT64_MAX ^ vec1_cells[pos]);
			stress += popcount64(possibleMoves);

			while(possibleMoves != 0) {
				uint64_t to = firstOne(possibleMoves);
				moveArray.addMove(BitMove(PAWN | color, pos / 8, pos % 8, to / 8, to % 8));
				possibleMoves &= (UINT64_MAX ^ vec1_cells[to]);
			}
		}

		pawn = figures[PAWN] & mask & (horizontal[1]);

		while(pawn != 0) {
			uint8_t pos = firstOne(pawn);
			possibleMoves = bitboard[PAWN | BLACK][pos / 8][pos % 8];
			possibleMoves &= ( minus8[pos] & (UINT64_MAX ^ minus8[firstOne(minus8[pos] & (white_bit_mask | black_bit_mask))]));
			possibleMoves &= UINT64_MAX ^ (vec1_cells[firstOne(minus8[pos] & (white_bit_mask | black_bit_mask))]);
			possibleMoves &= ((mask | emask) ^ UINT64_MAX);
			pawn &= (UINT64_MAX ^ vec1_cells[pos]);
			stress += popcount64(possibleMoves);

			while(possibleMoves != 0) {
				uint64_t to = firstOne(possibleMoves);

				BitMove replaced(PAWN | color, pos / 8, pos % 8, to / 8, to % 8);
				replaced.setReplaced(KNIGHT | color);
				moveArray.addMove(replaced);

				replaced.setReplaced(BISHOP | color);
				moveArray.addMove(replaced);

				replaced.setReplaced(ROOK | color);
				moveArray.addMove(replaced);

				replaced.setReplaced(QUEEN | color);
				moveArray.addMove(replaced);

				possibleMoves &= (UINT64_MAX ^ vec1_cells[to]);
			}
		}
	}
}

void BitBoard::bitBoardAttackMoveGenerator(MoveArray& moveArray) {
	moveArray.clear();
	uint64_t possibleMoves, mask, emask;

	uint8_t color, enemyColor;
	if(whiteMove) {
		color = WHITE;
		enemyColor = BLACK;
		mask = white_bit_mask;
		emask = black_bit_mask;
	} else {
		color = BLACK;
		enemyColor = WHITE;
		mask = black_bit_mask;
		emask = white_bit_mask;
	}

	//Пешки
	if(whiteMove) {
		uint64_t pawn = figures[PAWN] & mask & (UINT64_MAX ^ horizontal[6]);
		uint64_t rightPawnAttack = (pawn << 9) & (UINT64_MAX ^ vertical[0]) & emask & (UINT64_MAX ^ figures[KING]);
		moveArray.num_attacks += popcount64(rightPawnAttack);
		while(rightPawnAttack != 0) {
			uint64_t to = firstOne(rightPawnAttack);
			moveArray.addMove(BitMove(getFigure(to / 8, to % 8), PAWN | color, to / 8 - 1, to % 8 - 1, to / 8, to % 8));
			rightPawnAttack &= (UINT64_MAX ^ vec1_cells[to]);
		}
		uint64_t leftPawnAttack = (pawn << 7) & (UINT64_MAX ^ vertical[7]) & emask & (UINT64_MAX ^ figures[KING]);
		moveArray.num_attacks += popcount64(leftPawnAttack);
		while(leftPawnAttack != 0) {
			uint64_t to = firstOne(leftPawnAttack);
			moveArray.addMove(BitMove(getFigure(to / 8, to % 8), PAWN | color, to / 8 - 1, to % 8 + 1, to / 8, to % 8));
			leftPawnAttack &= (UINT64_MAX ^ vec1_cells[to]);
		}

		pawn = figures[PAWN] & mask & horizontal[6];
		rightPawnAttack = (pawn << 9) & (UINT64_MAX ^ vertical[0]) & emask & (UINT64_MAX ^ figures[KING]);
		moveArray.num_attacks += popcount64(rightPawnAttack);
		while(rightPawnAttack != 0) {
			uint64_t to = firstOne(rightPawnAttack);
			BitMove replaced = BitMove(getFigure(to / 8, to % 8), PAWN | color, to / 8 - 1, to % 8 - 1, to / 8, to % 8);

			replaced.setReplaced(KNIGHT | color);
			moveArray.addMove(replaced);

			replaced.setReplaced(BISHOP | color);
			moveArray.addMove(replaced);

			replaced.setReplaced(ROOK | color);
			moveArray.addMove(replaced);

			replaced.setReplaced(QUEEN | color);
			moveArray.addMove(replaced);

			rightPawnAttack &= (UINT64_MAX ^ vec1_cells[to]);
		}
		leftPawnAttack = (pawn << 7) & (UINT64_MAX ^ vertical[7]) & emask & (UINT64_MAX ^ figures[KING]);
		moveArray.num_attacks += popcount64(leftPawnAttack);
		while(leftPawnAttack != 0) {
			uint64_t to = firstOne(leftPawnAttack);
			BitMove replaced = BitMove(getFigure(to / 8, to % 8), PAWN | color, to / 8 - 1, to % 8 + 1, to / 8, to % 8);

			replaced.setReplaced(KNIGHT | color);
			moveArray.addMove(replaced);

			replaced.setReplaced(BISHOP | color);
			moveArray.addMove(replaced);

			replaced.setReplaced(ROOK | color);
			moveArray.addMove(replaced);

			replaced.setReplaced(QUEEN | color);
			moveArray.addMove(replaced);

			leftPawnAttack &= (UINT64_MAX ^ vec1_cells[to]);
		}
	} else {
		uint64_t pawn = figures[PAWN] & mask & (UINT64_MAX ^ horizontal[1]);
		uint64_t rightPawnAttack = (pawn >> 9) & (UINT64_MAX ^ vertical[7]) & emask & (UINT64_MAX ^ figures[KING]);
		moveArray.num_attacks += popcount64(rightPawnAttack);
		while(rightPawnAttack != 0) {
			uint64_t to = firstOne(rightPawnAttack);
			moveArray.addMove(BitMove(getFigure(to / 8, to % 8), PAWN | color, to / 8 + 1, to % 8 + 1, to / 8, to % 8));
			rightPawnAttack &= (UINT64_MAX ^ vec1_cells[to]);
		}

		uint64_t leftPawnAttack = (pawn >> 7) & (UINT64_MAX ^ vertical[0]) & emask & (UINT64_MAX ^ figures[KING]);
		moveArray.num_attacks += popcount64(leftPawnAttack);
		while(leftPawnAttack != 0) {
			uint64_t to = firstOne(leftPawnAttack);
			moveArray.addMove(BitMove(getFigure(to / 8, to % 8), PAWN | color, to / 8 + 1, to % 8 - 1, to / 8, to % 8));
			leftPawnAttack &= (UINT64_MAX ^ vec1_cells[to]);
		}

		pawn = figures[PAWN] & mask & horizontal[1];
		rightPawnAttack = (pawn >> 9) & (UINT64_MAX ^ vertical[7]) & emask & (UINT64_MAX ^ figures[KING]);
		moveArray.num_attacks += popcount64(rightPawnAttack);
		while(rightPawnAttack != 0) {
			uint64_t to = firstOne(rightPawnAttack);
			BitMove replaced = BitMove(getFigure(to / 8, to % 8), PAWN | color, to / 8 + 1, to % 8 + 1, to / 8, to % 8);

			replaced.setReplaced(KNIGHT | color);
			moveArray.addMove(replaced);

			replaced.setReplaced(BISHOP | color);
			moveArray.addMove(replaced);

			replaced.setReplaced(ROOK | color);
			moveArray.addMove(replaced);

			replaced.setReplaced(QUEEN | color);
			moveArray.addMove(replaced);

			rightPawnAttack &= (UINT64_MAX ^ vec1_cells[to]);
		}
		leftPawnAttack = (pawn >> 7) & (UINT64_MAX ^ vertical[0]) & emask & (UINT64_MAX ^ figures[KING]);
		moveArray.num_attacks += popcount64(leftPawnAttack);
		while(leftPawnAttack != 0) {
			uint64_t to = firstOne(leftPawnAttack);
			BitMove replaced = BitMove(getFigure(to / 8, to % 8), PAWN | color, to / 8 + 1, to % 8 - 1, to / 8, to % 8);

			replaced.setReplaced(KNIGHT | color);
			moveArray.addMove(replaced);

			replaced.setReplaced(BISHOP | color);
			moveArray.addMove(replaced);

			replaced.setReplaced(ROOK | color);
			moveArray.addMove(replaced);

			replaced.setReplaced(QUEEN | color);
			moveArray.addMove(replaced);

			leftPawnAttack &= (UINT64_MAX ^ vec1_cells[to]);
		}
	}

	//Пешки (взятие на проходе)

	if(passant_enable) {
		if(whiteMove) {
			uint64_t passant_cell = vec2_cells[passant_y][passant_x];
			uint64_t whitePawnMask = figures[PAWN] & white_bit_mask & horizontal[4];

			if(((passant_cell >> 9) & whitePawnMask)) {
				moveArray.addMove(BitMove(PAWN | BLACK, PAWN | WHITE, passant_y - 1, passant_x - 1, passant_y, passant_x, true));
			}

			if(((passant_cell >> 7) & whitePawnMask)) {
				moveArray.addMove(BitMove(PAWN | BLACK, PAWN | WHITE, passant_y - 1, passant_x + 1, passant_y, passant_x, true));
			}
		} else {
			uint64_t passant_cell = vec2_cells[passant_y][passant_x];
			uint64_t blackPawnMask = figures[PAWN] & black_bit_mask & horizontal[3];

			if(((passant_cell << 9) & blackPawnMask)) {
				moveArray.addMove(BitMove(PAWN | WHITE, PAWN | BLACK, passant_y + 1, passant_x + 1, passant_y, passant_x, true));
			}

			if(((passant_cell << 7) & blackPawnMask)) {
				moveArray.addMove(BitMove(PAWN | WHITE, PAWN | BLACK, passant_y + 1, passant_x - 1, passant_y, passant_x, true));
			}
		}
	}

	//Кони
	uint64_t knight = figures[KNIGHT] & mask;
	while(knight != 0) {
		uint8_t pos = firstOne(knight);
		possibleMoves = bitboard[KNIGHT | color][pos / 8][pos % 8] & (UINT64_MAX ^ mask) & (UINT64_MAX ^ (figures[KING] & emask));
		possibleMoves &= emask;
		knight &= (UINT64_MAX ^ vec1_cells[pos]);
		stress += popcount64(possibleMoves);
		moveArray.num_attacks += popcount64(possibleMoves);

		while(possibleMoves != 0) {
			uint64_t to = firstOne(possibleMoves);
			moveArray.addMove(BitMove(getFigure(to / 8, to % 8), KNIGHT | color, pos / 8, pos % 8, to / 8, to % 8));
			possibleMoves &= (UINT64_MAX ^ vec1_cells[to]);
			possibleMoves &= ((figures[KING] & emask) ^ UINT64_MAX);
		}
	}

	//Слоны
	uint64_t bishop = figures[BISHOP] & mask;
	while(bishop != 0) {
		possibleMoves = 0;
		uint8_t pos = firstOne(bishop);
		possibleMoves |= (minus7[pos] & (UINT64_MAX ^ minus7[lastOne(minus7[pos] & (white_bit_mask | black_bit_mask))]));
		possibleMoves |= (minus9[pos] & (UINT64_MAX ^ minus9[lastOne(minus9[pos] & (white_bit_mask | black_bit_mask))]));
		possibleMoves |= ( plus7[pos] & (UINT64_MAX ^  plus7[firstOne(plus7[pos] & (white_bit_mask | black_bit_mask))]));
		possibleMoves |= ( plus9[pos] & (UINT64_MAX ^  plus9[firstOne(plus9[pos] & (white_bit_mask | black_bit_mask))]));
		possibleMoves &= (mask ^ UINT64_MAX);
		possibleMoves &= ((figures[KING] & emask) ^ UINT64_MAX);
		possibleMoves &= emask;
		bishop &= (UINT64_MAX ^ vec1_cells[pos]);
		stress += popcount64(possibleMoves);
		moveArray.num_attacks += popcount64(possibleMoves);

		while(possibleMoves != 0) {
			uint64_t to = firstOne(possibleMoves);
			moveArray.addMove(BitMove(getFigure(to / 8, to % 8), BISHOP | color, pos / 8, pos % 8, to / 8, to % 8));
			possibleMoves &= (UINT64_MAX ^ vec1_cells[to]);
		}
	}

	//Ладьи
	uint64_t rook = figures[ROOK] & mask;
	while(rook != 0) {
		possibleMoves = 0;
		uint8_t pos = firstOne(rook);
		possibleMoves |= (minus1[pos] & (UINT64_MAX ^ minus1[lastOne(minus1[pos] & (white_bit_mask | black_bit_mask))]));
		possibleMoves |= (minus8[pos] & (UINT64_MAX ^ minus8[lastOne(minus8[pos] & (white_bit_mask | black_bit_mask))]));
		possibleMoves |= ( plus1[pos] & (UINT64_MAX ^  plus1[firstOne(plus1[pos] & (white_bit_mask | black_bit_mask))]));
		possibleMoves |= ( plus8[pos] & (UINT64_MAX ^  plus8[firstOne(plus8[pos] & (white_bit_mask | black_bit_mask))]));
		possibleMoves &= (mask ^ UINT64_MAX);
		possibleMoves &= ((figures[KING] & emask) ^ UINT64_MAX);
		possibleMoves &= emask;
		rook &= (UINT64_MAX ^ vec1_cells[pos]);
		stress += popcount64(possibleMoves);
		moveArray.num_attacks += popcount64(possibleMoves);

		while(possibleMoves != 0) {
			uint64_t to = firstOne(possibleMoves);
			moveArray.addMove(BitMove(getFigure(to / 8, to % 8), ROOK | color, pos / 8, pos % 8, to / 8, to % 8));
			possibleMoves &= (UINT64_MAX ^ vec1_cells[to]);
		}
	}

	//Ферзи
	uint64_t queen = figures[QUEEN] & mask;
	while(queen != 0) {
		possibleMoves = 0;
		uint8_t pos = firstOne(queen);
		possibleMoves |= (minus1[pos] & (UINT64_MAX ^ minus1[lastOne(minus1[pos] & (white_bit_mask | black_bit_mask))]));
		possibleMoves |= (minus8[pos] & (UINT64_MAX ^ minus8[lastOne(minus8[pos] & (white_bit_mask | black_bit_mask))]));
		possibleMoves |= ( plus1[pos] & (UINT64_MAX ^  plus1[firstOne(plus1[pos] & (white_bit_mask | black_bit_mask))]));
		possibleMoves |= ( plus8[pos] & (UINT64_MAX ^  plus8[firstOne(plus8[pos] & (white_bit_mask | black_bit_mask))]));
		possibleMoves |= (minus7[pos] & (UINT64_MAX ^ minus7[lastOne(minus7[pos] & (white_bit_mask | black_bit_mask))]));
		possibleMoves |= (minus9[pos] & (UINT64_MAX ^ minus9[lastOne(minus9[pos] & (white_bit_mask | black_bit_mask))]));
		possibleMoves |= ( plus7[pos] & (UINT64_MAX ^  plus7[firstOne(plus7[pos] & (white_bit_mask | black_bit_mask))]));
		possibleMoves |= ( plus9[pos] & (UINT64_MAX ^  plus9[firstOne(plus9[pos] & (white_bit_mask | black_bit_mask))]));
		possibleMoves &= (mask ^ UINT64_MAX);
		possibleMoves &= ((figures[KING] & emask) ^ UINT64_MAX);
		possibleMoves &= emask;
		queen &= (UINT64_MAX ^ vec1_cells[pos]);
		stress += popcount64(possibleMoves);
		moveArray.num_attacks += popcount64(possibleMoves);

		while(possibleMoves != 0) {
			uint64_t to = firstOne(possibleMoves);
			moveArray.addMove(BitMove(getFigure(to / 8, to % 8), QUEEN | color, pos / 8, pos % 8, to / 8, to % 8));
			possibleMoves &= (UINT64_MAX ^ vec1_cells[to]);
		}
	}

	//Короли
	uint64_t king = figures[KING] & mask;
	while(king != 0) {
		uint8_t pos = firstOne(king);
		possibleMoves = bitboard[KING | color][pos / 8][pos % 8] & (UINT64_MAX ^ mask) & (UINT64_MAX ^ (figures[KING] & emask));
		possibleMoves &= emask;
		king &= (UINT64_MAX ^ vec1_cells[pos]);
		stress += popcount64(possibleMoves);
		moveArray.num_attacks += popcount64(possibleMoves);

		while(possibleMoves != 0) {
			uint64_t to = firstOne(possibleMoves);
			moveArray.addMove(BitMove(getFigure(to / 8, to % 8), KING | color, pos / 8, pos % 8, to / 8, to % 8));
			possibleMoves &= (UINT64_MAX ^ vec1_cells[to]);
		}
	}
}

void BitBoard::move(BitMove& mv) {
	pushHistory();
	uint8_t movedFigure = getFigure(mv.fromY, mv.fromX);

	clearCell(mv.toY, mv.toX);
	clearCell(mv.fromY, mv.fromX);

	if(mv.replaced) {
		addFigure(mv.replacedFigure, mv.toY, mv.toX);
	} else {
		addFigure(movedFigure, mv.toY, mv.toX);
	}

	if(mv.passant) {
		if(whiteMove) {
			clearCell(passant_y - 1, passant_x);
		} else {
			clearCell(passant_y + 1, passant_x);
		}
	}

	if((mv.movedFigure & TYPE_SAVE) == KING) {
		if(whiteMove) {
			if(mv.fromY == 0 && mv.fromX == 4 && mv.toY == 0 && mv.toX == 6) {
				addFigure(ROOK | WHITE, 0, 5);
				clearCell(0, 7);
			} else if(mv.fromY == 0 && mv.fromX == 4 && mv.toY == 0 && mv.toX == 2) {
				addFigure(ROOK | WHITE, 0, 3);
				clearCell(0, 0);
			}
		} else {
			if(mv.fromY == 7 && mv.fromX == 4 && mv.toY == 7 && mv.toX == 6) {
				addFigure(ROOK | BLACK, 7, 5);
				clearCell(7, 7);
			} else if(mv.fromY == 7 && mv.fromX == 4 && mv.toY == 7 && mv.toX == 2) {
				addFigure(ROOK | BLACK, 7, 3);
				clearCell(7, 0);
			}
		}
	}

	passant_enable = false;

	if((mv.movedFigure & TYPE_SAVE) == PAWN && abs(mv.toY - mv.fromY) > 1) {
		passant_enable = true;
		passant_y = (mv.toY + mv.fromY) / 2;
		passant_x = mv.fromX;
	}

	whiteMove = !whiteMove;
	if(whiteMove) {
		++moveNumber;
	}

	if(!mv.isAttack && (mv.movedFigure & TYPE_SAVE) != PAWN) {
		++ruleNumber;
	} else {
		ruleNumber = 0;
	}

	castlingMap &= (figures[KING] | figures[ROOK]);
}

void BitBoard::goBack() {
	if(!history.empty()) {
		for(unsigned int i = 0; i < 7; ++i) {
			figures[i] = history.top().figures[i];
		}

		white_bit_mask = history.top().white_bit_mask;
		black_bit_mask = history.top().black_bit_mask;
		moveNumber = history.top().moveNumber;
		ruleNumber = history.top().ruleNumber;
		//wsc = history.top().wsc;
		//wlc = history.top().wlc;
		//bsc = history.top().bsc;
		//blc = history.top().blc;
		castlingMap = history.top().castlingMap;
		whiteMove = history.top().whiteMove;
		evalute = history.top().evalute;
		passant_y = history.top().passant_y;
		passant_x = history.top().passant_x;
		passant_enable = history.top().passant_enable;

		history.pop();
	}
}


void BitBoard::clearCell(uint8_t y, uint8_t x) {
	uint8_t figure = getFigure(y, x);

	hash ^= (zobrist[figure][y][x]);

	uint8_t color = (figure & COLOR_SAVE);
	if(figure) {
		if(color == WHITE) {
			figure &= TYPE_SAVE;
			if(figure == PAWN) {
				evalute -= PAWN_EV;
				evalute -= pawnMatr[7 - y][x];
			} else if(figure == KNIGHT) {
				evalute -= KNIGHT_EV;
				evalute -= knightMatr[7 - y][x];
			} else if(figure == BISHOP) {
				evalute -= BISHOP_EV;
				evalute -= bishopMatr[7 - y][x];
			} else if(figure == ROOK) {
				evalute -= ROOK_EV;
				evalute -= rookMatr[7 - y][x];
			} else if(figure == QUEEN) {
				evalute -= QUEEN_EV;
				evalute -= queenMatr[7 - y][x];
			} else if(figure == KING) {
				//evalute -= whitePawnMatr[7 - mv.fromY][mv.fromX];
			}
		} else {
			figure &= TYPE_SAVE;
			if(figure == PAWN) {
				evalute += PAWN_EV;
				evalute += pawnMatr[y][x];
			} else if(figure == KNIGHT) {
				evalute += KNIGHT_EV;
				evalute += knightMatr[y][x];
			} else if(figure == BISHOP) {
				evalute += BISHOP_EV;
				evalute += bishopMatr[y][x];
			} else if(figure == ROOK) {
				evalute += ROOK_EV;
				evalute += rookMatr[y][x];
			} else if(figure == QUEEN) {
				evalute += QUEEN_EV;
				evalute += queenMatr[y][x];
			} else if(figure == KING) {
				//evalute -= whitePawnMatr[7 - mv.fromY][mv.fromX];
			}
		}

		if((getFigure(y, x) & TYPE_SAVE) != 0) {
			figures[getFigure(y, x) & TYPE_SAVE] &= (UINT64_MAX ^ vec2_cells[y][x]);
		}

		white_bit_mask &= (UINT64_MAX ^ vec2_cells[y][x]);
		black_bit_mask &= (UINT64_MAX ^ vec2_cells[y][x]);
	}
}

void BitBoard::addFigure(uint8_t figure, uint8_t y, uint8_t x) {
	uint8_t color = (figure & COLOR_SAVE);

	hash ^= (zobrist[figure][y][x]);

	if(figure) {
		if(color == WHITE) {
			figure &= TYPE_SAVE;
			if(figure == PAWN) {
				evalute += PAWN_EV;
				evalute += pawnMatr[7 - y][x];
			} else if(figure == KNIGHT) {
				evalute += KNIGHT_EV;
				evalute += knightMatr[7 - y][x];
			} else if(figure == BISHOP) {
				evalute += BISHOP_EV;
				evalute += bishopMatr[7 - y][x];
			} else if(figure == ROOK) {
				evalute += ROOK_EV;
				evalute += rookMatr[7 - y][x];
			} else if(figure == QUEEN) {
				evalute += QUEEN_EV;
				evalute += queenMatr[7 - y][x];
			} else if(figure == KING) {
				//evalute -= whitePawnMatr[7 - mv.fromY][mv.fromX];
			}
		} else {
			figure &= TYPE_SAVE;
			if(figure == PAWN) {
				evalute -= PAWN_EV;
				evalute -= pawnMatr[y][x];
			} else if(figure == KNIGHT) {
				evalute -= KNIGHT_EV;
				evalute -= knightMatr[y][x];
			} else if(figure == BISHOP) {
				evalute -= BISHOP_EV;
				evalute -= bishopMatr[y][x];
			} else if(figure == ROOK) {
				evalute -= ROOK_EV;
				evalute -= rookMatr[y][x];
			} else if(figure == QUEEN) {
				evalute -= QUEEN_EV;
				evalute -= queenMatr[y][x];
			} else if(figure == KING) {
				//evalute -= whitePawnMatr[7 - mv.fromY][mv.fromX];
			}
		}

		figures[figure & TYPE_SAVE] |= vec2_cells[y][x];

		if(color == WHITE) {
			white_bit_mask |= vec2_cells[y][x];
		} else if(color == BLACK) {
			black_bit_mask |= vec2_cells[y][x];
		}
	}
}

void BitBoard::printBitBoard(uint64_t bit_board) {
	uint64_t doubler = 1;
	//doubler <<= 63;
	for(int y = 7; y >= 0; --y) {
		for(int x = 0; x < BOARD_SIZE; ++x) {
			std::cout << (bool)((doubler << (y * 8 + x)) & bit_board);
		}
		std::cout << "\n";
	}
	/*for(int i = 0; i < 8; ++i) {
		for(int j = 0; j < 8; ++j) {
			std::cout << (bool)(bit_board & doubler);
			doubler >>= 1;
		}
		std::cout << "\n";
	}*/
}

void BitBoard::pushHistory() {
	GoBack newHistory;

	for(unsigned int i = 0; i < 7; ++i) {
		newHistory.figures[i] = figures[i];
	}

	newHistory.white_bit_mask = white_bit_mask;
	newHistory.black_bit_mask = black_bit_mask;
	newHistory.moveNumber = moveNumber;
	newHistory.ruleNumber = ruleNumber;
	//newHistory.wsc = wsc;
	//newHistory.wlc = wlc;
	//newHistory.bsc = bsc;
	//newHistory.blc = blc;
	newHistory.whiteMove = whiteMove;
	newHistory.evalute = evalute;
	newHistory.castlingMap = castlingMap;
	newHistory.passant_enable = passant_enable;
	newHistory.passant_x = passant_x;
	newHistory.passant_y = passant_y;
	history.push(newHistory);
}

void BitBoard::evaluteAll() {
	evalute = 0;

	evalute += popcount64(figures[PAWN] & white_bit_mask) * PAWN_EV;
	evalute -= popcount64(figures[PAWN] & black_bit_mask) * PAWN_EV;
	evalute += popcount64(figures[KNIGHT] & white_bit_mask) * KNIGHT_EV;
	evalute -= popcount64(figures[KNIGHT] & black_bit_mask) * KNIGHT_EV;
	evalute += popcount64(figures[BISHOP] & white_bit_mask) * BISHOP_EV;
	evalute -= popcount64(figures[BISHOP] & black_bit_mask) * BISHOP_EV;
	evalute += popcount64(figures[ROOK] & white_bit_mask) * ROOK_EV;
	evalute -= popcount64(figures[ROOK] & black_bit_mask) * ROOK_EV;
	evalute += popcount64(figures[QUEEN] & white_bit_mask) * QUEEN_EV;
	evalute -= popcount64(figures[QUEEN] & black_bit_mask) * QUEEN_EV;

	uint64_t mask = figures[PAWN] & white_bit_mask;
	while(mask != 0) {
		uint8_t pos = firstOne(mask);
		evalute += pawnMatr[7 - pos / 8][pos % 8];
		mask &= (UINT64_MAX ^ vec1_cells[pos]);
	}

	mask = figures[PAWN] & black_bit_mask;
	while(mask != 0) {
		uint8_t pos = firstOne(mask);
		evalute -= pawnMatr[pos / 8][pos % 8];
		mask &= (UINT64_MAX ^ vec1_cells[pos]);
	}

	mask = figures[KNIGHT] & white_bit_mask;
	while(mask != 0) {
		uint8_t pos = firstOne(mask);
		evalute += knightMatr[7 - pos / 8][pos % 8];
		mask &= (UINT64_MAX ^ vec1_cells[pos]);
	}

	mask = figures[KNIGHT] & black_bit_mask;
	while(mask != 0) {
		uint8_t pos = firstOne(mask);
		evalute -= knightMatr[pos / 8][pos % 8];
		mask &= (UINT64_MAX ^ vec1_cells[pos]);
	}

	mask = figures[BISHOP] & white_bit_mask;
	while(mask != 0) {
		uint8_t pos = firstOne(mask);
		evalute += bishopMatr[7 - pos / 8][pos % 8];
		mask &= (UINT64_MAX ^ vec1_cells[pos]);
	}

	mask = figures[BISHOP] & black_bit_mask;
	while(mask != 0) {
		uint8_t pos = firstOne(mask);
		evalute -= bishopMatr[pos / 8][pos % 8];
		mask &= (UINT64_MAX ^ vec1_cells[pos]);
	}

	mask = figures[ROOK] & white_bit_mask;
	while(mask != 0) {
		uint8_t pos = firstOne(mask);
		evalute += rookMatr[7 - pos / 8][pos % 8];
		mask &= (UINT64_MAX ^ vec1_cells[pos]);
	}

	mask = figures[ROOK] & black_bit_mask;
	while(mask != 0) {
		uint8_t pos = firstOne(mask);
		evalute -= rookMatr[pos / 8][pos % 8];
		mask &= (UINT64_MAX ^ vec1_cells[pos]);
	}

	mask = figures[QUEEN] & white_bit_mask;
	while(mask != 0) {
		uint8_t pos = firstOne(mask);
		evalute += queenMatr[7 - pos / 8][pos % 8];
		mask &= (UINT64_MAX ^ vec1_cells[pos]);
	}

	mask = figures[QUEEN] & black_bit_mask;
	while(mask != 0) {
		uint8_t pos = firstOne(mask);
		evalute -= queenMatr[pos / 8][pos % 8];
		mask &= (UINT64_MAX ^ vec1_cells[pos]);
	}

	/*if(!whiteMove) {
		evalute = -evalute;
	}*/

	/*mask = figures[KING] & white_bit_mask;
	while(mask != 0) {
		uint8_t pos = firstOne(mask);
		evalute += queenMatr[7 - pos / 8][pos % 8];
		mask &= (UINT64_MAX ^ vec1_cells[pos]);
	}

	mask = figures[KING] & black_bit_mask;
	while(mask != 0) {
		uint8_t pos = firstOne(mask);
		evalute -= queenMatr[pos / 8][pos % 8];
		mask &= (UINT64_MAX ^ vec1_cells[pos]);
	}*/

	/*all_material_eval += popcount64(figureMask[PAWN | WHITE]) * PAWN_EV;
	all_material_eval += popcount64(figureMask[PAWN | BLACK]) * PAWN_EV;
	all_material_eval += popcount64(figureMask[KNIGHT | WHITE]) * KNIGHT_EV;
	all_material_eval += popcount64(figureMask[KNIGHT | BLACK]) * KNIGHT_EV;
	all_material_eval += popcount64(figureMask[BISHOP | WHITE]) * BISHOP_EV;
	all_material_eval += popcount64(figureMask[BISHOP | BLACK]) * BISHOP_EV;
	all_material_eval += popcount64(figureMask[ROOK | WHITE]) * ROOK_EV;
	all_material_eval += popcount64(figureMask[ROOK | BLACK]) * ROOK_EV;
	all_material_eval += popcount64(figureMask[QUEEN | WHITE]) * QUEEN_EV;
	all_material_eval += popcount64(figureMask[QUEEN | BLACK]) * QUEEN_EV;*/

	//double figure_state_eval = 0;

	/*for(int y = 0; y < BOARD_SIZE; ++y) {
		for(int x = 0; x < BOARD_SIZE; ++x) {
			if(b.board[y][x] == 0) {
				continue;
			}

			if((b.board[y][x] & COLOR_SAVE) == WHITE) {
				if((b.board[y][x] & TYPE_SAVE) == PAWN) {
					figure_state_eval += whitePawnMatr[y][x];
				} else if((b.board[y][x] & TYPE_SAVE) == KNIGHT) {
					figure_state_eval += knightMatr[y][x];
				} else if((b.board[y][x] & TYPE_SAVE) == BISHOP) {
					figure_state_eval += bishopMatr[y][x];
				} else if((b.board[y][x] & TYPE_SAVE) == ROOK) {
					figure_state_eval += rookMatr[y][x];
				} else if((b.board[y][x] & TYPE_SAVE) == QUEEN) {
					figure_state_eval += queenMatr[y][x];
				} else if((b.board[y][x] & TYPE_SAVE) == KING) {
					figure_state_eval += ((kingDebuteMatr[y][x] * (all_material_eval / ALL_MATERIAL)) + kingEndGameMatr[y][x] *  (1 - all_material_eval / ALL_MATERIAL));
				}
			} else {
				if((b.board[y][x] & TYPE_SAVE) == PAWN) {
					figure_state_eval -= whitePawnMatr[BOARD_SIZE - 1 - y][x];
				} else if((b.board[y][x] & TYPE_SAVE) == KNIGHT) {
					figure_state_eval -= knightMatr[BOARD_SIZE - 1 - y][x];
				} else if((b.board[y][x] & TYPE_SAVE) == BISHOP) {
					figure_state_eval -= bishopMatr[BOARD_SIZE - 1 - y][x];
				} else if((b.board[y][x] & TYPE_SAVE) == ROOK) {
					figure_state_eval -= rookMatr[BOARD_SIZE - 1 - y][x];
				} else if((b.board[y][x] & TYPE_SAVE) == QUEEN) {
					figure_state_eval -= queenMatr[BOARD_SIZE - 1 - y][x];
				} else if((b.board[y][x] & TYPE_SAVE) == KING) {
					figure_state_eval -= ((kingDebuteMatr[BOARD_SIZE - 1 - y][x] * (all_material_eval / ALL_MATERIAL)) + kingEndGameMatr[BOARD_SIZE - 1 - y][x] *  (1 - all_material_eval / ALL_MATERIAL));
				}
			}
		}
	}*/
}

uint8_t BitBoard::getFigure(uint8_t y, uint8_t x) {
	for(uint8_t i = 1; i < 7; ++i) {
		if(figures[i] & vec2_cells[y][x] & white_bit_mask) {
			return (i | WHITE);
		} else if(figures[i] & vec2_cells[y][x] & black_bit_mask) {
			return (i | BLACK);
		}
	}

	return 0;
}

double BitBoard::getEvalute() {
	if(whiteMove) {
		return evalute;
	} else {
		return -evalute;
	}
}

bool BitBoard::inCheck(uint8_t color) {
	uint64_t mask, emask;
	uint8_t ecolor;

	if(color == WHITE) {
		mask = white_bit_mask;
		emask = black_bit_mask;
		ecolor = BLACK;
	} else {
		mask = black_bit_mask;
		emask = white_bit_mask;
		ecolor = WHITE;
	}

	uint64_t kingPos = figures[KING] & mask;
	uint8_t kingCoord = firstOne(kingPos);


	uint64_t figure;
	figure = firstOne(plus8[kingCoord] & (mask | emask));
	figure = vec1_cells[figure];
	if(plus8[kingCoord] & (mask | emask) && (figure & (figures[ROOK] | figures[QUEEN]) & emask)) {
		return true;
	}

	figure = firstOne(plus1[kingCoord] & (mask | emask));
	figure = vec1_cells[figure];
	if(plus1[kingCoord] & (mask | emask) && (figure & (figures[ROOK] | figures[QUEEN]) & emask)) {
		return true;
	}

	figure = lastOne(minus8[kingCoord] & (mask | emask));
	figure = vec1_cells[figure];
	if(minus8[kingCoord] & (mask | emask) && (figure & (figures[ROOK] | figures[QUEEN]) & emask)) {
		return true;
	}

	figure = lastOne(minus1[kingCoord] & (mask | emask));
	figure = vec1_cells[figure];
	if(minus1[kingCoord] & (mask | emask) && (figure & (figures[ROOK] | figures[QUEEN]) & emask)) {
		return true;
	}

	figure = firstOne(plus7[kingCoord] & (mask | emask));
	figure = vec1_cells[figure];
	if(plus7[kingCoord] & (mask | emask) && (figure & (figures[BISHOP] | figures[QUEEN]) & emask)) {
		return true;
	}

	figure = firstOne(plus9[kingCoord] & (mask | emask));
	figure = vec1_cells[figure];
	if(plus9[kingCoord] & (mask | emask) && (figure & (figures[BISHOP] | figures[QUEEN]) & emask)) {
		return true;
	}

	figure = lastOne(minus7[kingCoord] & (mask | emask));
	figure = vec1_cells[figure];
	if(minus7[kingCoord] & (mask | emask) && (figure & (figures[BISHOP] | figures[QUEEN]) & emask)) {
		return true;
	}

	figure = lastOne(minus9[kingCoord] & (mask | emask));
	figure = vec1_cells[figure];
	if(minus9[kingCoord] & (mask | emask) && (figure & (figures[BISHOP] | figures[QUEEN]) & emask)) {
		return true;
	}

	if(bitboard[KNIGHT | WHITE][kingCoord / 8][kingCoord % 8] & figures[KNIGHT] & emask) {
		return true;
	}

	if(color == WHITE) {
		if(((figures[PAWN] & emask) >> 9) & (UINT64_MAX ^ vertical[7]) & kingPos) {
			return true;
		}

		if(((figures[PAWN] & emask) >> 7) & (UINT64_MAX ^ vertical[0]) & kingPos) {
			return true;
		}
	} else {
		if(((figures[PAWN] & emask) << 9) & (UINT64_MAX ^ vertical[0]) & kingPos) {
			return true;
		}

		if(((figures[PAWN] & emask) << 7) & (UINT64_MAX ^ vertical[7]) & kingPos) {
			return true;
		}
	}

	uint8_t eKingCoord = firstOne(figures[KING] & emask);

	if(bitboard[KING | WHITE][eKingCoord / 8][eKingCoord % 8] & kingPos) {
		return true;
	}

	return false;
}

bool BitBoard::inCheck(uint8_t color, uint8_t y, uint8_t x) {
	uint64_t mask, emask;
	uint8_t ecolor;

	if(color == WHITE) {
		mask = white_bit_mask & (UINT64_MAX ^ (figures[KING] & WHITE));
		emask = black_bit_mask & (UINT64_MAX ^ (figures[KING] & BLACK));
		ecolor = BLACK;
	} else {
		mask = black_bit_mask & (UINT64_MAX ^ (figures[KING] & BLACK));
		emask = white_bit_mask & (UINT64_MAX ^ (figures[KING] & WHITE));
		ecolor = WHITE;
	}

	uint64_t kingPos = vec2_cells[y][x];
	uint8_t kingCoord = y * 8 + x;


	uint64_t figure;
	figure = firstOne(plus8[kingCoord] & (mask | emask));
	figure = vec1_cells[figure];
	if(plus8[kingCoord] & (mask | emask) && (figure & (figures[ROOK] | figures[QUEEN]) & emask)) {
		return true;
	}

	figure = firstOne(plus1[kingCoord] & (mask | emask));
	figure = vec1_cells[figure];
	if(plus1[kingCoord] & (mask | emask) && (figure & (figures[ROOK] | figures[QUEEN]) & emask)) {
		return true;
	}

	figure = lastOne(minus8[kingCoord] & (mask | emask));
	figure = vec1_cells[figure];
	if(minus8[kingCoord] & (mask | emask) && (figure & (figures[ROOK] | figures[QUEEN]) & emask)) {
		return true;
	}

	figure = lastOne(minus1[kingCoord] & (mask | emask));
	figure = vec1_cells[figure];
	if(minus1[kingCoord] & (mask | emask) && (figure & (figures[ROOK] | figures[QUEEN]) & emask)) {
		return true;
	}

	figure = firstOne(plus7[kingCoord] & (mask | emask));
	figure = vec1_cells[figure];
	if(plus7[kingCoord] & (mask | emask) && (figure & (figures[BISHOP] | figures[QUEEN]) & emask)) {
		return true;
	}

	figure = firstOne(plus9[kingCoord] & (mask | emask));
	figure = vec1_cells[figure];
	if(plus9[kingCoord] & (mask | emask) && (figure & (figures[BISHOP] | figures[QUEEN]) & emask)) {
		return true;
	}

	figure = lastOne(minus7[kingCoord] & (mask | emask));
	figure = vec1_cells[figure];
	if(minus7[kingCoord] & (mask | emask) && (figure & (figures[BISHOP] | figures[QUEEN]) & emask)) {
		return true;
	}

	figure = lastOne(minus9[kingCoord] & (mask | emask));
	figure = vec1_cells[figure];
	if(minus9[kingCoord] & (mask | emask) && (figure & (figures[BISHOP] | figures[QUEEN]) & emask)) {
		return true;
	}

	if(bitboard[KNIGHT | WHITE][kingCoord / 8][kingCoord % 8] & figures[KNIGHT] & emask) {
		return true;
	}

	if(color == WHITE) {
		if(((figures[PAWN] & emask) >> 9) & (UINT64_MAX ^ vertical[7]) & kingPos) {
			return true;
		}

		if(((figures[PAWN] & emask) >> 7) & (UINT64_MAX ^ vertical[0]) & kingPos) {
			return true;
		}
	} else {
		if(((figures[PAWN] & emask) << 9) & (UINT64_MAX ^ vertical[0]) & kingPos) {
			return true;
		}

		if(((figures[PAWN] & emask) << 7) & (UINT64_MAX ^ vertical[7]) & kingPos) {
			return true;
		}
	}

	uint8_t eKingCoord = firstOne(figures[KING] & emask);

	if(bitboard[KING | WHITE][eKingCoord / 8][eKingCoord % 8] & kingPos) {
		return true;
	}

	return false;
}

bool BitBoard::wsc() {
	return (castlingMap & vec2_cells[0][4]) && (castlingMap & vec2_cells[0][7]);
}

bool BitBoard::wlc() {
	return (castlingMap & vec2_cells[0][4]) && (castlingMap & vec2_cells[0][0]);
}

bool BitBoard::bsc() {
	return (castlingMap & vec2_cells[7][4]) && (castlingMap & vec2_cells[7][7]);
}

bool BitBoard::blc() {
	return (castlingMap & vec2_cells[7][4]) && (castlingMap & vec2_cells[7][0]);
}

void BitBoard::zobristGenerator() {
	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_int_distribution<unsigned long long> dis;

	for(int i = 0; i < 32; ++i) {
		for(int j = 0; j < BOARD_SIZE; ++j) {
			for(int k = 0; k < BOARD_SIZE; ++k) {
				zobrist[i][j][k] = dis(gen);
			}
		}
	}
}

void BitBoard::generateHash() {
	uint64_t figure_positions = (white_bit_mask | black_bit_mask);
	hash = 0;

	while(figure_positions) {
		uint8_t pos = firstOne(figure_positions);
		hash ^= (zobrist[getFigure(pos / 8, pos % 8)][pos / 8][pos % 8]);
		figure_positions &= (UINT64_MAX ^ vec1_cells[pos]);
	}
}

uint64_t BitBoard::getHash() {
	return hash;
}

uint64_t BitBoard::getColorHash() {
	if(!whiteMove) {
		return hash ^ reverse_color_const;
	}

	return hash;
}
