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

#include "game.hpp"

void Game::printVariant() {
	if(print_variant_enable) {
		for(unsigned int i = 0; i < variant.size(); ++i) {
			std::cout << variant[i] << " ";
		}

		std::cout << std::endl;
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
					whiteHistorySort[i][j][k][m] /= 10;
					blackHistorySort[i][j][k][m] /= 10;
				}
			}
		}
	}
}

void Game::clearCash() {
	//cleanWhiteHistory();
	//cleanBlackHistory();

	for(unsigned int i = 0; i < boardHash.size(); ++i) {
		boardHash[i].clean();
	}
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
