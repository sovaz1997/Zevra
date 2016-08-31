#include "game.hpp"

void Game::printVariant() {
	if(print_variant_enable) {
		for(int i = 0; i < variant.size(); ++i) {
			std::cout << variant[i] << " ";
		}

		std::cout << "\n";
	}
}

void Game::cleanWhiteHistory() {
	for(int i = 0; i < BOARD_SIZE; ++i) {
		for(int j = 0; j < BOARD_SIZE; ++j) {
			for(int k = 0; k < BOARD_SIZE; ++k) {
				for(int m = 0; m < BOARD_SIZE; ++m) {
					whiteHistorySort[i][j][k][m] = 0;
				}
			}
		}
	}
}

void Game::cleanBlackHistory() {
	for(int i = 0; i < BOARD_SIZE; ++i) {
		for(int j = 0; j < BOARD_SIZE; ++j) {
			for(int k = 0; k < BOARD_SIZE; ++k) {
				for(int m = 0; m < BOARD_SIZE; ++m) {
					blackHistorySort[i][j][k][m] = 0;
				}
			}
		}
	}
}

void Game::flattenHistory() {
	for(int i = 0; i < BOARD_SIZE; ++i) {
		for(int j = 0; j < BOARD_SIZE; ++j) {
			for(int k = 0; k < BOARD_SIZE; ++k) {
				for(int m = 0; m < BOARD_SIZE; ++m) {
					whiteHistorySort[i][j][k][m] /= 1000000000;
					blackHistorySort[i][j][k][m] /= 1000000000;
				}
			}
		}
	}
}

void Game::clearCash() {

	for(int i = 0; i < whiteKiller.size(); ++i) {
		whiteKiller[i] = Killer();
	}

	for(int i = 0; i < blackKiller.size(); ++i) {
		blackKiller[i] = Killer();
	}

	for(int i = 0; i < boardHash.size(); ++i) {
		boardHash[i].enable = false;
	}

	gameHash = std::vector<uint64_t>();
}

uint64_t Game::getColorHash(Board & b) {
	uint64_t res = getHash(b);

	if(!b.isWhiteMove()) {
		res = (res * res + 1);
	}

	return res;
}

uint64_t Game::getHash(Board & b) {
	uint64_t hash = 0;

	for(unsigned int y = 0; y < 8; ++y) {
		for(unsigned int x = 0; x < 8; ++x) {
			/*int k = 0;

			if(b.getFigure(y, x) == 0) { k = 1; }
			else if(b.getFigure(y, x) & TYPE_SAVE == PAWN) { k = 2; }
			else if(b.getFigure(y, x) & TYPE_SAVE == KNIGHT) { k = 3; }
			else if(b.getFigure(y, x) & TYPE_SAVE == BISHOP) { k = 4; }
			else if(b.getFigure(y, x) & TYPE_SAVE == ROOK) { k = 5; }
			else if(b.getFigure(y, x) & TYPE_SAVE == QUEEN) { k = 6; }
			else if(b.getFigure(y, x) & TYPE_SAVE == KING) { k = 7; }

			if(b.getFigure(y, x) & COLOR_SAVE == BLACK) {
				k += 6;
			}
			uint64_t res = 1;
			for(unsigned int i = 0; i < (y+1) * 8 + (x+1); ++i) {
				res = res + k*(100000000*((y+1)+((x+1)^17))*k*(12^k));
				hash += res;
			}

			hash += res;*/
			if(b.getFigure(y, x) != 0) {
				hash = (hash ^ zobrist[b.getFigure(y, x)][y][x]);
			}
		}
	}

	return hash;
}

std::vector<std::string> Game::getStringArray(std::string str) {
	std::vector<std::string> result;
	std::string tmp;
	for(unsigned int i = 0; i < str.size(); ++i) {
		if(str[i] == ' ') {
			result.push_back(tmp);
			tmp.clear();
		} else {
			tmp.push_back(str[i]);
		}
	}

	result.push_back(tmp);

	return result;
}
