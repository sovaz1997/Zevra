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

Game::Game() {
	initEngine();
	idPrint();
}

//experemental

int64_t Game::negamax_future(int64_t alpha, int64_t beta, int depth, int ply, int rule, bool nullMoveAccess, BitMove* prev) {
	if(depth <= 0) {
		return quies_future(alpha, beta, ply + 1)/*game_board.getEvalute()*/;
	}

	if(depth > 2) {
		if((rule == FIXED_TIME && timer.getTime() >= time) || stopped) {
			return 0;
		}
	}

	int extended = 0;

	++nodesCounter;

	uint8_t color = WHITE;

	if(!game_board.whiteMove) {
		color = BLACK;
	}

	if(game_board.inCheck(color)) {
		++extended;
	}

	game_board.bitBoardMoveGenerator(moveArray[ply]);
	sortMoves(moveArray[ply], depth);
	BitMove local_move;
	int num_moves = 0;

	for(unsigned int i = 0; i < moveArray[ply].count; ++i) {
		if(depth > 2) {
			if((rule == FIXED_TIME && timer.getTime() >= time) || stopped) {
				return 0;
			}
		}

		game_board.move(moveArray[ply].moveArray[i]);

		if(game_board.inCheck(color)) {
			game_board.goBack();
			continue;
		}

		++num_moves;

		int64_t tmp;

		/*int R = 3;
		if(nullMoveAccess && depth >= R && !extended) {
			game_board.makeNullMove();
			int64_t eval = negamax_future(beta - 1, beta, depth - 1 - R, ply + 1, rule, false, prev);
			game_board.unMakeNullMove();

			if(eval >= beta) {
				game_board.goBack();
				return beta;
			}
		}*/

		/*if(i > 3 && !extended && !moveArray[ply].moveArray[i].isAttack) {
			tmp = -negamax_future(-beta, -alpha, depth - 2, ply + 1, rule, nullMoveAccess, &moveArray[ply].moveArray[i]);

			if(tmp > alpha && tmp < beta) {
				tmp = -negamax_future(-beta, -alpha, depth - 1, ply + 1, rule, nullMoveAccess, &moveArray[ply].moveArray[i]);	
			}
		} else {*/
		
			tmp = -negamax_future(-beta, -alpha, depth - 1 + extended, ply + 1, rule, nullMoveAccess, &moveArray[ply].moveArray[i]);
		//}
		game_board.goBack();

		if(tmp > alpha) {
			local_move = moveArray[ply].moveArray[i];
			alpha = tmp;

			if(!local_move.isAttack) {
				if(color == WHITE) {
					whiteKiller[ply] = Killer(local_move);
				} else {
					blackKiller[ply] = Killer(local_move);
				}

				if(color == WHITE) {
					whiteHistorySort[local_move.fromY][local_move.fromX][local_move.toY][local_move.toX] += pow(depth, depth);
				} else {
					blackHistorySort[local_move.fromY][local_move.fromX][local_move.toY][local_move.toX] += pow(depth, depth);
				}
			}
		}
		
		if(alpha >= beta) {
			return beta;
		}
	}

	if(num_moves == 0) {
		if(game_board.inCheck(color)) {
			return BLACK_WIN + ply;
		} else {
			return 0;
		}
	}

	if(ply == 0) {
		bestMove = local_move;
		bestScore = alpha;
	}

	if(ply == 0) {
		if(num_moves >= 0) {
			std::cout << "info depth " << max_depth << " time " << (int64_t)((clock() - start_timer) / (CLOCKS_PER_SEC / 1000)) << " nodes " << nodesCounter << " nps " << (int64_t)(nodesCounter / ((clock() - start_timer) / CLOCKS_PER_SEC));
			std::cout << " ";
			printScore(bestScore);
			std::cout << " pv " << bestMove.getMoveString() << std::endl;
			//printPV(depth);
		} else {
			std::cout << std::endl;
		}
	}

	return alpha;
}

int64_t Game::quies_future(int64_t alpha, int64_t beta, int ply) {
	++nodesCounter;
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
	sortAttacks(moveArray[ply]);
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