#include "game.hpp"

void Game::printBitBoard(uint64_t bit_board) {
	for(int y = 0; y < BOARD_SIZE; ++y) {
		for(int x = 0; x < BOARD_SIZE; ++x) {
			if(bit_board & cells[y][x]) {
				std::cout << 1;
			} else {
				std::cout << 0;
			}
		}
		std::cout << "\n";
	}
}
