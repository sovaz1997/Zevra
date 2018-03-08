#include "game.hpp"

void Game::flattenHistory() {
	for(int s = 0; s < 2; ++s) {
		for(int i = 0; i < BOARD_SIZE; ++i) {
			for(int j = 0; j < BOARD_SIZE; ++j) {
				for(int k = 0; k < BOARD_SIZE; ++k) {
					for(int m = 0; m < BOARD_SIZE; ++m) {
						historySort[s][i][j][k][m] /= 10000;
					}
				}
			}
		}
	}
}

void Game::cleanHistory() {
	for(int s = 0; s < 2; ++s) {
		for(int i = 0; i < BOARD_SIZE; ++i) {
			for(int j = 0; j < BOARD_SIZE; ++j) {
				for(int k = 0; k < BOARD_SIZE; ++k) {
					for(int m = 0; m < BOARD_SIZE; ++m) {
						historySort[s][i][j][k][m] = 0;
					}
				}
			}
		}
	}
}

void Game::clearCash() {
	cleanHistory();

	for(unsigned int i = 0; i < boardHash.size(); ++i) {
		boardHash[i].clean();
	}

	hash_filled = 0;
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

void Game::setHashSize(int mb_size) {
	uint64_t hash_size = ((uint64_t)mb_size * 1024 * 1024);

	int bits = 0;
	for(uint64_t i = 1; ; i *= 2) {
		if(i * sizeof(Hash) >= hash_size || i == 0) {
			break;
		}

		++bits;
	}

	hash_width = bits;
	hash_cutter = std::pow(2, hash_width) - 1;

	game_board.hash_width = bits;
	game_board.hash_cutter = hash_cutter;

	max_hash_filled = pow(2, hash_width);
	boardHash.resize(pow(2, hash_width));
	clearCash();
	game_board.third_repeat = std::vector<int> (pow(2, hash_width), 0);
}