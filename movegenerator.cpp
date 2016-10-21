#include "game.hpp"

void Game::sortMoves(std::vector<Move>& result, int depth) {
	if(!result.empty()) {
	int num_attacks = 0;
	for(int i = 0; i < result.size(); ++i) {
		/*if(result[i].getAttackPrice(game_board) != 0) {
			++num_attacks;
		}*/
			/*for(int j = 0; j < num_attacks - 1; ++j) {
				if(result[j].getAttackPrice(game_board) < result[j + 1].getAttackPrice(game_board)) {
					std::swap(result[j], result[j+1]);
				}
			}*/
		}

		/*if(game_board.whiteMove) {
			for(unsigned int i = num_attacks + 1; i < result.size() - 1; ++i) {
				for(int j = i - 1; j >= num_attacks && whiteHistorySort[result[j].fromY][result[j].fromX][result[j].toY][result[j].toX] < whiteHistorySort[result[j+1].fromY][result[j+1].fromX][result[j+1].toY][result[j+1].toX]; --j) {
					std::swap(result[j], result[j+1]);
				}
			}
		} else {
			for(unsigned int i = num_attacks + 1; i < result.size() - 1; ++i) {
				for(int j = i - 1; j >= num_attacks && blackHistorySort[result[j].fromY][result[j].fromX][result[j].toY][result[j].toX] < blackHistorySort[result[j+1].fromY][result[j+1].fromX][result[j+1].toY][result[j+1].toX]; --j) {
					std::swap(result[j], result[j+1]);
				}
			}
		}*/

		/*for(unsigned int i = num_attacks; i < result.size(); ++i) {

			if(game_board.isWhiteMove()) {
				if(result[i].equal(whiteKiller[depth].move) && whiteKiller[depth].enable) {
					result.erase(result.begin() + i);
					result.insert(result.begin() + num_attacks, whiteKiller[depth].move);
					break;
				}
			} else {
				if(result[i].equal(blackKiller[depth].move) && blackKiller[depth].enable) {
					result.erase(result.begin() + i);
					result.insert(result.begin() + num_attacks, blackKiller[depth].move);
					break;
				}
			}
		}*/

		/*uint64_t hash = getColorHash(game_board);
		if(boardHash[hash & hash_cutter].enable) {
			if(boardHash[hash & hash_cutter].hash == hash && boardHash[hash & hash_cutter].depth >= depth) {
				for(unsigned int i = 0; i < result.size(); ++i) {
					if(boardHash[hash & hash_cutter].move.equal(result[i])) {
						result.erase(result.begin() + i);
						result.insert(result.begin(), boardHash[hash & hash_cutter].move);
						result[0].fromHash = true;
						break;
					}
				}
			}
		}*/

		if(best_pv.size() > depth) {
			for(unsigned int i = 0; i < result.size(); ++i) {
				if(best_pv.pv[depth].equal(result[i])) {
					result.erase(result.begin() + i);
					result.insert(result.begin(), best_pv.pv[depth]);
					break;
				}
			}
		}

		/*if(hasBestMove && depth == 0) {
			for(unsigned int i = 0; i < result.size(); ++i) {
				if(bestMove.equal(result[i])) {
					result.erase(result.begin() + i);
					result.insert(result.begin(), bestMove);
					break;
				}
			}
		}*/
		}
}
