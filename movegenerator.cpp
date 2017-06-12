#include "game.hpp"

void Game::sortMoves(MoveArray& result, int depth) {
	int num_attacks = result.num_attacks;

	for(int i = 0; i < result.count; ++i) {
		result.moveArray[i].history_weight = whiteHistorySort[result.moveArray[i].fromY][result.moveArray[i].fromX][result.moveArray[i].toY][result.moveArray[i].toX];
		if(game_board.whiteMove) {
			if(whiteSecondKiller[depth].enable && result.moveArray[i].equal(whiteSecondKiller[depth].move)) {
				result.moveArray[i].history_weight = 1000000;
			}
			if(whiteKiller[depth].enable && result.moveArray[i].equal(whiteKiller[depth].move)) {
				result.moveArray[i].history_weight = 10000000;
			}
		} else {
			if(blackSecondKiller[depth].enable && result.moveArray[i].equal(blackSecondKiller[depth].move)) {
				result.moveArray[i].history_weight = 1000000;
			}
			if(blackKiller[depth].enable && result.moveArray[i].equal(blackKiller[depth].move)) {
				result.moveArray[i].history_weight = 10000000;
			}
		}
	}

	result.setHistoryCompare(true);
	std::sort(result.moveArray.begin() + result.num_attacks, result.moveArray.begin() + result.count);

	/*bool firstKillerEnable = false;

	for(unsigned int i = num_attacks; i < result.count; ++i) {
		if(game_board.whiteMove) {
			if(result.moveArray[i].equal(whiteKiller[depth].move) && whiteKiller[depth].enable) {
				result.moveArray.erase(result.moveArray.begin() + i);
				result.moveArray.emplace(result.moveArray.begin() + num_attacks, whiteKiller[depth].move);
				firstKillerEnable = true;
				break;
			}
		} else {
			if(result.moveArray[i].equal(blackKiller[depth].move) && blackKiller[depth].enable) {
				result.moveArray.erase(result.moveArray.begin() + i);
				result.moveArray.emplace(result.moveArray.begin() + num_attacks, blackKiller[depth].move);
				firstKillerEnable = true;
				break;
			}
		}
	}

	for(unsigned int i = num_attacks + firstKillerEnable; i < result.count; ++i) {
		if(game_board.whiteMove) {
			if(result.moveArray[i].equal(whiteSecondKiller[depth].move) && whiteSecondKiller[depth].enable) {
				result.moveArray.erase(result.moveArray.begin() + i);
				result.moveArray.emplace(result.moveArray.begin() + num_attacks + firstKillerEnable, whiteSecondKiller[depth].move);
				break;
			}
		} else {
			if(result.moveArray[i].equal(blackSecondKiller[depth].move) && blackSecondKiller[depth].enable) {
				result.moveArray.erase(result.moveArray.begin() + i);
				result.moveArray.emplace(result.moveArray.begin() + num_attacks + firstKillerEnable, blackSecondKiller[depth].move);
				break;
			}
		}
	}*/


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
