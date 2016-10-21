#include "bitboard.hpp"

BitBoard::BitBoard() : moveNumber(0), ruleNumber(0) {
	preInit();
	clear();
}

BitBoard::~BitBoard() {

}

void BitBoard::setFen(std::string fen) {
	clear();

	std::vector<std::string> fenArray = splitter(fen, ' ');
	std::vector<std::string> notation = splitter(fenArray[0], '/');

	std::reverse(notation.begin(), notation.end());

	int pointer = 0;

	for(unsigned int y = 0; y < 8; ++y) {
		int x = 0;
		for(int i = 0; i < notation[y].size(); ++i) {
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
			wsc = true;
		} else if(fenArray[2][i] == 'Q') {
			wlc = true;
		} else if(fenArray[2][i] == 'k') {
			bsc = true;
		} else if(fenArray[2][i] == 'q') {
			blc = true;
		}
	}

	if(fenArray[3] != "-") {
		uint8_t x = (fenArray[3][0] - 'a');
		uint8_t y = (fenArray[3][1] - '1');

		passant_enable = true;
		passant_x = x;
		passant_y = y;
	}

	moveNumber = std::stoi(fenArray[4]);
	ruleNumber = std::stoi(fenArray[5]);
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

	if(!wsc && !wlc && !bsc && !blc) {
		res.push_back('-');
	} else {
		if(wsc) {
			res.push_back('K');
		}
		if(wlc) {
			res.push_back('Q');
		}
		if(bsc) {
			res.push_back('k');
		}
		if(blc) {
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
	res += std::to_string(moveNumber);
	res.push_back(' ');
	res += std::to_string(ruleNumber);

	return res;
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
	passant_enable = false;
	whiteMove = true;
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
	return __builtin_ctzll(mask);
}

uint8_t BitBoard::lastOne(uint64_t mask) {
	return 63 - __builtin_clzll(mask);
}

void BitBoard::preInit() {
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

	for(int i = 0; i < 64; ++i) {
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
}
