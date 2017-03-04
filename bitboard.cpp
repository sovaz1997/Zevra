/*
  Zevra, a UCI chess playing engine
  Copyright (C) 2016-2017 Oleg Smirnov (author)
  Zevra is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version.
  Zevra is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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

	whitePassantMade = false;
 	blackPassantMade = false;

	third_repeat = std::vector<int> (pow(2, hash_width), 0);

	hash_enable = true;

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

	//while(!history.empty()) {
		//history.pop_front();
		history_iterator = 0;
	//}

	gameHash.clear();
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
	return __builtin_popcountll(value);
}

uint8_t BitBoard::firstOne(uint64_t mask) {
	if(!mask) { return 64; }
	return __builtin_ctzll(mask);
}

int BitBoard::getAllFiguresWeight() {
	int result = 0;

	result += (popcount64(figures[PAWN])) * PAWN_EV;
	result += (popcount64(figures[KNIGHT])) * KNIGHT_EV;
	result += (popcount64(figures[BISHOP])) * BISHOP_EV;
	result += (popcount64(figures[ROOK])) * ROOK_EV;
	result += (popcount64(figures[QUEEN])) * QUEEN_EV;

	return result;
}

double BitBoard::getPositionStage() {
	return std::min(1.0, (double)getAllFiguresWeight() / (double) ALL_MATERIAL);
}

uint8_t BitBoard::lastOne(uint64_t mask) {
	if(!mask) { return 64; }
	return 63 - __builtin_clzll(mask);
}

void BitBoard::preInit() {
	history = std::vector<GoBack> (10000);
	third_repeat = std::vector<int> (pow(2, hash_width), 0);
	history_iterator = 0;
	
	margin = 0;

	for(unsigned int y1 = 0; y1 < 8; ++y1) {
		for(unsigned int x1 = 0; x1 < 8; ++x1) {
			for(unsigned int y2 = 0; y2 < 8; ++y2) {
				for(unsigned int x2 = 0; x2 < 8; ++x2) {
					distance[y1][x1][y2][x2] = std::sqrt(std::pow(y1 - y2, 2) + std::pow(x1 - x2, 2));
				}	
			}
		}	
	}

	magicConstantsSet();
	zobristGenerator();
	castlingMap = 0;

	for(int i = 0; i < 64; ++i) {
		for(int j = 0; j < 64; ++j) {
			kingSecurityArray[i][j] = king_security[std::max(std::abs(i / 8 - j / 8), std::abs(i % 8 - j % 8))];
		}
	}

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

	magicInit();
}

void BitBoard::magicInit() {
	for(uint8_t y = 0; y < 8; ++y) {
		for(uint8_t x = 0; x < 8; ++x) {
			rookMagicMask[y][x] = 0;

			if(plus1[y * 8 + x]) {
				rookMagicMask[y][x] |= plus1[y * 8 + x];
				rookMagicMask[y][x] &= (UINT64_MAX ^ vertical[7]);
			}
			if(minus1[y * 8 + x]) {
				rookMagicMask[y][x] |= minus1[y * 8 + x];
				rookMagicMask[y][x] &= (UINT64_MAX ^ vertical[0]);
			}
			if(plus8[y * 8 + x]) {
				rookMagicMask[y][x] |= plus8[y * 8 + x];
				rookMagicMask[y][x] &= (UINT64_MAX ^ horizontal[7]);
			}
			if(minus8[y * 8 + x]) {
				rookMagicMask[y][x] |= minus8[y * 8 + x];
				rookMagicMask[y][x] &= (UINT64_MAX ^ horizontal[0]);
			}

			rookMagicMask[y][x] &= (UINT64_MAX ^ vec2_cells[y][x]);
		}
	}

	for(uint8_t y = 0; y < 8; ++y) {
		for(uint8_t x = 0; x < 8; ++x) {
			bishopMagicMask[y][x] = 0;

			if(plus9[y * 8 + x]) {
				bishopMagicMask[y][x] |= plus9[y * 8 + x];
				bishopMagicMask[y][x] &= (UINT64_MAX ^ vertical[7]);
				bishopMagicMask[y][x] &= (UINT64_MAX ^ horizontal[7]);
			}
			if(minus9[y * 8 + x]) {
				bishopMagicMask[y][x] |= minus9[y * 8 + x];
				bishopMagicMask[y][x] &= (UINT64_MAX ^ vertical[0]);
				bishopMagicMask[y][x] &= (UINT64_MAX ^ horizontal[0]);
			}
			if(plus7[y * 8 + x]) {
				bishopMagicMask[y][x] |= plus7[y * 8 + x];
				bishopMagicMask[y][x] &= (UINT64_MAX ^ vertical[0]);
				bishopMagicMask[y][x] &= (UINT64_MAX ^ horizontal[7]);
			}
			if(minus7[y * 8 + x]) {
				bishopMagicMask[y][x] |= minus7[y * 8 + x];
				bishopMagicMask[y][x] &= (UINT64_MAX ^ vertical[7]);
				bishopMagicMask[y][x] &= (UINT64_MAX ^ horizontal[0]);
			}

			bishopMagicMask[y][x] &= (UINT64_MAX ^ vec2_cells[y][x]);
		}
	}

	for(uint8_t y = 0; y < 8; ++y) {
		for(uint8_t x = 0; x < 8; ++x) {
			std::vector<uint64_t> rook_combination = std::vector<uint64_t>((int)std::pow(2, popcount64(rookMagicMask[y][x])));
			for(uint64_t k = 0; k < std::pow(2, popcount64(rookMagicMask[y][x])); ++k) {
				uint64_t tmp_bitboard = rookMagicMask[y][x];
				uint64_t bit_combination = 0;

				for(int m = 0; m < popcount64(rookMagicMask[y][x]); ++m) {
					uint8_t pos = firstOne(tmp_bitboard);
					tmp_bitboard &= (UINT64_MAX ^ vec1_cells[pos]);
					if(vec1_cells[m] & k) {
						bit_combination |= vec1_cells[pos];
					}
				}
				rook_combination[k] = bit_combination;
			}

			std::vector<uint64_t> bishop_combination = std::vector<uint64_t>((int)std::pow(2, popcount64(bishopMagicMask[y][x])));
			for(uint64_t k = 0; k < std::pow(2, popcount64(bishopMagicMask[y][x])); ++k) {
				uint64_t tmp_bitboard = bishopMagicMask[y][x];
				uint64_t bit_combination = 0;

				for(int m = 0; m < popcount64(bishopMagicMask[y][x]); ++m) {
					uint8_t pos = firstOne(tmp_bitboard);
					tmp_bitboard &= (UINT64_MAX ^ vec1_cells[pos]);
					if(vec1_cells[m] & k) {
						bit_combination |= vec1_cells[pos];
					}
				}

				bishop_combination[k] = bit_combination;
			}

			std::vector<uint64_t> rook_result_array(std::pow(2, popcount64(rookMagicMask[y][x])));
			for(int s = 0; s < std::pow(2, popcount64(rookMagicMask[y][x])); ++s) {
				uint64_t rook_result = 0;
				rook_result |= (minus1[y * 8 + x] & (UINT64_MAX ^ minus1[lastOne(minus1[y * 8 + x] & (horizontal[y] & rook_combination[s]))]));
				rook_result |= (minus8[y * 8 + x] & (UINT64_MAX ^ minus8[lastOne(minus8[y * 8 + x] & (vertical[x] & rook_combination[s]))]));
				rook_result |= ( plus1[y * 8 + x] & (UINT64_MAX ^  plus1[firstOne(plus1[y * 8 + x] & (horizontal[y] & rook_combination[s]))]));
				rook_result |= ( plus8[y * 8 + x] & (UINT64_MAX ^  plus8[firstOne(plus8[y * 8 + x] & (vertical[x] & rook_combination[s]))]));
				uint64_t ind = ((rook_combination[s] * ROOK_MAGIC[y][x]) >> (64 - popcount64(rookMagicMask[y][x])));

				rook_result_array[ind] = rook_result;
			}
			rookMagic[y][x] = Magic(rook_result_array, ROOK_MAGIC[y][x], 64 - popcount64(rookMagicMask[y][x]));

			std::vector<uint64_t> bishop_result_array(std::pow(2, popcount64(bishopMagicMask[y][x])));
			for(int s = 0; s < std::pow(2, popcount64(bishopMagicMask[y][x])); ++s) {
				uint64_t bishop_result = 0;
				bishop_result |= (minus7[y * 8 + x] & (UINT64_MAX ^ minus7[lastOne(minus7[y * 8 + x] & ((plus7[y * 8 + x] | minus7[y * 8 + x] | vec2_cells[y][x]) & bishop_combination[s]))]));
				bishop_result |= (minus9[y * 8 + x] & (UINT64_MAX ^ minus9[lastOne(minus9[y * 8 + x] & ((plus9[y * 8 + x] | minus9[y * 8 + x] | vec2_cells[y][x]) & bishop_combination[s]))]));
				bishop_result |= ( plus7[y * 8 + x] & (UINT64_MAX ^  plus7[firstOne(plus7[y * 8 + x] & ((plus7[y * 8 + x] | minus7[y * 8 + x] | vec2_cells[y][x]) & bishop_combination[s]))]));
				bishop_result |= ( plus9[y * 8 + x] & (UINT64_MAX ^  plus9[firstOne(plus9[y * 8 + x] & ((plus9[y * 8 + x] | minus9[y * 8 + x] | vec2_cells[y][x]) & bishop_combination[s]))]));
				uint64_t ind = ((bishop_combination[s] * BISHOP_MAGIC[y][x]) >> (64 - popcount64(bishopMagicMask[y][x])));
				bishop_result_array[ind] = bishop_result;
			}
			bishopMagic[y][x] = Magic(bishop_result_array, BISHOP_MAGIC[y][x], 64 - popcount64(bishopMagicMask[y][x]));
		}
	}
}

int BitBoard::getFiguresCount() {
	return popcount64(white_bit_mask | black_bit_mask);
}

void BitBoard::bitBoardMoveGenerator(MoveArray& moveArray) {
	moveArray.clear();
	bitBoardAttackMoveGenerator(moveArray);
	uint64_t possibleMoves, mask, emask;

	uint8_t color;
	if(whiteMove) {
		color = WHITE;
		mask = white_bit_mask;
		emask = black_bit_mask;
	} else {
		color = BLACK;
		mask = black_bit_mask;
		emask = white_bit_mask;
	}

	//Ладьи
	uint64_t rook = figures[ROOK] & mask;
	while(rook != 0) {
		uint8_t pos = firstOne(rook);

		possibleMoves = rookMagic[pos / 8][pos % 8].getPossibleMoves(rookMagicMask[pos / 8][pos % 8] & (white_bit_mask | black_bit_mask) & (UINT64_MAX ^ vec1_cells[pos])) & ((figures[KING] ^ emask) ^ UINT64_MAX) & (UINT64_MAX ^ emask) & (mask ^ UINT64_MAX);

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
		uint8_t pos = firstOne(bishop);

		possibleMoves = bishopMagic[pos / 8][pos % 8].getPossibleMoves(bishopMagicMask[pos / 8][pos % 8] & (white_bit_mask | black_bit_mask) & (UINT64_MAX ^ vec1_cells[pos])) & ((figures[KING] ^ emask) ^ UINT64_MAX) & (UINT64_MAX ^ emask) & (mask ^ UINT64_MAX);
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
		uint8_t pos = firstOne(queen);

		possibleMoves = rookMagic[pos / 8][pos % 8].getPossibleMoves(rookMagicMask[pos / 8][pos % 8] & (white_bit_mask | black_bit_mask) & (UINT64_MAX ^ vec1_cells[pos])) & ((figures[KING] ^ emask) ^ UINT64_MAX) & (UINT64_MAX ^ emask) & (mask ^ UINT64_MAX);
		possibleMoves |= (bishopMagic[pos / 8][pos % 8].getPossibleMoves(bishopMagicMask[pos / 8][pos % 8] & (white_bit_mask | black_bit_mask) & (UINT64_MAX ^ vec1_cells[pos])) & ((figures[KING] ^ emask) ^ UINT64_MAX) & (UINT64_MAX ^ emask) & (mask ^ UINT64_MAX));

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

	uint8_t color;
	if(whiteMove) {
		color = WHITE;
		mask = white_bit_mask;
		emask = black_bit_mask;
	} else {
		color = BLACK;
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
		possibleMoves = bishopMagic[pos / 8][pos % 8].getPossibleMoves(bishopMagicMask[pos / 8][pos % 8] & (white_bit_mask | black_bit_mask) & (UINT64_MAX ^ vec1_cells[pos])) & ((figures[KING] & emask) ^ UINT64_MAX) & emask & (mask ^ UINT64_MAX);
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

		possibleMoves = rookMagic[pos / 8][pos % 8].getPossibleMoves(rookMagicMask[pos / 8][pos % 8] & (white_bit_mask | black_bit_mask) & (UINT64_MAX ^ vec1_cells[pos])) & ((figures[KING] & emask) ^ UINT64_MAX) & emask & (mask ^ UINT64_MAX);
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
		possibleMoves = rookMagic[pos / 8][pos % 8].getPossibleMoves(rookMagicMask[pos / 8][pos % 8] & (white_bit_mask | black_bit_mask) & (UINT64_MAX ^ vec1_cells[pos])) & ((figures[KING] & emask) ^ UINT64_MAX) & emask & (mask ^ UINT64_MAX);
		possibleMoves |= (bishopMagic[pos / 8][pos % 8].getPossibleMoves(bishopMagicMask[pos / 8][pos % 8] & (white_bit_mask | black_bit_mask) & (UINT64_MAX ^ vec1_cells[pos])) & ((figures[KING] & emask) ^ UINT64_MAX) & emask & (mask ^ UINT64_MAX));
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
	attacked = false;

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
				whitePassantMade = true;
			} else if(mv.fromY == 0 && mv.fromX == 4 && mv.toY == 0 && mv.toX == 2) {
				addFigure(ROOK | WHITE, 0, 3);
				clearCell(0, 0);
				whitePassantMade = true;
			}
		} else {
			if(mv.fromY == 7 && mv.fromX == 4 && mv.toY == 7 && mv.toX == 6) {
				addFigure(ROOK | BLACK, 7, 5);
				clearCell(7, 7);
				blackPassantMade = true;
			} else if(mv.fromY == 7 && mv.fromX == 4 && mv.toY == 7 && mv.toX == 2) {
				addFigure(ROOK | BLACK, 7, 3);
				clearCell(7, 0);
				blackPassantMade = true;
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

	if(mv.isAttack) {
		attacked = true;
	}

	castlingMap &= (figures[KING] | figures[ROOK]);

	++third_repeat[getColorHash() & hash_cutter];

	if(third_repeat[getColorHash() & hash_cutter] >= 3) {
		hash_enable = false;
	}
}

void BitBoard::goBack() {
	--history_iterator;
	if(history_iterator >= 0) {
		--third_repeat[getColorHash() & hash_cutter];
		for(unsigned int i = 0; i < 7; ++i) {
			figures[i] = history[history_iterator].figures[i];
		}

		white_bit_mask = history[history_iterator].white_bit_mask;
		black_bit_mask = history[history_iterator].black_bit_mask;
		moveNumber = history[history_iterator].moveNumber;
		ruleNumber = history[history_iterator].ruleNumber;
		castlingMap = history[history_iterator].castlingMap;
		whiteMove = history[history_iterator].whiteMove;
		evalute = history[history_iterator].evalute;
		passant_y = history[history_iterator].passant_y;
		passant_x = history[history_iterator].passant_x;
		passant_enable = history[history_iterator].passant_enable;
		hash = history[history_iterator].hash;
		hash_enable = history[history_iterator].hash_enable;
		attacked = history[history_iterator].attacked;
		margin = history[history_iterator].margin;
		whitePassantMade = history[history_iterator].whitePassantMade;
 		blackPassantMade = history[history_iterator].blackPassantMade;
	}
}


void BitBoard::clearCell(uint8_t y, uint8_t x) {
	if(!(vec2_cells[y][x] & (white_bit_mask | black_bit_mask))) {
		return;
	}

	uint8_t figure = getFigure(y, x);

	if(vec2_cells[y][x] & (white_bit_mask | black_bit_mask)) {
		hash ^= zobrist[figure][y][x];
	}

	if(figure) {
		if((getFigure(y, x) & TYPE_SAVE) != 0) {
			figures[getFigure(y, x) & TYPE_SAVE] &= (UINT64_MAX ^ vec2_cells[y][x]);
		}

		white_bit_mask &= (UINT64_MAX ^ vec2_cells[y][x]);
		black_bit_mask &= (UINT64_MAX ^ vec2_cells[y][x]);
	}
}

void BitBoard::addFigure(uint8_t figure, uint8_t y, uint8_t x) {
	uint8_t color = (figure & COLOR_SAVE);
	uint8_t originalFigure = figure;

	if(figure) {
		figures[figure & TYPE_SAVE] |= vec2_cells[y][x];

		if(color == WHITE) {
			white_bit_mask |= vec2_cells[y][x];
		} else if(color == BLACK) {
			black_bit_mask |= vec2_cells[y][x];
		}
	}

	hash ^= zobrist[originalFigure][y][x];
}

void BitBoard::printBitBoard(uint64_t bit_board) {
	uint64_t doubler = 1;
	for(int y = 7; y >= 0; --y) {
		for(int x = 0; x < BOARD_SIZE; ++x) {
			std::cout << (bool)((doubler << (y * 8 + x)) & bit_board);
		}
		std::cout << std::endl;
	}
}

void BitBoard::pushHistory() {
	for(unsigned int i = 0; i < 7; ++i) {
		history[history_iterator].figures[i] = figures[i];
	}

	history[history_iterator].white_bit_mask = white_bit_mask;
	history[history_iterator].black_bit_mask = black_bit_mask;
	history[history_iterator].moveNumber = moveNumber;
	history[history_iterator].ruleNumber = ruleNumber;
	history[history_iterator].whiteMove = whiteMove;
	history[history_iterator].evalute = evalute;
	history[history_iterator].castlingMap = castlingMap;
	history[history_iterator].passant_enable = passant_enable;
	history[history_iterator].passant_x = passant_x;
	history[history_iterator].passant_y = passant_y;
	history[history_iterator].hash = hash;
	history[history_iterator].hash_enable = hash_enable;
	history[history_iterator].attacked = attacked;
	history[history_iterator].margin = margin;
	history[history_iterator].whitePassantMade = whitePassantMade;
 	history[history_iterator].blackPassantMade = blackPassantMade;
	++history_iterator;
}

void BitBoard::evaluteAll() {
	double positionStage = getPositionStage();

	evalute = 0;
	evalute += popcount64(figures[PAWN] & white_bit_mask) * (positionStage * PAWN_EV + (1 - positionStage) * ENDGAME_PAWN_EV);
	evalute -= popcount64(figures[PAWN] & black_bit_mask) * (positionStage * PAWN_EV + (1 - positionStage) * ENDGAME_PAWN_EV);
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

	mask = figures[KING] & white_bit_mask;
	while(mask != 0) {
		uint8_t pos = firstOne(mask);
		evalute += ((positionStage * kingDebuteMatr[7 - pos / 8][pos % 8]) + ((1 - positionStage) * kingEndGameMatr[7 - pos / 8][pos % 8]));
		mask &= (UINT64_MAX ^ vec1_cells[pos]);

		evalute += (positionStage * popcount64(bitboard[KING | WHITE][pos / 8][pos % 8] & figures[PAWN] & white_bit_mask) * 3);
	}

	mask = figures[KING] & black_bit_mask;
	while(mask != 0) {
		uint8_t pos = firstOne(mask);
		evalute -= ((positionStage * kingDebuteMatr[pos / 8][pos % 8]) + ((1 - positionStage) * kingEndGameMatr[pos / 8][pos % 8]));
		mask &= (UINT64_MAX ^ vec1_cells[pos]);

		evalute -= (positionStage * popcount64(bitboard[KING | BLACK][pos / 8][pos % 8] & figures[PAWN] & black_bit_mask) * 3);
	}

	if(whitePassantMade) {
 		evalute += 50;
 	}
 	if(blackPassantMade) {
 		evalute -= 50;
 	}

	 //преимущество 2 слонов
	if(popcount64(figures[BISHOP] & white_bit_mask) >= 2) {
		evalute += 30;
	}
	if(popcount64(figures[BISHOP] & black_bit_mask) >= 2) {
		evalute -= 30;
	}

	//uint8_t white_king_pos = firstOne(figures[KING] & white_bit_mask);
	//uint8_t black_king_pos = firstOne(figures[KING] & black_bit_mask);

	/*mask = figures[KNIGHT] & white_bit_mask;
	while(mask != 0) {
		uint8_t pos = firstOne(mask);
		evalute += king_security[distance[pos / 8][pos % 8][black_king_pos / 8][black_king_pos % 8]];
		mask &= (UINT64_MAX ^ vec1_cells[pos]);
	}

	
	mask = figures[KNIGHT] & black_bit_mask;
	while(mask != 0) {
		uint8_t pos = firstOne(mask);
		evalute -= king_security[distance[pos / 8][pos % 8][white_king_pos / 8][white_king_pos % 8]];
		mask &= (UINT64_MAX ^ vec1_cells[pos]);
	}*/

	/*mask = figures[KNIGHT] & white_bit_mask;
	while(mask != 0) {
		uint8_t pos = firstOne(mask);
		evalute += king_security[distance[pos / 8][pos % 8][white_king_pos / 8][white_king_pos % 8]];
		mask &= (UINT64_MAX ^ vec1_cells[pos]);
	}

	mask = figures[KNIGHT] & black_bit_mask;
	while(mask != 0) {
		uint8_t pos = firstOne(mask);
		evalute -= king_security[distance[pos / 8][pos % 8][black_king_pos / 8][black_king_pos % 8]];
		mask &= (UINT64_MAX ^ vec1_cells[pos]);
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

BitMove BitBoard::getRandomMove() {
	MoveArray moveArray;
	bitBoardMoveGenerator(moveArray);

	uint8_t color;

	if(whiteMove) {
		color = WHITE;
	} else {
		color = BLACK;
	}

	BitMove res;

	for(unsigned int i = 0; i < moveArray.count; ++i) {
		move(moveArray.moveArray[i]);
		if(!inCheck(color)) {
			goBack();
			res = moveArray.moveArray[i];
			break;
		}

		goBack();
	}

	return res;
}

int64_t BitBoard::getEvalute() {
	if(!hash_enable || ruleNumber >= 100) { return 0; }
	evaluteAll();

	if(whiteMove) {
		return evalute;
	} else {
		return -evalute;
	}
}

bool BitBoard::inCheck(uint8_t color) {
	uint64_t mask, emask;

	if(color == WHITE) {
		mask = white_bit_mask;
		emask = black_bit_mask;
	} else {
		mask = black_bit_mask;
		emask = white_bit_mask;
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

	if(color == WHITE) {
		mask = white_bit_mask & (UINT64_MAX ^ (figures[KING] & WHITE));
		emask = black_bit_mask & (UINT64_MAX ^ (figures[KING] & BLACK));
	} else {
		mask = black_bit_mask & (UINT64_MAX ^ (figures[KING] & BLACK));
		emask = white_bit_mask & (UINT64_MAX ^ (figures[KING] & WHITE));
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
				/*for(unsigned int s = 0; s < 64; ++s) {
					int m = rand() % 2;
					zobrist[i][j][k] += m * std::pow(2, s);//dis(gen);
				}*/
				zobrist[i][j][k] = dis(gen);
				//std::cout << zobrist[i][j][k] << std::endl;
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
		return (hash ^ reverse_color_const);
	}

	return hash;
}

void BitBoard::magicNumberGenerator() {
	for(uint8_t y = 0; y < 8; ++y) {
		for(uint8_t x = 0; x < 8; ++x) {
			std::vector<uint64_t> rook_combination = std::vector<uint64_t>((int)std::pow(2, popcount64(rookMagicMask[y][x])));
			for(uint64_t k = 0; k < std::pow(2, popcount64(rookMagicMask[y][x])); ++k) {
				uint64_t tmp_bitboard = rookMagicMask[y][x];
				uint64_t bit_combination = 0;

				for(int m = 0; m < popcount64(rookMagicMask[y][x]); ++m) {
					uint8_t pos = firstOne(tmp_bitboard);
					tmp_bitboard &= (UINT64_MAX ^ vec1_cells[pos]);
					if(vec1_cells[m] & k) {
						bit_combination |= vec1_cells[pos];
					}
				}
				rook_combination[k] = bit_combination;
				printBitBoard(rook_combination[k]);
				std::cout << std::endl;
			}

			bool stopped = false;

			uint64_t magic;
			while(!stopped) {
				magic = magicGenerator();//dis(gen);
				std::vector<int> check(rook_combination.size(), 0);

				for(unsigned int i = 0; i < rook_combination.size(); ++i) {
					uint64_t index = (rook_combination[i] * magic) >> (64 - popcount64(rookMagicMask[y][x]));

					if(check[index]) {
						break;
					}

					++check[index];

					if(i == rook_combination.size() - 1) {
						stopped = true;
						std::cout << "ROOK_MAGIC[" << (int)y << "][" << (int)x << "] = " << (unsigned long long) magic << "ULL;" << std::endl;
					}
				}
			}
		}
	}

	for(uint8_t y = 0; y < 8; ++y) {
		for(uint8_t x = 0; x < 8; ++x) {
			std::vector<uint64_t> bishop_combination = std::vector<uint64_t>((int)std::pow(2, popcount64(bishopMagicMask[y][x])));
			for(uint64_t k = 0; k < std::pow(2, popcount64(bishopMagicMask[y][x])); ++k) {
				uint64_t tmp_bitboard = bishopMagicMask[y][x];
				uint64_t bit_combination = 0;

				for(int m = 0; m < popcount64(bishopMagicMask[y][x]); ++m) {
					uint8_t pos = firstOne(tmp_bitboard);
					tmp_bitboard &= (UINT64_MAX ^ vec1_cells[pos]);
					if(vec1_cells[m] & k) {
						bit_combination |= vec1_cells[pos];
					}
				}

				bishop_combination[k] = bit_combination;
			}

			bool stopped = false;

			uint64_t magic;
			while(!stopped) {
				magic = magicGenerator();
				std::vector<int> check(bishop_combination.size(), 0);

				for(unsigned int i = 0; i < bishop_combination.size(); ++i) {
					uint64_t index = (bishop_combination[i] * magic) >> (64 - popcount64(bishopMagicMask[y][x]));

					if(check[index]) {
						break;
					}

					++check[index];

					if(i == bishop_combination.size() - 1) {
						stopped = true;
						std::cout << "BISHOP_MAGIC[" << (int)y << "][" << (int)x << "] = " << (unsigned long long) magic << "ULL;" << std::endl;
					}
				}
			}
		}
	}
}

bool BitBoard::testOfDraw() {
	return (gameHash.count(getHash()) >= 3);
}

uint64_t BitBoard::magicGenerator() {
	uint64_t result = 0;

	int i = 0;

	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_int_distribution<unsigned long long> dis;
	while(i < 7) {
		uint64_t pos = dis(gen) % 64;

		if(!(result & vec1_cells[pos])) {
			result |= vec1_cells[pos];
			++i;
		}
	}

	return result;
}

void BitBoard::makeNullMove() {
	if(!in_null_move) {
		whiteMove = !whiteMove;
		hash ^= 747489434796739468;
		in_null_move = true;
	}
}

void BitBoard::unMakeNullMove() {
	if(in_null_move) {
		whiteMove = !whiteMove;
		hash ^= 747489434796739468;
		in_null_move = false;
	}
}

void BitBoard::magicConstantsSet() {
	ROOK_MAGIC[0][0] = 36028940099256448ULL;
	ROOK_MAGIC[0][1] = 1170936178531373060ULL;
	ROOK_MAGIC[0][2] = 36046389741890176ULL;
	ROOK_MAGIC[0][3] = 1188967895993417732ULL;
	ROOK_MAGIC[0][4] = 1297054319295725600ULL;
	ROOK_MAGIC[0][5] = 9367491691713527816ULL;
	ROOK_MAGIC[0][6] = 4791840998646874368ULL;
	ROOK_MAGIC[0][7] = 9295430181724422146ULL;
	ROOK_MAGIC[1][0] = 2392539451899906ULL;
	ROOK_MAGIC[1][1] = 2341942243698409664ULL;
	ROOK_MAGIC[1][2] = 11399805301432320ULL;
	ROOK_MAGIC[1][3] = 9232520007966068736ULL;
	ROOK_MAGIC[1][4] = 649785533559474176ULL;
	ROOK_MAGIC[1][5] = 1441714839302965252ULL;
	ROOK_MAGIC[1][6] = 281827164293376ULL;
	ROOK_MAGIC[1][7] = 36732502722740480ULL;
	ROOK_MAGIC[2][0] = 18023744903578176ULL;
	ROOK_MAGIC[2][1] = 4503874652086274ULL;
	ROOK_MAGIC[2][2] = 212207354777856ULL;
	ROOK_MAGIC[2][3] = 4645986651621376ULL;
	ROOK_MAGIC[2][4] = 565149245703200ULL;
	ROOK_MAGIC[2][5] = 4611827307315725312ULL;
	ROOK_MAGIC[2][6] = 576465150635280400ULL;
	ROOK_MAGIC[2][7] = 2306408158207180868ULL;
	ROOK_MAGIC[3][0] = 2377970974143316016ULL;
	ROOK_MAGIC[3][1] = 36239905401081984ULL;
	ROOK_MAGIC[3][2] = 563027264946312ULL;
	ROOK_MAGIC[3][3] = 41675890895095808ULL;
	ROOK_MAGIC[3][4] = 9156734983864449ULL;
	ROOK_MAGIC[3][5] = 563233422575632ULL;
	ROOK_MAGIC[3][6] = 36037610296119298ULL;
	ROOK_MAGIC[3][7] = 4406637658177ULL;
	ROOK_MAGIC[4][0] = 54043745829519936ULL;
	ROOK_MAGIC[4][1] = 144150399295688704ULL;
	ROOK_MAGIC[4][2] = 81065360773615616ULL;
	ROOK_MAGIC[4][3] = 2310346647512551456ULL;
	ROOK_MAGIC[4][4] = 144194359363896320ULL;
	ROOK_MAGIC[4][5] = 4611721220113645584ULL;
	ROOK_MAGIC[4][6] = 1153202996847314432ULL;
	ROOK_MAGIC[4][7] = 2308095363111714884ULL;
	ROOK_MAGIC[5][0] = 288652863631032320ULL;
	ROOK_MAGIC[5][1] = 633331590955040ULL;
	ROOK_MAGIC[5][2] = 2310347159133782064ULL;
	ROOK_MAGIC[5][3] = 9288708859691044ULL;
	ROOK_MAGIC[5][4] = 54192729176965248ULL;
	ROOK_MAGIC[5][5] = 13835621022415618176ULL;
	ROOK_MAGIC[5][6] = 55044435345409ULL;
	ROOK_MAGIC[5][7] = 292066820105ULL;
	ROOK_MAGIC[6][0] = 324329552652279936ULL;
	ROOK_MAGIC[6][1] = 71476854661632ULL;
	ROOK_MAGIC[6][2] = 35236986552576ULL;
	ROOK_MAGIC[6][3] = 158330345783424ULL;
	ROOK_MAGIC[6][4] = 4611708008794259584ULL;
	ROOK_MAGIC[6][5] = 288250167965024384ULL;
	ROOK_MAGIC[6][6] = 72339077604838656ULL;
	ROOK_MAGIC[6][7] = 576601491150737536;
	ROOK_MAGIC[7][0] = 140738026340419ULL;
	ROOK_MAGIC[7][1] = 4785349483044993ULL;
	ROOK_MAGIC[7][2] = 44259639042049ULL;
	ROOK_MAGIC[7][3] = 72092847129757953ULL;
	ROOK_MAGIC[7][4] = 563018742106118ULL;
	ROOK_MAGIC[7][5] = 4611967527931871233ULL;
	ROOK_MAGIC[7][6] = 18016666260670468ULL;
	ROOK_MAGIC[7][7] = 36033195606573314ULL;

	BISHOP_MAGIC[0][0] = 9227880103252345088ULL;
	BISHOP_MAGIC[0][1] = 642149158781056ULL;
	BISHOP_MAGIC[0][2] = 1130590048886784ULL;
	BISHOP_MAGIC[0][3] = 4612816318796661312ULL;
	BISHOP_MAGIC[0][4] = 299342124548116ULL;
	BISHOP_MAGIC[0][5] = 4900199106774827012ULL;
	BISHOP_MAGIC[0][6] = 72621162618159104ULL;
	BISHOP_MAGIC[0][7] = 282026947199232ULL;
	BISHOP_MAGIC[1][0] = 580968784370861056ULL;
	BISHOP_MAGIC[1][1] = 8867102621952ULL;
	BISHOP_MAGIC[1][2] = 74779709276164ULL;
	BISHOP_MAGIC[1][3] = 4508014862157825ULL;
	BISHOP_MAGIC[1][4] = 171137955145449472ULL;
	BISHOP_MAGIC[1][5] = 144186115455188992ULL;
	BISHOP_MAGIC[1][6] = 2201475366912ULL;
	BISHOP_MAGIC[1][7] = 141872467976ULL;
	BISHOP_MAGIC[2][0] = 4503737134482050ULL;
	BISHOP_MAGIC[2][1] = 1266706186502208ULL;
	BISHOP_MAGIC[2][2] = 38280648506478608ULL;
	BISHOP_MAGIC[2][3] = 9225623976288501760ULL;
	BISHOP_MAGIC[2][4] = 1154047413122566144ULL;
	BISHOP_MAGIC[2][5] = 562953191464962ULL;
	BISHOP_MAGIC[2][6] = 70385992471552ULL;
	BISHOP_MAGIC[2][7] = 2305878198216425984ULL;
	BISHOP_MAGIC[3][0] = 9042383896051976ULL;
	BISHOP_MAGIC[3][1] = 13528391202439426ULL;
	BISHOP_MAGIC[3][2] = 282574765687297ULL;
	BISHOP_MAGIC[3][3] = 18019071635292192ULL;
	BISHOP_MAGIC[3][4] = 285873056801025ULL;
	BISHOP_MAGIC[3][5] = 1157434999841227008ULL;
	BISHOP_MAGIC[3][6] = 2308165727531828224ULL;
	BISHOP_MAGIC[3][7] = 1126174801854608ULL;
	BISHOP_MAGIC[4][0] = 569615778324480ULL;
	BISHOP_MAGIC[4][1] = 72357795073101824ULL;
	BISHOP_MAGIC[4][2] = 37417822259204ULL;
	BISHOP_MAGIC[4][3] = 9228439136728384000ULL;
	BISHOP_MAGIC[4][4] = 18016606122697344ULL;
	BISHOP_MAGIC[4][5] = 2306689641757050880ULL;
	BISHOP_MAGIC[4][6] = 571754637574656ULL;
	BISHOP_MAGIC[4][7] = 36593947077780480ULL;
	BISHOP_MAGIC[5][0] = 1130585850396672ULL;
	BISHOP_MAGIC[5][1] = 9224511139625304576ULL;
	BISHOP_MAGIC[5][2] = 1153062413910673409ULL;
	BISHOP_MAGIC[5][3] = 2305845491738353696ULL;
	BISHOP_MAGIC[5][4] = 1161101794345152ULL;
	BISHOP_MAGIC[5][5] = 1127033811829000ULL;
	BISHOP_MAGIC[5][6] = 2254290962874432ULL;
	BISHOP_MAGIC[5][7] = 5630607669329956ULL;
	BISHOP_MAGIC[6][0] = 1130302383063112ULL;
	BISHOP_MAGIC[6][1] = 4611827048009171072ULL;
	BISHOP_MAGIC[6][2] = 4503788640010248ULL;
	BISHOP_MAGIC[6][3] = 576504769343127552ULL;
	BISHOP_MAGIC[6][4] = 2305843284268941312ULL;
	BISHOP_MAGIC[6][5] = 6631702200576ULL;
	BISHOP_MAGIC[6][6] = 18017731974561792ULL;
	BISHOP_MAGIC[6][7] = 9223952596190904448ULL;
	BISHOP_MAGIC[7][0] = 72602194379776ULL;
	BISHOP_MAGIC[7][1] = 563092358956032ULL;
	BISHOP_MAGIC[7][2] = 35330419886080ULL;
	BISHOP_MAGIC[7][3] = 108122674944934400ULL;
	BISHOP_MAGIC[7][4] = 35185179628032ULL;
	BISHOP_MAGIC[7][5] = 35201820791296ULL;
	BISHOP_MAGIC[7][6] = 9012215810621696ULL;
	BISHOP_MAGIC[7][7] = 1706510799372320ULL;
}
