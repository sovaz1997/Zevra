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

void Game::sortMoves(MoveArray& result, int depth) {
	int num_attacks = result.num_attacks;

	if(game_board.whiteMove) {
		for(unsigned int i = result.num_attacks + 1; i < result.count - 1; ++i) {
			for(int j = i - 1; j >= num_attacks && whiteHistorySort[result.moveArray[j].fromY][result.moveArray[j].fromX][result.moveArray[j].toY][result.moveArray[j].toX] < whiteHistorySort[result.moveArray[j+1].fromY][result.moveArray[j+1].fromX][result.moveArray[j+1].toY][result.moveArray[j+1].toX]; --j) {
				std::swap(result.moveArray[j], result.moveArray[j+1]);
			}
		}
	} else {
		for(unsigned int i = num_attacks + 1; i < result.count - 1; ++i) {
			for(int j = i - 1; j >= num_attacks && blackHistorySort[result.moveArray[j].fromY][result.moveArray[j].fromX][result.moveArray[j].toY][result.moveArray[j].toX] < blackHistorySort[result.moveArray[j+1].fromY][result.moveArray[j+1].fromX][result.moveArray[j+1].toY][result.moveArray[j+1].toX]; --j) {
				std::swap(result.moveArray[j], result.moveArray[j+1]);
			}
		}
	}

	for(unsigned int i = num_attacks; i < result.count; ++i) {
		if(game_board.whiteMove) {
			if(result.moveArray[i].equal(whiteKiller[depth].move) && whiteKiller[depth].enable) {
				result.moveArray.erase(result.moveArray.begin() + i);
				result.moveArray.emplace(result.moveArray.begin() + num_attacks, whiteKiller[depth].move);
				break;
			}
		} else {
			if(result.moveArray[i].equal(blackKiller[depth].move) && blackKiller[depth].enable) {
				result.moveArray.erase(result.moveArray.begin() + i);
				result.moveArray.emplace(result.moveArray.begin() + num_attacks, blackKiller[depth].move);
				break;
			}
		}
	}


	uint64_t hash = game_board.getColorHash();
	if(boardHash[hash & hash_cutter].flag != EMPTY /*&& boardHash[hash & hash_cutter].flag != ALPHA*/) {
		if(boardHash[hash & hash_cutter].key == hash) {
			for(unsigned int i = 0; i < result.count; ++i) {
				if(boardHash[hash & hash_cutter].move.equal(result.moveArray[i])) {
					BitMove tmp = (BitMove)result.moveArray[i];
					result.moveArray.erase(result.moveArray.begin() + i);
					result.moveArray.emplace(result.moveArray.begin(), tmp);
					result.moveArray[0].fromHash = true;
					break;
				}
			}
		}
	}
}

void Game::sortAttacks(MoveArray& moves) {
	std::sort(moves.moveArray.begin(), moves.moveArray.begin() + moves.num_attacks);
}
