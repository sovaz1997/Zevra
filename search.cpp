#include "game.hpp"

double Game::negamax(BitBoard & b, double alpha, double beta, int depth, int real_depth, int rule, bool inNullMove, bool lazyEval, PV* pline) {
	++nodesCounter;
	/*if(game_board.testOfDraw()) {
		return 0;
	}*/

	double oldAlpha = alpha;

	bool extended = false;

	double eval = -INFINITY;
	double margin = PAWN_EV / 2;

	int nextDepth = depth - 1;
	if(depth > 2) {
		if(rule == FIXED_TIME && timer.getTime() >= time) {
			return 0;
		}
	}

	uint8_t color;
	if(b.whiteMove) {
		color = WHITE;
	} else {
		color = BLACK;
	}

	if(depth <= 0 || real_depth >= 100) {
		pline->count = 0;
		return quies(b, alpha, beta, rule, real_depth);
	}

	//PV line;

	if(b.inCheck(color)) {
		++nextDepth;
		extended = true;
		inNullMove = true;
	}

	int tmp;

	uint64_t hash = b.getColorHash();

	Hash* currentHash = &boardHash[hash & hash_cutter];

	if(currentHash->flag != EMPTY && currentHash->key == hash && !extended) {
		if(real_depth > 0 && currentHash->depth >= depth) {
			double score = currentHash->score;

			if(score > INFINITY - 100) {
				score -= real_depth;
			} else if(score < -INFINITY + 100) {
				score += real_depth;
			}

			if(currentHash->flag == EXACT || currentHash->flag == BETA) {
				if(score > alpha) {
					alpha = score;
				}
				if(alpha >= beta) {
					return beta;
				}
			} else if(currentHash->flag == ALPHA) {
				if(score <= alpha) {
					return alpha;
				}
			}
		}

		if(currentHash->flag != ALPHA && real_depth > 0) {
			b.move(currentHash->move);
			tmp = -negamax(b, -beta, -alpha, depth - 1, real_depth + 1, rule, inNullMove, false, pline);
			b.goBack();

			if(tmp > alpha) {
				recordHash(depth, tmp, tmp<beta?EXACT:BETA, hash, currentHash->move, real_depth);
				alpha = tmp;
				if(alpha >= beta) {
		  			return beta;
				}
			}
		}
	}

	bool inCheck;
	if(color == WHITE) {
		inCheck = b.inCheck(WHITE);
	} else {
		inCheck = b.inCheck(BLACK);
	}

	/*if(!inNullMove && depth <= 2 && !extended && !b.attacked && !inCheck && b.getEvalute() - margin >= beta) {
		return beta;
	}*/

	if(option.nullMovePrunningEnable) {
		if(!inNullMove && !b.inCheck(color) && !extended && !b.attacked && real_depth > 2 && b.getFiguresCount() > 3) {
			if(negamax(b, beta - 1, beta, depth - 3, real_depth + 1, rule, true, true, pline) >= beta) {
				return beta;
			}
		}
	}

	int num_moves = 0;

	b.bitBoardMoveGenerator(moveArray[real_depth]);
	sortAttacks(moveArray[real_depth]);
	sortMoves(moveArray[real_depth], real_depth);

	BitMove local_move;

	if(moveArray[real_depth].count > 0) {
		local_move = moveArray[real_depth].moveArray[0];
	}

/*	if(real_depth == 0) {
		std::cout << "info depth " << max_depth << "\n";
	}*/
	for(unsigned int i = 0; i < moveArray[real_depth].count; ++i) {
		b.move(moveArray[real_depth].moveArray[i]);

		if(game_board.inCheck(color)) {
			b.goBack();
			continue;
		}

		if(num_moves == 0) {
			local_move = moveArray[real_depth].moveArray[i];
		}

		++num_moves;


		/*if(!lazyEval && b.getEvalute() + margin <= alpha && !b.inCheck(color) && !extended && !b.attacked && -negamax(b, -beta, -alpha, depth - 3, real_depth + 1, rule, inNullMove, true, pline) <= alpha) {
			tmp = alpha;
		} else {
			tmp = -negamax(b, -beta, -alpha, nextDepth, real_depth + 1, rule, inNullMove, lazyEval, pline);
		}*/

		if(!option.lmrEnable) {
			tmp = -negamax(b, -(alpha + 1), -alpha, nextDepth, real_depth + 1, rule, inNullMove, lazyEval, pline);

			if(tmp > alpha && tmp < beta) {
				tmp = -negamax(b, -beta, -alpha, nextDepth, real_depth + 1, rule, inNullMove, lazyEval, pline);
			}
		} else {
			if(num_moves <= 3 || b.inCheck(color) || extended || inNullMove || b.attacked) {
				tmp = -negamax(b, -beta, -alpha, nextDepth, real_depth + 1, rule, inNullMove, lazyEval, pline);
			} else {
				tmp = -negamax(b, -(alpha + 1), -alpha, nextDepth-1, real_depth + 1, rule, inNullMove, lazyEval, pline);

				if(tmp > alpha) {
					tmp = -negamax(b, -beta, -alpha, nextDepth, real_depth + 1, rule, inNullMove, lazyEval, pline);
				}
			}
		}

		b.goBack();

		if(tmp > eval) {
			eval = tmp;
		}


		if(tmp > alpha) {
			alpha = tmp;
			local_move = moveArray[real_depth].moveArray[i];
			
			if(real_depth == 0) {
				if(num_moves >= 0) {
					std::cout << "info depth " << max_depth << " time " << (int)((clock() - start_timer) / (CLOCKS_PER_SEC / 1000)) << " nodes " << nodesCounter << " nps " << (int)(nodesCounter / ((clock() - start_timer) / CLOCKS_PER_SEC));
					std::cout << " ";
					printScore(eval);
					std::cout << " pv " << bestMove.getMoveString() << "\n";
				} else {
					std::cout << "\n";
				}
			}

			if(!local_move.isAttack) {
				if(color == WHITE) {
					//whiteHistorySort[local_move.fromY][local_move.fromX][local_move.toY][local_move.toX] += pow(depth, 2);
					whiteKiller[real_depth] = Killer(local_move);
				} else {
					//blackHistorySort[local_move.fromY][local_move.fromX][local_move.toY][local_move.toX] += pow(depth, 2);
					blackKiller[real_depth] = Killer(local_move);
				}
			}

			recordHash(depth, tmp, tmp<beta?EXACT:BETA, hash, moveArray[real_depth].moveArray[i], real_depth);

			//pline->line[0] = local_move;
			//memcpy(pline->line + 1, line.line, line.count * sizeof(local_move));
			//pline->count = line.count + 1;


		}

		if(alpha >= beta) {
			return beta;
		}
	}

	if(num_moves == 0) {
		if(game_board.inCheck(color)) {
			return BLACK_WIN + real_depth;
		} else {
			return 0;
		}
	}


	if(real_depth == 0) {
		if(rule == FIXED_TIME && timer.getTime() >= time) {
			return 0;
		}

		bestMove = local_move;
		bestScore = alpha;

		//for(int i = 0; i < pline->count; ++i) {
			//std::cout << pline->line[i].getMoveString() << " ";
		//}
	}

	if(alpha == oldAlpha) {
		recordHash(depth, alpha, ALPHA, hash, local_move, real_depth);
	}

	return alpha;
}

uint64_t Game::perft(int depth) {
	uint64_t res = 0;
	if(depth == 0) {
		return 1;
	}

	game_board.bitBoardMoveGenerator(moveArray[depth]);

	for(unsigned int i = 0; i < moveArray[depth].count; ++i) {
		game_board.move(moveArray[depth].moveArray[i]);
		if(game_board.whiteMove) {
			if(game_board.inCheck(BLACK)) {
				game_board.goBack();
				continue;
			}
		} else {
			if(game_board.inCheck(WHITE)) {
				game_board.goBack();
				continue;
			}
		}

		res += perft(depth - 1);
		game_board.goBack();
	}

	return res;
}

double Game::quies(BitBoard & b, double alpha, double beta, int rule, int real_depth) {
	double val = b.getEvalute();

	if(val >= beta) {
		return beta;
	}

	if(alpha < val) {
		alpha = val;
	}

	b.bitBoardAttackMoveGenerator(moveArray[real_depth]);
	sortAttacks(moveArray[real_depth]);

	for(unsigned int i = 0; i < moveArray[real_depth].count && alpha < beta; ++i) {
		++nodesCounter;
		b.move(moveArray[real_depth].moveArray[i]);

		if(!b.whiteMove) {
			if(game_board.inCheck(WHITE)) {
				b.goBack();
				continue;
			}
		} else {
			if(game_board.inCheck(BLACK)) {
				b.goBack();
				continue;
			}
		}

		val = -quies(b, -beta, -alpha, rule, real_depth + 1);
		b.goBack();

		if(val >= beta) {
			return val;
		}

		if(val > alpha) {
			alpha = val;
		}
	}

	return alpha;
}

bool Game::recordHash(int depth, int score, int flag, uint64_t key, BitMove move, int real_depth) {
	Hash* hash = &boardHash[key & hash_cutter];

	if(flag == ALPHA && (hash->flag == EXACT || hash->flag == BETA)) {
		return 0;
	}

	if(score > INFINITY - 100) {
		score += real_depth;
	} else if(score < -INFINITY + 100) {
		score -= real_depth;
	}

	hash->depth = depth;
	hash->score = score;
	hash->flag = flag;
	hash->key = key;

	if(flag != ALPHA) {
		hash->move = move;
	}
}
