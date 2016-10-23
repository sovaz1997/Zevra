#include "game.hpp"

void Game::printVariant() {
	if(print_variant_enable) {
		for(unsigned int i = 0; i < variant.size(); ++i) {
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
					whiteHistorySort[i][j][k][m] /= 1000000;
					blackHistorySort[i][j][k][m] /= 1000000;
				}
			}
		}
	}
}

void Game::clearCash() {
	cleanWhiteHistory();
	cleanBlackHistory();
	/*for(unsigned int i = 0; i < whiteKiller.size(); ++i) {
		whiteKiller[i] = Killer();
	}

	for(unsigned int i = 0; i < blackKiller.size(); ++i) {
		blackKiller[i] = Killer();
	}*/

	/*for(unsigned int i = 0; i < boardHash.size(); ++i) {
		boardHash[i].enable = false;
	}*/

	//gameHash = std::vector<uint64_t>();
}

/*uint64_t Game::getColorHash(Board & b) {
	uint64_t res = getHash(b);

	if(!b.isWhiteMove()) {
		res = (res * res + 1);
	}

	return res;
}

uint64_t Game::getColorHash(Board & b, uint64_t hash) {
	if(!b.isWhiteMove()) {
		hash = (hash * hash + 1);
	}

	return hash;
}

uint64_t Game::getHash(Board & b) {
	uint64_t hash = 0;

	for(unsigned int y = 0; y < 8; ++y) {
		for(unsigned int x = 0; x < 8; ++x) {
			if(b.board[y][x] != 0) {
				hash = (hash ^ zobrist[b.board[y][x]][y][x]);
			}
		}
	}

	return hash;
}*/

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
