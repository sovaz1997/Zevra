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

void Game::printScore(double score) {
	std::cout << "score ";
	if(score > BLACK_WIN + 10000 && score < WHITE_WIN - 10000) {
		std::cout << "cp " << (int) (score / PAWN_EV * 100);
	} else if(score < 0) {
		std::cout << "score mate " <<  (int)(-fabs(score - BLACK_WIN) / 2 - 1);
	} else {
		std::cout << "mate " <<  (int)(fabs(score - WHITE_WIN) / 2 + 1);
	}
}

std::string Game::getScore(double score) {
	std::string result;
	result += "score ";
	if(score > BLACK_WIN + 10000 && score < WHITE_WIN - 10000) {
		result += "cp ";
		result += std::to_string((int) (score / PAWN_EV * 100));
	} else if(score < 0) {
		result += "score mate ";
		result += std::to_string((int)(-fabs(score - BLACK_WIN) / 2 - 1));
	} else {
		result += "mate ";
		result += std::to_string((int)(fabs(score - WHITE_WIN) / 2 + 1));
	}

	return result;
}
