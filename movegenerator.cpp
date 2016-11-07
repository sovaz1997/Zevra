#include "game.hpp"

void Game::sortMoves(MoveArray& result, int depth) {
	int num_attacks = result.num_attacks;
	for(unsigned int i = num_attacks; i < result.count; ++i) {
		if(game_board.whiteMove) {
			if(result.moveArray[i].equal(whiteKiller[depth].move) && whiteKiller[depth].enable) {
				result.moveArray.erase(result.moveArray.begin() + i);
				if(num_attacks) {
					result.moveArray.insert(result.moveArray.begin() + 1, whiteKiller[depth].move);
				} else {
					result.moveArray.insert(result.moveArray.begin(), whiteKiller[depth].move);
				}

				break;
			}
		} else {
			if(result.moveArray[i].equal(blackKiller[depth].move) && blackKiller[depth].enable) {
				result.moveArray.erase(result.moveArray.begin() + i);
				if(num_attacks) {
					result.moveArray.insert(result.moveArray.begin() + 1, blackKiller[depth].move);
				} else {
					result.moveArray.insert(result.moveArray.begin(), blackKiller[depth].move);
				}
				break;
			}
		}
	}

	uint64_t hash = game_board.getColorHash();
	if(boardHash[hash & hash_cutter].enable) {
		if(boardHash[hash & hash_cutter].hash == hash && boardHash[hash & hash_cutter].depth >= depth) {
			for(unsigned int i = 0; i < result.count; ++i) {
				if(boardHash[hash & hash_cutter].move.equal(result.moveArray[i])) {
					result.moveArray.erase(result.moveArray.begin() + i);
					result.moveArray.insert(result.moveArray.begin(), boardHash[hash & hash_cutter].move);
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
