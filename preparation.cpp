#include "game.hpp"

void Game::initEngine() {
	whiteKiller.resize(100);
	blackKiller.resize(100);
	boardHash.resize(pow(2, hash_width));
	hash_cutter = pow(2, hash_width) - 1;

	for(int i = 0; i < BOARD_SIZE; ++i) {
		for(int j = 0; j < BOARD_SIZE; ++j) {
			for(int k = 0; k < BOARD_SIZE; ++k) {
				for(int m = 0; m < BOARD_SIZE; ++m) {
					evalute_cells_size[i][j][k][m]  = sqrt(pow(i - k, 2) + pow(j - m, 2)) + 5;
				}
			}
		}
	}

	cleanWhiteHistory();
	cleanBlackHistory();
	clearCash();
	moveArray.resize(200);
}
