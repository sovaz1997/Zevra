#include "game.hpp"

void Game::sortMoves(MoveArray& result, int depth) {
	int num_attacks = 0;
	if(game_board.whiteMove) {
		for(unsigned int i = result.num_attacks + 1; i < result.count - 1; ++i) {
			for(int j = i - 1; j >= result.num_attacks && whiteHistorySort[result.moveArray[j].fromY][result.moveArray[j].fromX][result.moveArray[j].toY][result.moveArray[j].toX] < whiteHistorySort[result.moveArray[j+1].fromY][result.moveArray[j+1].fromX][result.moveArray[j+1].toY][result.moveArray[j+1].toX]; --j) {
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
				result.moveArray.insert(result.moveArray.begin() + num_attacks, whiteKiller[depth].move);
				break;
			}
		} else {
			if(result.moveArray[i].equal(blackKiller[depth].move) && blackKiller[depth].enable) {
				result.moveArray.erase(result.moveArray.begin() + i);
				result.moveArray.insert(result.moveArray.begin() + num_attacks, blackKiller[depth].move);
				break;
			}
		}
}
	/*uint64_t hash = getColorHash(game_board);
	if(boardHash[hash & hash_cutter].enable) {
		if(boardHash[hash & hash_cutter].hash == hash && boardHash[hash & hash_cutter].depth >= depth) {
			for(unsigned int i = 0; i < result.size(); ++i) {
				if(boardHash[hash & hash_cutter].move.equal(result.moveArray[i])) {
					result.erase(result.begin() + i);
					result.insert(result.begin(), boardHash[hash & hash_cutter].move);
					result[0].fromHash = true;
					break;
				}
			}
		}
	}*/

	/*if(best_pv.size() > depth) {
		for(unsigned int i = 0; i < result.count; ++i) {
			if(best_pv.pv[depth].equal(result.moveArray[i])) {
				result.moveArray.erase(result.moveArray.begin() + i);
				result.moveArray.insert(result.moveArray.begin(), best_pv.pv[depth]);
				break;
			}
		}
	}*/

	/*if(hasBestMove && depth == 0) {
		for(unsigned int i = 0; i < result.size(); ++i) {
			if(bestMove.equal(result.moveArray[i])) {
				result.erase(result.begin() + i);
				result.insert(result.begin(), bestMove);
				break;
			}
		}
	}*/
}

void Game::sortAttacks(MoveArray& moves) {
	for(int i = 0; i < moves.num_attacks; ++i) {
		for(int j = 0; j < moves.num_attacks - 1; ++j) {
			if(moves.moveArray[j].getAttackPrice() < moves.moveArray[j + 1].getAttackPrice()) {
				std::swap(moves.moveArray[j], moves.moveArray[j+1]);
			}
		}
	}
}
