#include "game.hpp"

void Game::initEngine() {
	whiteKiller.resize(100);
	blackKiller.resize(100);
	whiteSecondKiller.resize(100);
	blackSecondKiller.resize(100);

    setHashSize(option.hash_size);

	for(int i = 0; i < BOARD_SIZE; ++i) {
		for(int j = 0; j < BOARD_SIZE; ++j) {
			for(int k = 0; k < BOARD_SIZE; ++k) {
				for(int m = 0; m < BOARD_SIZE; ++m) {
					evalute_cells_size[i][j][k][m]  = sqrt(pow(i - k, 2) + pow(j - m, 2)) + 5;
				}
			}
		}
	}

	cleanHistory();
	clearCash();
	moveArray.resize(200);

	hash_filled = 0;
	max_hash_filled = boardHash.size();
}