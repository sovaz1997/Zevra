#include "game.hpp"

void Game::sortMoves(MoveArray& result, int depth) {
	int num_attacks = result.num_attacks;

	for(int i = 0; i < result.count; ++i) {
		result.moveArray[i].history_weight = whiteHistorySort[result.moveArray[i].fromY][result.moveArray[i].fromX][result.moveArray[i].toY][result.moveArray[i].toX];
	}

	result.setHistoryCompare(true);
	std::sort(result.moveArray.begin() + result.num_attacks, result.moveArray.begin() + result.count);

	/*if(result.num_attacks >= 0 && result.num_attacks < result.moveArray.size() && result.count >= 1 && result.count < result.moveArray.size()) {
		for(unsigned int i = result.num_attacks + 1; i < result.count; ++i) {
			for(int j = i; j > num_attacks && whiteHistorySort[result.moveArray[j - 1].fromY][result.moveArray[j - 1].fromX][result.moveArray[j - 1].toY][result.moveArray[j - 1].toX] < whiteHistorySort[result.moveArray[j].fromY][result.moveArray[j].fromX][result.moveArray[j].toY][result.moveArray[j].toX]; --j) {
				std::swap(result.moveArray[j], result.moveArray[j-1]);
			}
		}
	}*/

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
	moves.setHistoryCompare(false);
	std::sort(moves.moveArray.begin(), moves.moveArray.begin() + moves.num_attacks);
}
