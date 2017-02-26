#include "game.hpp"

Game::Game() {
	initEngine();
	idPrint();
}

//experemental

int64_t Game::negamax_future(int64_t alpha, int64_t beta, int depth, int ply) {
	if(depth <= 0) {
		return quies_future(alpha, beta, ply + 1)/*game_board.getEvalute()*/;
	}

	uint8_t color = WHITE;

	if(!game_board.whiteMove) {
		color = BLACK;
	}

	game_board.bitBoardMoveGenerator(moveArray[ply]);
	sortMoves(moveArray[ply], depth);
	BitMove local_move;

	for(unsigned int i = 0; i < moveArray[ply].count; ++i) {
		game_board.move(moveArray[ply].moveArray[i]);

		if(game_board.inCheck(color)) {
			game_board.goBack();
			continue;
		}

		int64_t tmp = -negamax_future(-beta, -alpha, depth - 1, ply + 1);
		game_board.goBack();

		if(tmp > alpha) {
			local_move = moveArray[ply].moveArray[i];
			alpha = tmp;
		}
		
		if(alpha >= beta) {
			return beta;
		}
	}

	if(ply == 0) {
		bestMove = local_move;
		bestScore = alpha;
	}

	return alpha;
}

int64_t Game::quies_future(int64_t alpha, int64_t beta, int ply) {
	int64_t now_evalute = game_board.getEvalute();
	
	if(now_evalute >= beta) {
		return beta;
	}

	if(alpha < now_evalute) {
		alpha = now_evalute;
	}

	uint8_t color = WHITE;

	if(!game_board.whiteMove) {
		color = BLACK;
	}

	game_board.bitBoardAttackMoveGenerator(moveArray[ply]);
	sortMoves(moveArray[ply], 100);
	BitMove local_move;

	for(unsigned int i = 0; i < moveArray[ply].count; ++i) {
		game_board.move(moveArray[ply].moveArray[i]);

		if(game_board.inCheck(color)) {
			game_board.goBack();
			continue;
		}

		int64_t tmp = -quies_future(-beta, -alpha, ply + 1);
		game_board.goBack();

		if(tmp >= beta) {
			return beta;
		}

		if(alpha < tmp) {
			alpha = tmp;
		}
	}

	return alpha;
}