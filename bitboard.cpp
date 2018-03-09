#include "bitboard.hpp"

BitBoard::BitBoard() {
	currentState.moveNumber = 0;
	currentState.ruleNumber = 0;
	preInit();
	setFen(startpos_fen);
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
				currentState.figures[KING] |= vec2_cells[y][x];
				currentState.piece_bit_mask[whiteSide] |= vec2_cells[y][x];
				++x;
			} else if(notation[y][i] == 'Q') {
				currentState.figures[QUEEN] |= vec2_cells[y][x];
				currentState.piece_bit_mask[whiteSide] |= vec2_cells[y][x];
				++x;
			} else if(notation[y][i] == 'R') {
				currentState.figures[ROOK] |= vec2_cells[y][x];
				currentState.piece_bit_mask[whiteSide] |= vec2_cells[y][x];
				++x;
			} else if(notation[y][i] == 'B') {
				currentState.figures[BISHOP] |= vec2_cells[y][x];
				currentState.piece_bit_mask[whiteSide] |= vec2_cells[y][x];
				++x;
			} else if(notation[y][i] == 'N') {
				currentState.figures[KNIGHT] |= vec2_cells[y][x];
				currentState.piece_bit_mask[whiteSide] |= vec2_cells[y][x];
				++x;
			} else if(notation[y][i] == 'P') {
				currentState.figures[PAWN] |= vec2_cells[y][x];
				currentState.piece_bit_mask[whiteSide] |= vec2_cells[y][x];
				++x;
			} else if(notation[y][i] == 'k') {
				currentState.figures[KING] |= vec2_cells[y][x];
				currentState.piece_bit_mask[!whiteSide] |= vec2_cells[y][x];
				++x;
			} else if(notation[y][i] == 'q') {
				currentState.figures[QUEEN] |= vec2_cells[y][x];
				currentState.piece_bit_mask[!whiteSide] |= vec2_cells[y][x];
				++x;
			} else if(notation[y][i] == 'r') {
				currentState.figures[ROOK] |= vec2_cells[y][x];
				currentState.piece_bit_mask[!whiteSide] |= vec2_cells[y][x];
				++x;
			} else if(notation[y][i] == 'b') {
				currentState.figures[BISHOP] |= vec2_cells[y][x];
				currentState.piece_bit_mask[!whiteSide] |= vec2_cells[y][x];
				++x;
			} else if(notation[y][i] == 'n') {
				currentState.figures[KNIGHT] |= vec2_cells[y][x];
				currentState.piece_bit_mask[!whiteSide] |= vec2_cells[y][x];
				++x;
			} else if(notation[y][i] == 'p') {
				currentState.figures[PAWN] |= vec2_cells[y][x];
				currentState.piece_bit_mask[!whiteSide] |= vec2_cells[y][x];
				++x;
			} else if(notation[y][i] >= '0' && notation[y][i] <= '9') {
				x += (notation[y][i] - '0');
			}
		}
	}

	if(fenArray[1] == "w") {
		currentState.color = true;
	} else {
		currentState.color = false;
	}

	for(unsigned int i = 0; i < fenArray[2].size() && fenArray[2] != "-"; ++i) {
		if(fenArray[2][i] == 'K') {
			currentState.castlingMap |= (vec2_cells[0][4] | vec2_cells[0][7]);
		} else if(fenArray[2][i] == 'Q') {
			currentState.castlingMap |= (vec2_cells[0][4] | vec2_cells[0][0]);
		} else if(fenArray[2][i] == 'k') {
			currentState.castlingMap |= (vec2_cells[7][4] | vec2_cells[7][7]);
		} else if(fenArray[2][i] == 'q') {
			currentState.castlingMap |= (vec2_cells[7][4] | vec2_cells[7][0]);
		}
	}

	if(fenArray[3] != "-") {
		uint8_t x = (fenArray[3][0] - 'a');
		uint8_t y = (fenArray[3][1] - '1');

		currentState.passant_enable = true;
		currentState.passant_x = x;
		currentState.passant_y = y;
	}

	currentState.ruleNumber = std::stoi(fenArray[4]);
	currentState.moveNumber = std::stoi(fenArray[5]);

	generateHash();

	++third_repeat[getColorHash() & hash_cutter];
}

std::string BitBoard::getFen() {
	std::string res;

	for(int y = 7; y >= 0; --y) {
		int space = 0;
		for(int x = 0; x < 8; ++x) {
			if(currentState.piece_bit_mask[whiteSide] & vec2_cells[y][x]) {
				if(space != 0) {
					res.push_back('0' + space);
					space = 0;
				}

				if(currentState.figures[KING] & vec2_cells[y][x]) {
					res.push_back('K');
				} else if(currentState.figures[QUEEN] & vec2_cells[y][x]) {
					res.push_back('Q');
				} else if(currentState.figures[ROOK] & vec2_cells[y][x]) {
					res.push_back('R');
				} else if(currentState.figures[BISHOP] & vec2_cells[y][x]) {
					res.push_back('B');
				} else if(currentState.figures[KNIGHT] & vec2_cells[y][x]) {
					res.push_back('N');
				} else if(currentState.figures[PAWN] & vec2_cells[y][x]) {
					res.push_back('P');
				}
			} else if(currentState.piece_bit_mask[!whiteSide] & vec2_cells[y][x]) {
				if(space != 0) {
					res.push_back('0' + space);
					space = 0;
				}

				if(currentState.figures[KING] & vec2_cells[y][x]) {
					res.push_back('k');
				} else if(currentState.figures[QUEEN] & vec2_cells[y][x]) {
					res.push_back('q');
				} else if(currentState.figures[ROOK] & vec2_cells[y][x]) {
					res.push_back('r');
				} else if(currentState.figures[BISHOP] & vec2_cells[y][x]) {
					res.push_back('b');
				} else if(currentState.figures[KNIGHT] & vec2_cells[y][x]) {
					res.push_back('n');
				} else if(currentState.figures[PAWN] & vec2_cells[y][x]) {
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

	if(currentState.color) {
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

	if(currentState.passant_enable) {
		res.push_back(currentState.passant_x + 'a');
		res.push_back(currentState.passant_y + '1');
	} else {
		res.push_back('-');
	}


	res.push_back(' ');
	res += std::to_string(currentState.ruleNumber);
	res.push_back(' ');
	res += std::to_string(currentState.moveNumber);

	return res;
}

void BitBoard::clear() {
	currentState.hash = 0;
	currentState.recapture_position = 0;

	currentState.whitePassantMade = false;
	currentState.blackPassantMade = false;

	third_repeat = std::vector<int> (std::pow(2, hash_width), 0);

	currentState.hash_enable = true;

	for(unsigned int i = 0; i < 7; ++i) {
		currentState.figures[i] = 0;
	}

	currentState.piece_bit_mask[whiteSide] = 0;
	currentState.piece_bit_mask[!whiteSide] = 0;
	currentState.moveNumber = 0;
	currentState.ruleNumber = 0;
	currentState.passant_enable = false;
	currentState.color = true;
	currentState.castlingMap = 0;

	history_iterator = 0;

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
	return !mask ? 64 : __builtin_ctzll(mask);
}

uint8_t BitBoard::lastOne(uint64_t mask) {
	return !mask ? 64 : (63 - __builtin_clzll(mask));
}

void BitBoard::preInit() {
	history = std::vector<GoBack> (10000);
	third_repeat = std::vector<int> (pow(2, hash_width), 0);
	history_iterator = 0;
	
	currentState.margin = 0;

	magicConstantsSet();
	zobristGenerator();
	currentState.castlingMap = 0;

	for(int i = 0; i < 64; ++i) {
		for(int j = 0; j < 64; ++j) {
			kingSecurityArray[i][j] = king_security[std::max(std::abs(i / 8 - j / 8), std::abs(i % 8 - j % 8))];
		}
	}

	for(int i = 0; i < 64; ++i) {
		int y_pos = i / 8;
		int x_pos = i % 8;
		for(int y = 0; y < 8; ++y) {
			for(int x = 0; x < 8; ++x) {
				metric[i][y][x] = std::max(std::abs(y_pos - y), std::abs(x_pos - x));
			}
		}
	}

	blackCells = ~whiteCells;

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

	isolated_pawn_map = std::vector<uint8_t>(256, 0);

	uint8_t three_lines = 128 | 64 | 32;

	std::vector<uint8_t> pawn_mask(8);
	pawn_mask[0] = 128 | 64;

	for(unsigned int i = 1; i < pawn_mask.size(); ++i, three_lines >>= 1) {
		pawn_mask[i] = three_lines;
	}

	for(int i = 0; i < 256; ++i) {
		for(unsigned int j = 0; j < 8; ++j) {
			if(i & (128 >> j)) {
				isolated_pawn_map[i] += (popcount64(i & pawn_mask[j]) <= 1);
			}
		}
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

	for(unsigned int i = 0; i < 64; ++i) {
		capturePawnMap[WHITE][i] = 0;
		capturePawnMap[BLACK][i] = 0;

		capturePawnMap[WHITE][i] |= ((vec1_cells[i]) << 9) & ~vertical[0];
		capturePawnMap[WHITE][i] |= ((vec1_cells[i]) << 7) & ~vertical[7];
		capturePawnMap[BLACK][i] |= ((vec1_cells[i]) >> 9) & ~vertical[7];
		capturePawnMap[BLACK][i] |= ((vec1_cells[i]) >> 7) & ~vertical[0];
	}

	for(unsigned int i = 0; i < 64; ++i) {
		whitePawnCheckCells[i] = 0;
		blackPawnCheckCells[i] = 0;
	}

	for(unsigned int i = 0; i < 64; ++i) {
		uint64_t pawnMap = capturePawnMap[WHITE][i];

		while(pawnMap) {
			int pos = firstOne(pawnMap);
			pawnMap &= ~vec1_cells[pos];

			whitePawnCheckCells[pos] |= vec1_cells[i];
		}

		pawnMap = capturePawnMap[BLACK][i];
		
		while(pawnMap) {
			int pos = firstOne(pawnMap);
			pawnMap &= ~vec1_cells[pos];
		
			blackPawnCheckCells[pos] |= vec1_cells[i];
		}
	}

	magicInit();

	whiteCells = 0;

	for(int i = 0; i < 64; ++i) {
		int y_pos = i / 8;
		int x_pos = i % 8;
		
		if((y_pos + x_pos) % 2 != 0) {
			whiteCells |= vec1_cells[i];
		}
	}

	blackCells = ~whiteCells;

	colorExtended[whiteSide] = WHITE;
	colorExtended[!whiteSide] = BLACK;
}

void BitBoard::magicInit() {
	for(uint8_t y = 0; y < 8; ++y) {
		for(uint8_t x = 0; x < 8; ++x) {
			rookMagicMask[y][x] = 0;

			if(plus1[y * 8 + x]) {
				rookMagicMask[y][x] |= plus1[y * 8 + x];
				rookMagicMask[y][x] &= ~vertical[7];
			}
			if(minus1[y * 8 + x]) {
				rookMagicMask[y][x] |= minus1[y * 8 + x];
				rookMagicMask[y][x] &= ~vertical[0];
			}
			if(plus8[y * 8 + x]) {
				rookMagicMask[y][x] |= plus8[y * 8 + x];
				rookMagicMask[y][x] &= ~horizontal[7];
			}
			if(minus8[y * 8 + x]) {
				rookMagicMask[y][x] |= minus8[y * 8 + x];
				rookMagicMask[y][x] &= ~horizontal[0];
			}

			rookMagicMask[y][x] &= ~vec2_cells[y][x];
		}
	}

	for(uint8_t y = 0; y < 8; ++y) {
		for(uint8_t x = 0; x < 8; ++x) {
			bishopMagicMask[y][x] = 0;

			if(plus9[y * 8 + x]) {
				bishopMagicMask[y][x] |= plus9[y * 8 + x];
				bishopMagicMask[y][x] &= ~vertical[7];
				bishopMagicMask[y][x] &= ~horizontal[7];
			}
			if(minus9[y * 8 + x]) {
				bishopMagicMask[y][x] |= minus9[y * 8 + x];
				bishopMagicMask[y][x] &= ~vertical[0];
				bishopMagicMask[y][x] &= ~horizontal[0];
			}
			if(plus7[y * 8 + x]) {
				bishopMagicMask[y][x] |= plus7[y * 8 + x];
				bishopMagicMask[y][x] &= ~vertical[0];
				bishopMagicMask[y][x] &= ~horizontal[7];
			}
			if(minus7[y * 8 + x]) {
				bishopMagicMask[y][x] |= minus7[y * 8 + x];
				bishopMagicMask[y][x] &= ~vertical[7];
				bishopMagicMask[y][x] &= ~horizontal[0];
			}

			bishopMagicMask[y][x] &= ~vec2_cells[y][x];
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
					tmp_bitboard &= ~vec1_cells[pos];
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
					tmp_bitboard &= ~vec1_cells[pos];
					if(vec1_cells[m] & k) {
						bit_combination |= vec1_cells[pos];
					}
				}

				bishop_combination[k] = bit_combination;
			}

			std::vector<uint64_t> rook_result_array(std::pow(2, popcount64(rookMagicMask[y][x])));
			for(int s = 0; s < std::pow(2, popcount64(rookMagicMask[y][x])); ++s) {
				uint64_t rook_result = 0;
				rook_result |= (minus1[y * 8 + x] & ~minus1[lastOne(minus1[y * 8 + x] & (horizontal[y] & rook_combination[s]))]);
				rook_result |= (minus8[y * 8 + x] & ~minus8[lastOne(minus8[y * 8 + x] & (vertical[x] & rook_combination[s]))]);
				rook_result |= ( plus1[y * 8 + x] & ~plus1[firstOne(plus1[y * 8 + x] & (horizontal[y] & rook_combination[s]))]);
				rook_result |= ( plus8[y * 8 + x] & ~plus8[firstOne(plus8[y * 8 + x] & (vertical[x] & rook_combination[s]))]);
				uint64_t ind = ((rook_combination[s] * ROOK_MAGIC[y][x]) >> (64 - popcount64(rookMagicMask[y][x])));

				rook_result_array[ind] = rook_result;
			}
			rookMagic[y][x] = Magic(rook_result_array, ROOK_MAGIC[y][x], 64 - popcount64(rookMagicMask[y][x]));

			std::vector<uint64_t> bishop_result_array(std::pow(2, popcount64(bishopMagicMask[y][x])));
			for(int s = 0; s < std::pow(2, popcount64(bishopMagicMask[y][x])); ++s) {
				uint64_t bishop_result = 0;
				bishop_result |= (minus7[y * 8 + x] & ~minus7[lastOne(minus7[y * 8 + x] & ((plus7[y * 8 + x] | minus7[y * 8 + x] | vec2_cells[y][x]) & bishop_combination[s]))]);
				bishop_result |= (minus9[y * 8 + x] & ~minus9[lastOne(minus9[y * 8 + x] & ((plus9[y * 8 + x] | minus9[y * 8 + x] | vec2_cells[y][x]) & bishop_combination[s]))]);
				bishop_result |= ( plus7[y * 8 + x] & ~plus7[firstOne(plus7[y * 8 + x] & ((plus7[y * 8 + x] | minus7[y * 8 + x] | vec2_cells[y][x]) & bishop_combination[s]))]);
				bishop_result |= ( plus9[y * 8 + x] & ~plus9[firstOne(plus9[y * 8 + x] & ((plus9[y * 8 + x] | minus9[y * 8 + x] | vec2_cells[y][x]) & bishop_combination[s]))]);
				uint64_t ind = ((bishop_combination[s] * BISHOP_MAGIC[y][x]) >> (64 - popcount64(bishopMagicMask[y][x])));
				bishop_result_array[ind] = bishop_result;
			}
			bishopMagic[y][x] = Magic(bishop_result_array, BISHOP_MAGIC[y][x], 64 - popcount64(bishopMagicMask[y][x]));
		}
	}
}

int BitBoard::getFiguresCount() {
	return popcount64(currentState.piece_bit_mask[whiteSide] | currentState.piece_bit_mask[!whiteSide]);
}

void BitBoard::bitBoardMoveGenerator(MoveArray& moveArray, size_t& counter_moves) {
	bitBoardAttackMoveGenerator(moveArray, counter_moves);
	uint64_t possibleMoves, mask, emask, occu;

	uint8_t color = colorExtended[currentState.color];
	mask = currentState.piece_bit_mask[currentState.color];
	emask = currentState.piece_bit_mask[!currentState.color];
	occu = (mask | emask);

	//Ладьи
	uint64_t rook = currentState.figures[ROOK] & mask;
	while(rook != 0) {
		uint8_t pos = firstOne(rook);

		possibleMoves = rookMagic[pos / 8][pos % 8].getPossibleMoves(rookMagicMask[pos / 8][pos % 8] & occu & ~vec1_cells[pos]) & ~(currentState.figures[KING] ^ emask) & ~emask & ~mask;

		rook &= ~vec1_cells[pos];

		counter_moves += popcount64(possibleMoves);

		while(possibleMoves != 0) {
			uint64_t to = firstOne(possibleMoves);
			moveArray.addMove(BitMove(ROOK | color, pos / 8, pos % 8, to / 8, to % 8));
			possibleMoves &= ~vec1_cells[to];
		}
	}

	//Слоны
	uint64_t bishop = currentState.figures[BISHOP] & mask;
	while(bishop != 0) {
		uint8_t pos = firstOne(bishop);

		possibleMoves = bishopMagic[pos / 8][pos % 8].getPossibleMoves(bishopMagicMask[pos / 8][pos % 8] & occu & ~vec1_cells[pos]) & ~(currentState.figures[KING] ^ emask) & ~emask & ~mask;
		bishop &= ~vec1_cells[pos];

		counter_moves += popcount64(possibleMoves);

		while(possibleMoves != 0) {
			uint64_t to = firstOne(possibleMoves);
			moveArray.addMove(BitMove(BISHOP | color, pos / 8, pos % 8, to / 8, to % 8));
			possibleMoves &= ~vec1_cells[to];
		}
	}

	//Ферзи
	uint64_t queen = currentState.figures[QUEEN] & mask;
	while(queen != 0) {
		uint8_t pos = firstOne(queen);

		possibleMoves = rookMagic[pos / 8][pos % 8].getPossibleMoves(rookMagicMask[pos / 8][pos % 8] & occu & ~vec1_cells[pos]) & ~(currentState.figures[KING] ^ emask) & ~emask & ~mask;
		possibleMoves |= (bishopMagic[pos / 8][pos % 8].getPossibleMoves(bishopMagicMask[pos / 8][pos % 8] & occu & ~vec1_cells[pos]) & ~(currentState.figures[KING] ^ emask) & ~emask & ~mask);

		queen &= ~vec1_cells[pos];
		counter_moves += popcount64(possibleMoves);

		while(possibleMoves != 0) {
			uint64_t to = firstOne(possibleMoves);
			moveArray.addMove(BitMove(QUEEN | color, pos / 8, pos % 8, to / 8, to % 8));
			possibleMoves &= ~vec1_cells[to];
		}
	}

	//Кони
	uint64_t knight = currentState.figures[KNIGHT] & mask;
	while(knight != 0) {
		uint8_t pos = firstOne(knight);
		possibleMoves = bitboard[KNIGHT | color][pos / 8][pos % 8] & ~mask & ~(currentState.figures[KING] & emask);
		knight &= ~vec1_cells[pos];
		possibleMoves &= ~emask;
		counter_moves += popcount64(possibleMoves);

		while(possibleMoves != 0) {
			uint64_t to = firstOne(possibleMoves);
			moveArray.addMove(BitMove(KNIGHT | color, pos / 8, pos % 8, to / 8, to % 8));
			possibleMoves &= ~vec1_cells[to];
			possibleMoves &= ~(currentState.figures[KING] & emask);
		}
	}

	//Короли
	uint64_t king = currentState.figures[KING] & mask;
	while(king != 0) {
		uint8_t pos = firstOne(king);
		possibleMoves = bitboard[KING | color][pos / 8][pos % 8] & ~mask & ~(currentState.figures[KING] & emask);
		king &= ~vec1_cells[pos];
		possibleMoves &= ~emask;
		counter_moves += popcount64(possibleMoves);

		while(possibleMoves != 0) {
			uint64_t to = firstOne(possibleMoves);
			moveArray.addMove(BitMove(KING | color, pos / 8, pos % 8, to / 8, to % 8));
			possibleMoves &= ~vec1_cells[to];
		}
	}

	if(currentState.color) {
		if(wsc()) {
			if((occu & (vec2_cells[0][5] | vec2_cells[0][6])) == 0) {
				if(!inCheck(WHITE, 0, 4) && !inCheck(WHITE, 0, 5) && !inCheck(WHITE, 0, 6)) {
					moveArray.addMove(BitMove(KING | color, 0, 4, 0, 6));
				}
			}
		}
		if(wlc()) {
			if((occu & (vec2_cells[0][3] | vec2_cells[0][2] | vec2_cells[0][1])) == 0) {
				if(!inCheck(WHITE, 0, 4) && !inCheck(WHITE, 0, 3) && !inCheck(WHITE, 0, 2)) {
					moveArray.addMove(BitMove(KING | color, 0, 4, 0, 2));
				}
			}
		}
	} else {
		if(bsc()) {
			if((occu & (vec2_cells[7][5] | vec2_cells[7][6])) == 0) {
				if(!inCheck(BLACK, 7, 4) && !inCheck(BLACK, 7, 5) && !inCheck(BLACK, 7, 6)) {
					moveArray.addMove(BitMove(KING | color, 7, 4, 7, 6));
				}
			}
		}
		if(blc()) {
			if((occu & (vec2_cells[7][3] | vec2_cells[7][2] | vec2_cells[7][1])) == 0) {
				if(!inCheck(BLACK, 7, 4) && !inCheck(BLACK, 7, 3) && !inCheck(BLACK, 7, 2)) {
					moveArray.addMove(BitMove(KING | color, 7, 4, 7, 2));
				}
			}

		}
	}

	//Пешки
	if(currentState.color) {
		uint64_t pawn = currentState.figures[PAWN] & mask & ~horizontal[6];
		while(pawn != 0) {
			uint8_t pos = firstOne(pawn);
			possibleMoves = bitboard[PAWN | WHITE][pos / 8][pos % 8];
			possibleMoves &= (plus8[pos] & ~plus8[firstOne(plus8[pos] & occu)]);
			possibleMoves &= ~vec1_cells[firstOne(plus8[pos] & occu)];
			possibleMoves &= ~occu;
			pawn &= ~vec1_cells[pos];
			counter_moves += popcount64(possibleMoves);

			while(possibleMoves != 0) {
				uint64_t to = firstOne(possibleMoves);
				moveArray.addMove(BitMove(PAWN | color, pos / 8, pos % 8, to / 8, to % 8));
				possibleMoves &= ~vec1_cells[to];
			}
		}

		pawn = currentState.figures[PAWN] & mask & (horizontal[6]);
		while(pawn != 0) {
			uint8_t pos = firstOne(pawn);
			possibleMoves = bitboard[PAWN | WHITE][pos / 8][pos % 8];
			possibleMoves &= ( plus8[pos] & ~plus8[firstOne(plus8[pos] & occu)]);
			possibleMoves &= ~vec1_cells[firstOne(plus8[pos] & occu)];
			possibleMoves &= ~occu;
			pawn &= ~vec1_cells[pos];
			counter_moves += popcount64(possibleMoves);

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

				possibleMoves &= ~vec1_cells[to];
			}
		}
	} else {
		uint64_t pawn = currentState.figures[PAWN] & mask & ~horizontal[1];
		while(pawn != 0) {
			uint8_t pos = firstOne(pawn);
			possibleMoves = bitboard[PAWN | BLACK][pos / 8][pos % 8];
			possibleMoves &= (minus8[pos] & ~minus8[lastOne(minus8[pos] & occu)]);
			possibleMoves &= ~vec1_cells[lastOne(minus8[pos] & occu)];
			possibleMoves &= ~occu;
			pawn &= ~vec1_cells[pos];
			counter_moves += popcount64(possibleMoves);

			while(possibleMoves != 0) {
				uint64_t to = firstOne(possibleMoves);
				moveArray.addMove(BitMove(PAWN | color, pos / 8, pos % 8, to / 8, to % 8));
				possibleMoves &= ~vec1_cells[to];
			}
		}

		pawn = currentState.figures[PAWN] & mask & (horizontal[1]);

		while(pawn != 0) {
			uint8_t pos = firstOne(pawn);
			possibleMoves = bitboard[PAWN | BLACK][pos / 8][pos % 8];
			possibleMoves &= ( minus8[pos] & ~minus8[firstOne(minus8[pos] & occu)]);
			possibleMoves &= ~vec1_cells[firstOne(minus8[pos] & occu)];
			possibleMoves &= ~(mask | emask);
			pawn &= ~vec1_cells[pos];
			counter_moves += popcount64(possibleMoves);

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

				possibleMoves &= ~vec1_cells[to];
			}
		}
	}
}

void BitBoard::bitBoardAttackMoveGenerator(MoveArray& moveArray, size_t& counter_moves) {
	moveArray.clear();
	uint64_t possibleMoves, mask, emask, occu;

	uint8_t color = colorExtended[currentState.color];
	mask = currentState.piece_bit_mask[currentState.color];
	emask = currentState.piece_bit_mask[!currentState.color];
	occu = (mask | emask);

	//Пешки
	if(currentState.color) {
		uint64_t pawn = currentState.figures[PAWN] & mask & ~horizontal[6];
		uint64_t rightPawnAttack = (pawn << 9) & ~vertical[0] & emask & ~currentState.figures[KING];
		moveArray.num_attacks += popcount64(rightPawnAttack);
		while(rightPawnAttack != 0) {
			uint64_t to = firstOne(rightPawnAttack);
			moveArray.addMove(BitMove(getFigure(to / 8, to % 8), PAWN | color, to / 8 - 1, to % 8 - 1, to / 8, to % 8));
			rightPawnAttack &= ~vec1_cells[to];
		}
		uint64_t leftPawnAttack = (pawn << 7) & ~vertical[7] & emask & ~currentState.figures[KING];
		moveArray.num_attacks += popcount64(leftPawnAttack);
		while(leftPawnAttack != 0) {
			uint64_t to = firstOne(leftPawnAttack);
			moveArray.addMove(BitMove(getFigure(to / 8, to % 8), PAWN | color, to / 8 - 1, to % 8 + 1, to / 8, to % 8));
			leftPawnAttack &= ~vec1_cells[to];
		}

		pawn = currentState.figures[PAWN] & mask & horizontal[6];
		rightPawnAttack = (pawn << 9) & ~vertical[0] & emask & ~currentState.figures[KING];
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

			rightPawnAttack &= ~vec1_cells[to];
		}
		leftPawnAttack = (pawn << 7) & ~vertical[7] & emask & ~currentState.figures[KING];
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

			leftPawnAttack &= ~vec1_cells[to];
		}
	} else {
		uint64_t pawn = currentState.figures[PAWN] & mask & ~horizontal[1];
		uint64_t rightPawnAttack = (pawn >> 9) & ~vertical[7] & emask & ~currentState.figures[KING];
		moveArray.num_attacks += popcount64(rightPawnAttack);
		while(rightPawnAttack != 0) {
			uint64_t to = firstOne(rightPawnAttack);
			moveArray.addMove(BitMove(getFigure(to / 8, to % 8), PAWN | color, to / 8 + 1, to % 8 + 1, to / 8, to % 8));
			rightPawnAttack &= ~vec1_cells[to];
		}

		uint64_t leftPawnAttack = (pawn >> 7) & ~vertical[0] & emask & ~currentState.figures[KING];
		moveArray.num_attacks += popcount64(leftPawnAttack);
		while(leftPawnAttack != 0) {
			uint64_t to = firstOne(leftPawnAttack);
			moveArray.addMove(BitMove(getFigure(to / 8, to % 8), PAWN | color, to / 8 + 1, to % 8 - 1, to / 8, to % 8));
			leftPawnAttack &= ~vec1_cells[to];
		}

		pawn = currentState.figures[PAWN] & mask & horizontal[1];
		rightPawnAttack = (pawn >> 9) & ~vertical[7] & emask & ~currentState.figures[KING];
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

			rightPawnAttack &= ~vec1_cells[to];
		}
		leftPawnAttack = (pawn >> 7) & ~vertical[0] & emask & ~currentState.figures[KING];
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

			leftPawnAttack &= ~vec1_cells[to];
		}
	}

	//Пешки (взятие на проходе)

	if(currentState.passant_enable) {
		if(currentState.color) {
			uint64_t passant_cell = vec2_cells[currentState.passant_y][currentState.passant_x];
			uint64_t whitePawnMask = currentState.figures[PAWN] & currentState.piece_bit_mask[whiteSide] & horizontal[4];

			if(((passant_cell >> 9) & whitePawnMask)) {
				moveArray.addMove(BitMove(PAWN | BLACK, PAWN | WHITE, currentState.passant_y - 1, currentState.passant_x - 1, currentState.passant_y, currentState.passant_x, true));
			}

			if(((passant_cell >> 7) & whitePawnMask)) {
				moveArray.addMove(BitMove(PAWN | BLACK, PAWN | WHITE, currentState.passant_y - 1, currentState.passant_x + 1, currentState.passant_y, currentState.passant_x, true));
			}
		} else {
			uint64_t passant_cell = vec2_cells[currentState.passant_y][currentState.passant_x];
			uint64_t blackPawnMask = currentState.figures[PAWN] & currentState.piece_bit_mask[!whiteSide] & horizontal[3];

			if(((passant_cell << 9) & blackPawnMask)) {
				moveArray.addMove(BitMove(PAWN | WHITE, PAWN | BLACK, currentState.passant_y + 1, currentState.passant_x + 1, currentState.passant_y, currentState.passant_x, true));
			}

			if(((passant_cell << 7) & blackPawnMask)) {
				moveArray.addMove(BitMove(PAWN | WHITE, PAWN | BLACK, currentState.passant_y + 1, currentState.passant_x - 1, currentState.passant_y, currentState.passant_x, true));
			}
		}
	}

	//Кони
	uint64_t knight = currentState.figures[KNIGHT] & mask;
	while(knight != 0) {
		uint8_t pos = firstOne(knight);
		possibleMoves = bitboard[KNIGHT | color][pos / 8][pos % 8] & ~mask & ~(currentState.figures[KING] & emask);
		possibleMoves &= emask;
		knight &= ~vec1_cells[pos];
		counter_moves += popcount64(possibleMoves);
		moveArray.num_attacks += popcount64(possibleMoves);

		while(possibleMoves != 0) {
			uint64_t to = firstOne(possibleMoves);
			moveArray.addMove(BitMove(getFigure(to / 8, to % 8), KNIGHT | color, pos / 8, pos % 8, to / 8, to % 8));
			possibleMoves &= ~vec1_cells[to];
			possibleMoves &= ~(currentState.figures[KING] & emask);
		}
	}

	//Слоны
	uint64_t bishop = currentState.figures[BISHOP] & mask;
	while(bishop != 0) {
		possibleMoves = 0;
		uint8_t pos = firstOne(bishop);
		possibleMoves = bishopMagic[pos / 8][pos % 8].getPossibleMoves(bishopMagicMask[pos / 8][pos % 8] & occu & ~vec1_cells[pos]) & ~(currentState.figures[KING] & emask) & emask & ~mask;
		bishop &= ~vec1_cells[pos];
		counter_moves += popcount64(possibleMoves);
		moveArray.num_attacks += popcount64(possibleMoves);

		while(possibleMoves != 0) {
			uint64_t to = firstOne(possibleMoves);
			moveArray.addMove(BitMove(getFigure(to / 8, to % 8), BISHOP | color, pos / 8, pos % 8, to / 8, to % 8));
			possibleMoves &= ~vec1_cells[to];
		}
	}

	//Ладьи
	uint64_t rook = currentState.figures[ROOK] & mask;
	while(rook != 0) {
		possibleMoves = 0;
		uint8_t pos = firstOne(rook);

		possibleMoves = rookMagic[pos / 8][pos % 8].getPossibleMoves(rookMagicMask[pos / 8][pos % 8] & occu & ~vec1_cells[pos]) & ~(currentState.figures[KING] & emask) & emask & ~mask;
		rook &= ~vec1_cells[pos];
		counter_moves += popcount64(possibleMoves);
		moveArray.num_attacks += popcount64(possibleMoves);

		while(possibleMoves != 0) {
			uint64_t to = firstOne(possibleMoves);
			moveArray.addMove(BitMove(getFigure(to / 8, to % 8), ROOK | color, pos / 8, pos % 8, to / 8, to % 8));
			possibleMoves &= ~vec1_cells[to];
		}
	}

	//Ферзи
	uint64_t queen = currentState.figures[QUEEN] & mask;
	while(queen != 0) {
		possibleMoves = 0;
		uint8_t pos = firstOne(queen);
		possibleMoves = rookMagic[pos / 8][pos % 8].getPossibleMoves(rookMagicMask[pos / 8][pos % 8] & occu & ~vec1_cells[pos]) & ~(currentState.figures[KING] & emask) & emask & ~mask;
		possibleMoves |= (bishopMagic[pos / 8][pos % 8].getPossibleMoves(bishopMagicMask[pos / 8][pos % 8] & occu & ~vec1_cells[pos]) & ~(currentState.figures[KING] & emask) & emask & ~mask);
		queen &= ~vec1_cells[pos];
		counter_moves += popcount64(possibleMoves);
		moveArray.num_attacks += popcount64(possibleMoves);

		while(possibleMoves != 0) {
			uint64_t to = firstOne(possibleMoves);
			moveArray.addMove(BitMove(getFigure(to / 8, to % 8), QUEEN | color, pos / 8, pos % 8, to / 8, to % 8));
			possibleMoves &= ~vec1_cells[to];
		}
	}

	//Короли
	uint64_t king = currentState.figures[KING] & mask;
	while(king != 0) {
		uint8_t pos = firstOne(king);
		possibleMoves = bitboard[KING | color][pos / 8][pos % 8] & ~mask & ~(currentState.figures[KING] & emask);
		possibleMoves &= emask;
		king &= ~vec1_cells[pos];
		counter_moves += popcount64(possibleMoves);
		moveArray.num_attacks += popcount64(possibleMoves);

		while(possibleMoves != 0) {
			uint64_t to = firstOne(possibleMoves);
			moveArray.addMove(BitMove(getFigure(to / 8, to % 8), KING | color, pos / 8, pos % 8, to / 8, to % 8));
			possibleMoves &= ~vec1_cells[to];
		}
	}
}

double BitBoard::bitBoardMobilityEval(bool clr, double stage_game) {
	/*
	Функция оценивает мобильность фигур и безопасность короля одновременно, т. к. это позволяет не замедлять работу оценочной функции
	*/
	uint64_t possibleMoves, mask, emask, occu;

	uint8_t color = colorExtended[clr];
	uint8_t enemyColor = colorExtended[!clr];
	mask = currentState.piece_bit_mask[clr];
	emask = currentState.piece_bit_mask[!clr];
	occu = (mask | emask);

	double score = 0;

	uint64_t enemyKingPos = currentState.figures[KING] & emask;
	uint8_t pos = firstOne(enemyKingPos);
	uint64_t enemyKingPossibleMoves = bitboard[KING | enemyColor][pos / 8][pos % 8] & ~mask & ~emask;

	int kingDanger = 0;

	//Маска с полями, которые бьются пешками противника

	uint64_t goodCells = 0;


	uint64_t pawn = currentState.figures[PAWN] & emask;

	while(pawn != 0) {
		uint8_t pos = firstOne(pawn);
		goodCells |= capturePawnMap[enemyColor][pos];
		pawn &= ~vec1_cells[pos];
	}

	goodCells = ~goodCells;

	//Ладьи
	uint64_t rook = currentState.figures[ROOK] & mask;
	while(rook != 0) {
		uint8_t pos = firstOne(rook);
		possibleMoves = rookMagic[pos / 8][pos % 8].getPossibleMoves(rookMagicMask[pos / 8][pos % 8] & occu & ~vec1_cells[pos]) & ~(currentState.figures[KING] & emask) & ~mask;
		rook &= ~vec1_cells[pos];
		kingDanger += 3 * popcount64(possibleMoves & enemyKingPossibleMoves);
		possibleMoves &= goodCells;
		score += RookMobilityBonus[(int)popcount64(possibleMoves)];
	}

	//Слоны
	uint64_t bishop = currentState.figures[BISHOP] & mask;
	while(bishop != 0) {
		uint8_t pos = firstOne(bishop);
		possibleMoves = bishopMagic[pos / 8][pos % 8].getPossibleMoves(bishopMagicMask[pos / 8][pos % 8] & occu & ~vec1_cells[pos]) & ~(currentState.figures[KING] & emask) & ~mask;
		bishop &= ~vec1_cells[pos];		
		kingDanger += 2 * popcount64(possibleMoves & enemyKingPossibleMoves);
		possibleMoves &= goodCells;
		score += BishopMobilityBonus[(int)popcount64(possibleMoves)];
	}

	//Ферзи
	uint64_t queen = currentState.figures[QUEEN] & mask;
	while(queen != 0) {
		uint8_t pos = firstOne(queen);
		possibleMoves = rookMagic[pos / 8][pos % 8].getPossibleMoves(rookMagicMask[pos / 8][pos % 8] & occu & ~vec1_cells[pos]) & ~(currentState.figures[KING] & emask) & ~mask;
		possibleMoves |= (bishopMagic[pos / 8][pos % 8].getPossibleMoves(bishopMagicMask[pos / 8][pos % 8] & occu & ~vec1_cells[pos]) & ~(currentState.figures[KING] & emask) & ~mask);
		queen &= ~vec1_cells[pos];
		kingDanger += 5 * popcount64(possibleMoves & enemyKingPossibleMoves);
		possibleMoves &= goodCells;
		score += QueenMobilityBonus[(int)popcount64(possibleMoves)];
		
	}

	//Кони
	uint64_t knight = currentState.figures[KNIGHT] & mask;
	while(knight != 0) {
		uint8_t pos = firstOne(knight);
		possibleMoves = bitboard[KNIGHT | color][pos / 8][pos % 8] & ~mask & ~(currentState.figures[KING] & emask);
		knight &= ~vec1_cells[pos];
		kingDanger += 2 * popcount64(possibleMoves & enemyKingPossibleMoves);
		possibleMoves &= goodCells;
		score += KnightMobilityBonus[(int)popcount64(possibleMoves)];
	}

	score += SafetyTable[std::min(99, kingDanger)];
	return score;
}

void BitBoard::move(BitMove& mv) {
	pushHistory();
	uint8_t movedFigure = getFigure(mv.fromY, mv.fromX);
	currentState.recapture_position = vec2_cells[mv.toY][mv.toX];
	currentState.attacked = false;

	clearCell(mv.toY, mv.toX);
	clearCell(mv.fromY, mv.fromX);

	if(mv.replaced) {
		addFigure(mv.replacedFigure, mv.toY, mv.toX);
	} else {
		addFigure(movedFigure, mv.toY, mv.toX);
	}

	if(mv.passant) {
		if(currentState.color) {
			clearCell(currentState.passant_y - 1, currentState.passant_x);
		} else {
			clearCell(currentState.passant_y + 1, currentState.passant_x);
		}
	}

	if((mv.movedFigure & TYPE_SAVE) == KING) {
		if(currentState.color) {
			if(mv.fromY == 0 && mv.fromX == 4 && mv.toY == 0 && mv.toX == 6) {
				addFigure(ROOK | WHITE, 0, 5);
				clearCell(0, 7);
				currentState.hash ^= wscZobrist;
				currentState.whitePassantMade = true;
			} else if(mv.fromY == 0 && mv.fromX == 4 && mv.toY == 0 && mv.toX == 2) {
				addFigure(ROOK | WHITE, 0, 3);
				clearCell(0, 0);

				currentState.hash ^= wlcZobrist;

				currentState.whitePassantMade = true;
			}
		} else {
			if(mv.fromY == 7 && mv.fromX == 4 && mv.toY == 7 && mv.toX == 6) {
				addFigure(ROOK | BLACK, 7, 5);
				clearCell(7, 7);
				currentState.hash ^= bscZobrist;
				currentState.blackPassantMade = true;
			} else if(mv.fromY == 7 && mv.fromX == 4 && mv.toY == 7 && mv.toX == 2) {
				addFigure(ROOK | BLACK, 7, 3);
				clearCell(7, 0);
				currentState.hash ^= blcZobrist;
				currentState.blackPassantMade = true;
			}
		}
	}

	currentState.passant_enable = false;

	if((mv.movedFigure & TYPE_SAVE) == PAWN && abs(mv.toY - mv.fromY) > 1) {
		currentState.passant_enable = true;
		currentState.passant_y = (mv.toY + mv.fromY) / 2;
		currentState.passant_x = mv.fromX;
	}

	currentState.color = !currentState.color;
	if(currentState.color) {
		++currentState.moveNumber;
	}

	if(!mv.isAttack && (mv.movedFigure & TYPE_SAVE) != PAWN) {
		++currentState.ruleNumber;
	} else {
		currentState.ruleNumber = 0;
	}

	if(mv.isAttack) {
		currentState.attacked = true;
	}

	currentState.castlingMap &= (currentState.figures[KING] | currentState.figures[ROOK]);

	++third_repeat[getColorHash() & hash_cutter];

	if(third_repeat[getColorHash() & hash_cutter] >= 3 || currentState.ruleNumber >= 100) {
		currentState.hash_enable = false;
	}
}

void BitBoard::goBack() {
	--history_iterator;
	if(history_iterator >= 0) {
		--third_repeat[getColorHash() & hash_cutter];
		currentState = history[history_iterator];
	}
}


void BitBoard::clearCell(uint8_t y, uint8_t x) {
	if(!(vec2_cells[y][x] & (currentState.piece_bit_mask[whiteSide] | currentState.piece_bit_mask[!whiteSide]))) {
		return;
	}

	uint8_t figure = getFigure(y, x);

	currentState.hash ^= zobrist[figure][y][x];

	if(figure) {
		uint8_t figure_test = figure;
		if((figure_test & TYPE_SAVE) != 0) {
			currentState.figures[figure_test & TYPE_SAVE] &= ~vec2_cells[y][x];
		}

		currentState.piece_bit_mask[whiteSide] &= ~vec2_cells[y][x];
		currentState.piece_bit_mask[!whiteSide] &= ~vec2_cells[y][x];
	}
}

void BitBoard::addFigure(uint8_t figure, uint8_t y, uint8_t x) {
	uint8_t color = (figure & COLOR_SAVE);
	uint8_t originalFigure = figure;

	if(figure) {
		currentState.figures[figure & TYPE_SAVE] |= vec2_cells[y][x];

		if(color == WHITE) {
			currentState.piece_bit_mask[whiteSide] |= vec2_cells[y][x];
		} else if(color == BLACK) {
			currentState.piece_bit_mask[!whiteSide] |= vec2_cells[y][x];
		}
	}

	currentState.hash ^= zobrist[originalFigure][y][x];
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
	history[history_iterator] = currentState;
	++history_iterator;

	if(history_iterator >= history.size()) {
		history.resize(history_iterator + 1);
	}
}

uint8_t BitBoard::getFigure(uint8_t y, uint8_t x) {
	for(uint8_t i = 1; i < 7; ++i) {
		if(currentState.figures[i] & vec2_cells[y][x] & currentState.piece_bit_mask[whiteSide]) {
			return (i | WHITE);
		} else if(currentState.figures[i] & vec2_cells[y][x] & currentState.piece_bit_mask[!whiteSide]) {
			return (i | BLACK);
		}
	}

	return 0;
}

BitMove BitBoard::getRandomMove() {
	MoveArray moveArray;
	bitBoardMoveGenerator(moveArray, stress);

	uint8_t color;

	if(currentState.color) {
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

int64_t BitBoard::getEvaluate() {
	return newEvaluateAll() * (2 * currentState.color - 1) * (1 - !currentState.hash_enable);
}

bool BitBoard::inCheck(uint8_t color) {
	bool compressColor = color & (1 << 3);

	uint64_t mask = currentState.piece_bit_mask[compressColor];
	uint64_t emask = currentState.piece_bit_mask[!compressColor];
	uint64_t occu = (mask | emask);

	uint64_t kingPos = currentState.figures[KING] & mask;
	uint8_t kingCoord = firstOne(kingPos);

	if(!(bitboard[ROOK | colorExtended[!compressColor]][kingCoord / 8][kingCoord % 8] & currentState.piece_bit_mask[!compressColor] & (currentState.figures[ROOK] | currentState.figures[QUEEN])) &&
		!(bitboard[BISHOP | colorExtended[!compressColor]][kingCoord / 8][kingCoord % 8] & currentState.piece_bit_mask[!compressColor] & (currentState.figures[BISHOP] | currentState.figures[QUEEN])) &&
		!(bitboard[KNIGHT | colorExtended[!compressColor]][kingCoord / 8][kingCoord % 8] & currentState.piece_bit_mask[!compressColor] & (currentState.figures[KNIGHT])) &&
		!(bitboard[KING | colorExtended[!compressColor]][kingCoord / 8][kingCoord % 8] & currentState.piece_bit_mask[!compressColor] & (currentState.figures[KING])) &&
		!(capturePawnMap[colorExtended[compressColor]][kingCoord] & currentState.piece_bit_mask[!compressColor] & (currentState.figures[PAWN]))) {
			return false;
	}


	uint64_t figure;
	figure = firstOne(plus8[kingCoord] & occu);
	figure = vec1_cells[figure];
	if(plus8[kingCoord] & occu && (figure & (currentState.figures[ROOK] | currentState.figures[QUEEN]) & emask)) {
		return true;
	}

	figure = firstOne(plus1[kingCoord] & occu);
	figure = vec1_cells[figure];
	if(plus1[kingCoord] & occu && (figure & (currentState.figures[ROOK] | currentState.figures[QUEEN]) & emask)) {
		return true;
	}

	figure = lastOne(minus8[kingCoord] & occu);
	figure = vec1_cells[figure];
	if(minus8[kingCoord] & occu && (figure & (currentState.figures[ROOK] | currentState.figures[QUEEN]) & emask)) {
		return true;
	}

	figure = lastOne(minus1[kingCoord] & occu);
	figure = vec1_cells[figure];
	if(minus1[kingCoord] & occu && (figure & (currentState.figures[ROOK] | currentState.figures[QUEEN]) & emask)) {
		return true;
	}

	figure = firstOne(plus7[kingCoord] & occu);
	figure = vec1_cells[figure];
	if(plus7[kingCoord] & occu && (figure & (currentState.figures[BISHOP] | currentState.figures[QUEEN]) & emask)) {
		return true;
	}

	figure = firstOne(plus9[kingCoord] & occu);
	figure = vec1_cells[figure];
	if(plus9[kingCoord] & occu && (figure & (currentState.figures[BISHOP] | currentState.figures[QUEEN]) & emask)) {
		return true;
	}

	figure = lastOne(minus7[kingCoord] & occu);
	figure = vec1_cells[figure];
	if(minus7[kingCoord] & occu && (figure & (currentState.figures[BISHOP] | currentState.figures[QUEEN]) & emask)) {
		return true;
	}

	figure = lastOne(minus9[kingCoord] & occu);
	figure = vec1_cells[figure];
	if(minus9[kingCoord] & occu && (figure & (currentState.figures[BISHOP] | currentState.figures[QUEEN]) & emask)) {
		return true;
	}

	if(bitboard[KNIGHT | WHITE][kingCoord / 8][kingCoord % 8] & currentState.figures[KNIGHT] & emask) {
		return true;
	}

	if(color == WHITE) {
		if(((currentState.figures[PAWN] & emask) >> 9) & ~vertical[7] & kingPos) {
			return true;
		}

		if(((currentState.figures[PAWN] & emask) >> 7) & ~vertical[0] & kingPos) {
			return true;
		}
	} else {
		if(((currentState.figures[PAWN] & emask) << 9) & ~vertical[0] & kingPos) {
			return true;
		}

		if(((currentState.figures[PAWN] & emask) << 7) & ~vertical[7] & kingPos) {
			return true;
		}
	}

	uint8_t eKingCoord = firstOne(currentState.figures[KING] & emask);

	if(bitboard[KING | WHITE][eKingCoord / 8][eKingCoord % 8] & kingPos) {
		return true;
	}

	return false;
}

bool BitBoard::inCheck(uint8_t color, uint8_t y, uint8_t x) {
	bool compressColor = color & (1 << 3);

	uint64_t mask = currentState.piece_bit_mask[compressColor];
	uint64_t emask = currentState.piece_bit_mask[!compressColor];
	uint64_t occu = (mask | emask);

	uint64_t kingPos = vec2_cells[y][x];
	uint8_t kingCoord = y * 8 + x;

	if(!(bitboard[ROOK | colorExtended[!compressColor]][kingCoord / 8][kingCoord % 8] & currentState.piece_bit_mask[!compressColor] & (currentState.figures[ROOK] | currentState.figures[QUEEN])) &&
		!(bitboard[BISHOP | colorExtended[!compressColor]][kingCoord / 8][kingCoord % 8] & currentState.piece_bit_mask[!compressColor] & (currentState.figures[BISHOP] | currentState.figures[QUEEN])) &&
		!(bitboard[KNIGHT | colorExtended[!compressColor]][kingCoord / 8][kingCoord % 8] & currentState.piece_bit_mask[!compressColor] & (currentState.figures[KNIGHT])) &&
		!(bitboard[KING | colorExtended[!compressColor]][kingCoord / 8][kingCoord % 8] & currentState.piece_bit_mask[!compressColor] & (currentState.figures[KING])) &&
		!(capturePawnMap[colorExtended[compressColor]][kingCoord] & currentState.piece_bit_mask[!compressColor] & (currentState.figures[PAWN]))) {
			return false;
	}

	uint64_t figure;
	figure = firstOne(plus8[kingCoord] & occu);
	figure = vec1_cells[figure];
	if(plus8[kingCoord] & occu && (figure & (currentState.figures[ROOK] | currentState.figures[QUEEN]) & emask)) {
		return true;
	}

	figure = firstOne(plus1[kingCoord] & occu);
	figure = vec1_cells[figure];
	if(plus1[kingCoord] & occu && (figure & (currentState.figures[ROOK] | currentState.figures[QUEEN]) & emask)) {
		return true;
	}

	figure = lastOne(minus8[kingCoord] & occu);
	figure = vec1_cells[figure];
	if(minus8[kingCoord] & occu && (figure & (currentState.figures[ROOK] | currentState.figures[QUEEN]) & emask)) {
		return true;
	}

	figure = lastOne(minus1[kingCoord] & occu);
	figure = vec1_cells[figure];
	if(minus1[kingCoord] & occu && (figure & (currentState.figures[ROOK] | currentState.figures[QUEEN]) & emask)) {
		return true;
	}

	figure = firstOne(plus7[kingCoord] & occu);
	figure = vec1_cells[figure];
	if(plus7[kingCoord] & occu && (figure & (currentState.figures[BISHOP] | currentState.figures[QUEEN]) & emask)) {
		return true;
	}

	figure = firstOne(plus9[kingCoord] & occu);
	figure = vec1_cells[figure];
	if(plus9[kingCoord] & occu && (figure & (currentState.figures[BISHOP] | currentState.figures[QUEEN]) & emask)) {
		return true;
	}

	figure = lastOne(minus7[kingCoord] & occu);
	figure = vec1_cells[figure];
	if(minus7[kingCoord] & occu && (figure & (currentState.figures[BISHOP] | currentState.figures[QUEEN]) & emask)) {
		return true;
	}

	figure = lastOne(minus9[kingCoord] & occu);
	figure = vec1_cells[figure];
	if(minus9[kingCoord] & occu && (figure & (currentState.figures[BISHOP] | currentState.figures[QUEEN]) & emask)) {
		return true;
	}

	if(bitboard[KNIGHT | WHITE][kingCoord / 8][kingCoord % 8] & currentState.figures[KNIGHT] & emask) {
		return true;
	}

	if(color == WHITE) {
		if(((currentState.figures[PAWN] & emask) >> 9) & ~vertical[7] & kingPos) {
			return true;
		}

		if(((currentState.figures[PAWN] & emask) >> 7) & ~vertical[0] & kingPos) {
			return true;
		}
	} else {
		if(((currentState.figures[PAWN] & emask) << 9) & ~vertical[0] & kingPos) {
			return true;
		}

		if(((currentState.figures[PAWN] & emask) << 7) & ~vertical[7] & kingPos) {
			return true;
		}
	}

	uint8_t eKingCoord = firstOne(currentState.figures[KING] & emask);

	if(bitboard[KING | WHITE][eKingCoord / 8][eKingCoord % 8] & kingPos) {
		return true;
	}

	return false;
}

bool BitBoard::wsc() {
	return (currentState.castlingMap & vec2_cells[0][4]) && (currentState.castlingMap & vec2_cells[0][7]);
}

bool BitBoard::wlc() {
	return (currentState.castlingMap & vec2_cells[0][4]) && (currentState.castlingMap & vec2_cells[0][0]);
}

bool BitBoard::bsc() {
	return (currentState.castlingMap & vec2_cells[7][4]) && (currentState.castlingMap & vec2_cells[7][7]);
}

bool BitBoard::blc() {
	return (currentState.castlingMap & vec2_cells[7][4]) && (currentState.castlingMap & vec2_cells[7][0]);
}

void BitBoard::zobristGenerator() {
	std::random_device rd;
	std::mt19937_64 gen(rd());
	gen.seed(0);
	std::uniform_int_distribution<unsigned long long> dis;

	for(int i = 0; i < 32; ++i) {
		for(int j = 0; j < BOARD_SIZE; ++j) {
			for(int k = 0; k < BOARD_SIZE; ++k) {
				zobrist[i][j][k] = dis(gen);
			}
		}
	}

	wscZobrist = dis(gen);
	wlcZobrist = dis(gen);
	bscZobrist = dis(gen);
	blcZobrist = dis(gen);

	for(int i = 0; i < BOARD_SIZE; ++i) {
		for(int j = 0; j < BOARD_SIZE; ++j) {
			passantZobrist[i][j] = dis(gen);
		}
	}
}

void BitBoard::generateHash() {
	uint64_t figure_positions = (currentState.piece_bit_mask[whiteSide] | currentState.piece_bit_mask[!whiteSide]);
	currentState.hash = 0;

	while(figure_positions) {
		uint8_t pos = firstOne(figure_positions);
		currentState.hash ^= (zobrist[getFigure(pos / 8, pos % 8)][pos / 8][pos % 8]);
		figure_positions &= ~vec1_cells[pos];
	}
}

uint64_t BitBoard::getHash() {
	return currentState.hash;
}

uint64_t BitBoard::getColorHash() {
	if(!currentState.color) {
		return (currentState.hash ^ reverse_color_const);
	}

	return currentState.hash;
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
					tmp_bitboard &= ~vec1_cells[pos];
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
				magic = magicGenerator();
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
					tmp_bitboard &= ~vec1_cells[pos];
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
	currentState.color = !currentState.color;
	currentState.hash ^= 747489434796739468;
}

void BitBoard::unMakeNullMove() {
	currentState.color = !currentState.color;
	currentState.hash ^= 747489434796739468;
}

double BitBoard::newEvaluateAll() {

	double stage_game = popcount64(currentState.piece_bit_mask[whiteSide] | currentState.piece_bit_mask[!whiteSide]) / 32;

	double result = 0;

	//Подсчет количества фигур (для ускорения работы)
	int white_pawn_count = popcount64(currentState.figures[PAWN] & currentState.piece_bit_mask[whiteSide]);
	int black_pawn_count = popcount64(currentState.figures[PAWN] & currentState.piece_bit_mask[!whiteSide]);
	
	int white_knight_count = popcount64(currentState.figures[KNIGHT] & currentState.piece_bit_mask[whiteSide]);
	int black_knight_count = popcount64(currentState.figures[KNIGHT] & currentState.piece_bit_mask[!whiteSide]);
	
	int white_bishop_count = popcount64(currentState.figures[BISHOP] & currentState.piece_bit_mask[whiteSide]);
	int black_bishop_count = popcount64(currentState.figures[BISHOP] & currentState.piece_bit_mask[!whiteSide]);
	
	int white_rook_count = popcount64(currentState.figures[ROOK] & currentState.piece_bit_mask[whiteSide]);
	int black_rook_count = popcount64(currentState.figures[ROOK] & currentState.piece_bit_mask[!whiteSide]);
	
	int white_queen_count = popcount64(currentState.figures[QUEEN] & currentState.piece_bit_mask[whiteSide]);
	int black_queen_count = popcount64(currentState.figures[QUEEN] & currentState.piece_bit_mask[!whiteSide]);

	uint64_t occu = (currentState.piece_bit_mask[whiteSide] | currentState.piece_bit_mask[!whiteSide]);
	int pieces_count = popcount64(occu);

	if(pieces_count <= 3) {
		if(white_bishop_count == 1 || black_bishop_count == 1 || white_knight_count == 1 || black_knight_count == 1 || pieces_count < 3) {
			return 0;
		}
	}

	//Материал
	result += (white_pawn_count - black_pawn_count) * PAWN_EV.getScore(stage_game);
	result += white_knight_count * KNIGHT_EV.getScore(stage_game);
	result -= black_knight_count * KNIGHT_EV.getScore(stage_game);
	result += white_bishop_count * BISHOP_EV.getScore(stage_game);
	result -= black_bishop_count * BISHOP_EV.getScore(stage_game);
	result += white_rook_count * ROOK_EV.getScore(stage_game);
	result -= black_rook_count * ROOK_EV.getScore(stage_game);
	result += white_queen_count * QUEEN_EV.getScore(stage_game);
	result -= black_queen_count * QUEEN_EV.getScore(stage_game);

	//Бонус за 2-х слонов
	result += DoubleBishopsBonus * (bool)((currentState.figures[BISHOP] & currentState.piece_bit_mask[whiteSide] & whiteCells) && (currentState.figures[BISHOP] & currentState.piece_bit_mask[whiteSide] & blackCells));
	result -= DoubleBishopsBonus * (bool)((currentState.figures[BISHOP] & currentState.piece_bit_mask[!whiteSide] & whiteCells) && (currentState.figures[BISHOP] & currentState.piece_bit_mask[!whiteSide] & blackCells));
	
	//Поле-Фигура
	uint64_t mask = currentState.figures[PAWN] & currentState.piece_bit_mask[whiteSide];
	while(mask != 0) {
		uint8_t pos = firstOne(mask);
		if(vertical[pos % 8] & currentState.piece_bit_mask[!whiteSide] & currentState.figures[PAWN]) {
			result += pawnMatr[7 - pos / 8][pos % 8];
		} else {
			result += PassedPawnBonus[pos / 8];
		}

		mask &= ~vec1_cells[pos];
	}

	mask = currentState.figures[PAWN] & currentState.piece_bit_mask[!whiteSide];
	while(mask != 0) {
		uint8_t pos = firstOne(mask);
		if(vertical[pos % 8] & currentState.piece_bit_mask[whiteSide] & currentState.figures[PAWN]) {
			result -= pawnMatr[pos / 8][pos % 8];
		} else {
			result -= PassedPawnBonus[7 - pos / 8];
		}

		mask &= ~vec1_cells[pos];
	}

	mask = currentState.figures[KNIGHT] & currentState.piece_bit_mask[whiteSide];
	while(mask != 0) {
		uint8_t pos = firstOne(mask);
		result += knightMatr[7 - pos / 8][pos % 8];
		mask &= ~vec1_cells[pos];
	}

	mask = currentState.figures[KNIGHT] & currentState.piece_bit_mask[!whiteSide];
	while(mask != 0) {
		uint8_t pos = firstOne(mask);
		result -= knightMatr[pos / 8][pos % 8];
		mask &= ~vec1_cells[pos];
	}

	mask = currentState.figures[BISHOP] & currentState.piece_bit_mask[whiteSide];
	while(mask != 0) {
		uint8_t pos = firstOne(mask);
		result += bishopMatr[7 - pos / 8][pos % 8];
		mask &= ~vec1_cells[pos];
	}

	mask = currentState.figures[BISHOP] & currentState.piece_bit_mask[!whiteSide];
	while(mask != 0) {
		uint8_t pos = firstOne(mask);
		result -= bishopMatr[pos / 8][pos % 8];
		mask &= ~vec1_cells[pos];
	}

	mask = currentState.figures[ROOK] & currentState.piece_bit_mask[whiteSide];
	while(mask != 0) {
		uint8_t pos = firstOne(mask);
		result += rookMatr[7 - pos / 8][pos % 8];
		mask &= ~vec1_cells[pos];
	}

	mask = currentState.figures[ROOK] & currentState.piece_bit_mask[!whiteSide];
	while(mask != 0) {
		uint8_t pos = firstOne(mask);
		result -= rookMatr[pos / 8][pos % 8];
		mask &= ~vec1_cells[pos];
	}

	mask = currentState.figures[QUEEN] & currentState.piece_bit_mask[whiteSide];
	while(mask != 0) {
		uint8_t pos = firstOne(mask);
		result += queenMatr[7 - pos / 8][pos % 8];
		mask &= ~vec1_cells[pos];
	}

	mask = currentState.figures[QUEEN] & currentState.piece_bit_mask[!whiteSide];
	while(mask != 0) {
		uint8_t pos = firstOne(mask);
		result -= queenMatr[pos / 8][pos % 8];
		mask &= ~vec1_cells[pos];
	}

	mask = currentState.figures[KING] & currentState.piece_bit_mask[whiteSide];
	while(mask != 0) {
		uint8_t pos = firstOne(mask);
		result += kingPST[7 - pos / 8][pstIndex[pos % 8]].getScore(stage_game);
		mask &= ~vec1_cells[pos];
	}
	
	mask = currentState.figures[KING] & currentState.piece_bit_mask[!whiteSide];
	while(mask != 0) {
		uint8_t pos = firstOne(mask);
		result -= kingPST[pos / 8][pstIndex[pos % 8]].getScore(stage_game);
		mask &= ~vec1_cells[pos];
	}

	uint64_t whitePawns = currentState.piece_bit_mask[whiteSide] & currentState.figures[PAWN];
	uint64_t blackPawns = currentState.piece_bit_mask[!whiteSide] & currentState.figures[PAWN];

	//Изолированные пешки

	result += (isolated_pawn_map[compressVertical(whitePawns)] * IsolatedPawnBonus);
	result -= (isolated_pawn_map[compressVertical(blackPawns)] * IsolatedPawnBonus);
/*
	//Сдвоенные пешки
	
	for(int i = 0; i < BOARD_SIZE; ++i) {
		result += DualPawnBonus[popcount64(whitePawns & vertical[i])];
		result -= DualPawnBonus[popcount64(blackPawns & vertical[i])];
	}*/

	//Оценка близости вражеский фигур к королю

	uint64_t white_king_pos = firstOne(currentState.figures[KING] & currentState.piece_bit_mask[whiteSide]);
	uint64_t black_king_pos = firstOne(currentState.figures[KING] & currentState.piece_bit_mask[!whiteSide]);

	mask = currentState.figures[ROOK] & currentState.piece_bit_mask[!whiteSide];

	while(mask != 0) {
		uint8_t pos = firstOne(mask);
		result -= (kingSecurityArray[white_king_pos][pos] * 4);
		mask &= ~vec1_cells[pos];
	}

	mask = currentState.figures[QUEEN] & currentState.piece_bit_mask[!whiteSide];
	
	while(mask != 0) {
		uint8_t pos = firstOne(mask);
		result -= (kingSecurityArray[white_king_pos][pos] * 6);
		mask &= ~vec1_cells[pos];
	}

	mask = currentState.figures[KNIGHT] & currentState.piece_bit_mask[!whiteSide];
	
	while(mask != 0) {
		uint8_t pos = firstOne(mask);
		result -= (kingSecurityArray[white_king_pos][pos] * 2);
		mask &= ~vec1_cells[pos];
	}

	mask = currentState.figures[BISHOP] & currentState.piece_bit_mask[!whiteSide];
	
	while(mask != 0) {
		uint8_t pos = firstOne(mask);
		result -= (kingSecurityArray[white_king_pos][pos] * 1);
		mask &= ~vec1_cells[pos];
	}

	mask = currentState.figures[ROOK] & currentState.piece_bit_mask[whiteSide];
	
	while(mask != 0) {
		uint8_t pos = firstOne(mask);
		result += (kingSecurityArray[black_king_pos][pos] * 4);
		mask &= ~vec1_cells[pos];
	}

	mask = currentState.figures[QUEEN] & currentState.piece_bit_mask[whiteSide];
	
	while(mask != 0) {
		uint8_t pos = firstOne(mask);
		result += (kingSecurityArray[black_king_pos][pos] * 6);
		mask &= ~vec1_cells[pos];
	}

	mask = currentState.figures[KNIGHT] & currentState.piece_bit_mask[whiteSide];
	
	while(mask != 0) {
		uint8_t pos = firstOne(mask);
		result += (kingSecurityArray[black_king_pos][pos] * 2);
		mask &= ~vec1_cells[pos];
	}

	mask = currentState.figures[BISHOP] & currentState.piece_bit_mask[whiteSide];
	
	while(mask != 0) {
		uint8_t pos = firstOne(mask);
		result += (kingSecurityArray[black_king_pos][pos] * 1);
		mask &= ~vec1_cells[pos];
	}

	//Бонус за расположение слонов
	int pawnsOnWhiteCells = popcount64(whiteCells & currentState.figures[PAWN]);
	int pawnsOnBlackCells = popcount64(blackCells & currentState.figures[PAWN]);

	int whiteBishopsOnBlackCells = popcount64(blackCells & currentState.figures[BISHOP] & currentState.piece_bit_mask[whiteSide]);
	int whiteBishopsOnWhiteCells = popcount64(whiteCells & currentState.figures[BISHOP] & currentState.piece_bit_mask[whiteSide]);
	int blackBishopsOnBlackCells = popcount64(blackCells & currentState.figures[BISHOP] & currentState.piece_bit_mask[!whiteSide]);
	int blackBishopsOnWhiteCells = popcount64(whiteCells & currentState.figures[BISHOP] & currentState.piece_bit_mask[!whiteSide]);
	result += 5 * (((whiteBishopsOnBlackCells - blackBishopsOnBlackCells) * pawnsOnWhiteCells) +
			   ((whiteBishopsOnWhiteCells - blackBishopsOnWhiteCells) * pawnsOnBlackCells));

	//Бонус мобильности + Оценка безопасности короля

	result += bitBoardMobilityEval(whiteSide, stage_game);
	result -= bitBoardMobilityEval(!whiteSide, stage_game);

	//Наличие роировки

	result += (popcount64(currentState.piece_bit_mask[whiteSide] | currentState.piece_bit_mask[!whiteSide]) > 16) * (currentState.whitePassantMade * 50 - currentState.blackPassantMade * 50);

	return result;
}

BitMove BitBoard::getMove(uint8_t fromY, uint8_t fromX, uint8_t toY, uint8_t toX, bool replaced, uint8_t replacedFigure, bool& enable) {
	bitBoardMoveGenerator(moveArray, stress);
	enable = false;
	for(unsigned int i = 0; i < moveArray.count; ++i) {
		if(fromY == moveArray.moveArray[i].fromY && fromX == moveArray.moveArray[i].fromX && toY == moveArray.moveArray[i].toY && toX == moveArray.moveArray[i].toX && (!replaced || moveArray.moveArray[i].replacedFigure == replacedFigure)) {
			enable = true;
			return moveArray.moveArray[i];
		}
	}

	return BitMove(0, 0, 0, 0, 0);
}

uint8_t BitBoard::compressVertical(uint64_t value) {
	uint8_t result = 0;

	for(unsigned int i = 0; i < 8; ++i) {
		result += ((bool)(value & vertical[i]) << (7 - i));
	}

	return result;
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