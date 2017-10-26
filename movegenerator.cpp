#include "game.hpp"

void Game::sortMoves(MoveArray& result, int depth) {
	for(unsigned int i = 0; i < result.count; ++i) {
		result.moveArray[i].history_weight = whiteHistorySort[result.moveArray[i].fromY][result.moveArray[i].fromX][result.moveArray[i].toY][result.moveArray[i].toX];

		if(game_board.currentState.whiteMove) {
			if(whiteSecondKiller[depth].enable && result.moveArray[i].equal(whiteSecondKiller[depth].move)) {
				result.moveArray[i].history_weight = 1000000;
			}
			if(whiteKiller[depth].enable && result.moveArray[i].equal(whiteKiller[depth].move)) {
				result.moveArray[i].history_weight = 10000000;
			}
			if(whiteMateKiller[depth].enable && result.moveArray[i].equal(whiteMateKiller[depth].move)) {
				result.moveArray[i].history_weight = 100000000;
			}
			if(result.moveArray[i].fromHash) {
				result.moveArray[i].history_weight = 1000000000;
			}
		} else {
			if(blackSecondKiller[depth].enable && result.moveArray[i].equal(blackSecondKiller[depth].move)) {
				result.moveArray[i].history_weight = 1000000;
			}
			if(blackKiller[depth].enable && result.moveArray[i].equal(blackKiller[depth].move)) {
				result.moveArray[i].history_weight = 10000000;
			}
			if(blackMateKiller[depth].enable && result.moveArray[i].equal(blackMateKiller[depth].move)) {
				result.moveArray[i].history_weight = 100000000;
			}
			if(result.moveArray[i].fromHash) {
				result.moveArray[i].history_weight = 1000000000;
			}
		}
	}

	result.setHistoryCompare(true);
	std::sort(result.moveArray.begin() + result.num_attacks, result.moveArray.begin() + result.count);


	uint64_t hash = game_board.getColorHash();
	if(boardHash[hash & hash_cutter].flag != EMPTY && boardHash[hash & hash_cutter].flag != ALPHA && boardHash[hash & hash_cutter].depth >= depth) {
		if(boardHash[hash & hash_cutter].key == hash) {
			for(unsigned int i = 0; i < result.count; ++i) {
				if(boardHash[hash & hash_cutter].move_equal(result.moveArray[i])) {
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
	//std::sort(moves.moveArray.begin(), moves.moveArray.begin() + moves.num_attacks);
	
	int recapture_count = 0;
	BitMove mv;
	for(int i = moves.num_attacks - 1; i > recapture_count; --i) {
		if(game_board.vec2_cells[moves.moveArray[i].toY][moves.moveArray[i].toX] == game_board.currentState.recapture_position) {
			mv = moves.moveArray[i];
			moves.moveArray.erase(moves.moveArray.begin() + i);
			moves.moveArray.emplace(moves.moveArray.begin(), mv);
			++recapture_count;
		}
	}

	if(recapture_count) {
		std::sort(moves.moveArray.begin(), moves.moveArray.begin() + recapture_count);
	}
	if(moves.num_attacks - recapture_count) {
		std::sort(moves.moveArray.begin() + recapture_count, moves.moveArray.begin() + moves.num_attacks);
	}
}
