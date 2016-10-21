#include "bitboard.hpp"

BitBoard::BitBoard() : moveNumber(0), ruleNumber(0) {
	preInit();
	clear();
}

BitBoard::~BitBoard() {

}

void BitBoard::setFen(std::string fen) {
	std::vector<std::string> fenArray = splitter(fen);

	for(unsigned int i = 0; i < 64; ++i) {

	}
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

std::vector<std::string> BitBoard::splitter(std::string str) {
	std::vector<std::string> result;
	std::string tmp;
	for(unsigned int i = 0; i < str.size(); ++i) {
		if(str[i] == ' ') {
			result.push_back(tmp);
			tmp.clear();
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
