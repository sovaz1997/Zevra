#include "game.hpp"

double Game::negamax(BitBoard & b, double alpha, double beta, int depth, int real_depth, int rule, bool inNullMove) {
	++nodesCounter;

	bool extended = false;

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
		return quies(b, alpha, beta, rule, real_depth);
	}

	if(b.inCheck(color)) {
		++nextDepth;
		extended = true;
		inNullMove = true;
	}

	int R = 1;

	if(max_depth > 4) {
		R = 2;
	} else if(max_depth > 7) {
		R = 3;
	}

	if(real_depth >= 4 && !inNullMove && !b.inCheck(color) && !extended && !b.attacked &&  b.getFiguresCount() > 3 && b.getEvalute() >= beta) {
		double value = negamax(b, beta-1, beta, nextDepth - 2, real_depth + 1, rule, true);
		if(value >= beta) {
			return beta;
		}
	}

	int num_moves = 0;

	double max = BLACK_WIN;
	b.bitBoardMoveGenerator(moveArray[real_depth]);
	sortAttacks(moveArray[real_depth]);
	sortMoves(moveArray[real_depth], real_depth);

	BitMove local_move;

	if(moveArray[real_depth].count > 0) {
		local_move = moveArray[real_depth].moveArray[0];
	}

	int tmp;

	uint64_t hash = b.getColorHash();

	if(boardHash[hash & hash_cutter].enable && boardHash[hash & hash_cutter].hash == hash) {
		if(boardHash[hash & hash_cutter].depth >= depth) {
			for(unsigned int i = 0; i < moveArray[real_depth].count; ++i) {
				if(moveArray[real_depth].moveArray[i].equal(boardHash[hash & hash_cutter].move)) {
					if(abs(boardHash[hash & hash_cutter].evalute) < 100000) {
						if(abs(boardHash[hash & hash_cutter].evalute) >= (WHITE_WIN - 100)) {
							if(boardHash[hash & hash_cutter].evalute > 0) {
								boardHash[hash & hash_cutter].evalute -= (boardHash[hash & hash_cutter].ply - real_depth);
							} else {
								boardHash[hash & hash_cutter].evalute += (boardHash[hash & hash_cutter].ply - real_depth);
							}
						}

						local_move = boardHash[hash & hash_cutter].move;
						if(boardHash[hash & hash_cutter].type_mv == ALPHA_CUT_EV && boardHash[hash & hash_cutter].evalute <= alpha) {
							return boardHash[hash & hash_cutter].evalute;
						} else if(boardHash[hash & hash_cutter].type_mv == BETA_CUT_EV && boardHash[hash & hash_cutter].evalute >= beta) {
							return boardHash[hash & hash_cutter].evalute;
						} else if(boardHash[hash & hash_cutter].type_mv == REAL_EV) {
							return boardHash[hash & hash_cutter].evalute;
						}
					}
				}
			}
		}
	}

	for(unsigned int i = 0; i < moveArray[real_depth].count; ++i) {
		if(depth > 2) {
			if(rule == FIXED_TIME && timer.getTime() >= time) {
				return 0;
			}
		}

		b.move(moveArray[real_depth].moveArray[i]);

		if(game_board.inCheck(color)) {
			b.goBack();
			continue;
		}

		if(num_moves == 0) {
			local_move = moveArray[real_depth].moveArray[i];
		}

		++num_moves;

		tmp = -negamax(b, -beta, -alpha, nextDepth, real_depth + 1, rule, inNullMove);

		b.goBack();

		if(real_depth == 0 && num_moves > 1) {
				std::cout << "info depth " << max_depth << " currmove " << moveArray[real_depth].moveArray[i].getMoveString() << " currmovenumber " << num_moves;

			if(num_moves > 1) {
				std::cout << " ";
				printScore(alpha);
				std::cout << " nodes " << nodesCounter << " nps " << (int)(nodesCounter / ((clock() - start_timer) / CLOCKS_PER_SEC)) <<
				" time " << (int)((clock() - start_timer) / (CLOCKS_PER_SEC / 1000)) << "\n";
			} else {
				std::cout << "\n";
			}
		}

		if(tmp > max) {
			max = tmp;
			local_move = moveArray[real_depth].moveArray[i];
		}

		if(tmp > alpha) {
			alpha = tmp;
			local_move = moveArray[real_depth].moveArray[i];

			if(!local_move.isAttack) {
				if(color == WHITE) {
					whiteHistorySort[local_move.fromY][local_move.fromX][local_move.toY][local_move.toX] += pow(depth, 2);
					whiteKiller[real_depth] = Killer(local_move);
				} else {
					blackHistorySort[local_move.fromY][local_move.fromX][local_move.toY][local_move.toX] += pow(depth, 2);
					blackKiller[real_depth] = Killer(local_move);
				}
			}

			if(boardHash[hash & hash_cutter].enable) {
				if(boardHash[hash & hash_cutter].depth <= depth) {
					boardHash[hash & hash_cutter] = Hash(hash, local_move, depth, real_depth, tmp, alpha, beta, REAL_EV, b.getEvalute());
				}
			} else {
				boardHash[hash & hash_cutter] = Hash(hash, local_move, depth, real_depth, tmp, alpha, beta, REAL_EV, b.getEvalute());
			}
		}

		if(alpha >= beta) {
			if(abs(tmp) >= WHITE_WIN) {
				if(tmp > 0) {
					tmp = WHITE_WIN;
				} else {
					tmp = BLACK_WIN;
				}
			}

			if(boardHash[hash & hash_cutter].enable) {
				if(boardHash[hash & hash_cutter].depth <= depth) {
					boardHash[hash & hash_cutter] = Hash(hash, local_move, depth, real_depth, tmp, alpha, beta, BETA_CUT_EV, b.getEvalute());
				}
			} else {
				boardHash[hash & hash_cutter] = Hash(hash, local_move, depth, real_depth, tmp, alpha, beta, BETA_CUT_EV, b.getEvalute());
			}

			if(real_depth == 0) {
				if(depth > 2) {
					if(rule == FIXED_TIME && timer.getTime() >= time) {
						return 0;
					}
				}

				bestmove = local_move;
				bestMove = bestmove;
				bestScore = max;
			}

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
		if(depth > 2) {
			if(rule == FIXED_TIME && timer.getTime() >= time) {
				return 0;
			}
		}

		bestmove = local_move;
		bestMove = bestmove;
		bestScore = max;
	}

	if(abs(tmp) >= WHITE_WIN) {
		if(tmp > 0) {
			tmp = WHITE_WIN;
		} else {
			tmp = BLACK_WIN;
		}
	}

	if(boardHash[hash & hash_cutter].enable) {
		if(max < alpha) {
			boardHash[hash & hash_cutter] = Hash(hash, local_move, depth, real_depth, tmp, alpha, beta, ALPHA_CUT_EV, b.getEvalute());
		} else if(max > beta) {
			boardHash[hash & hash_cutter] = Hash(hash, local_move, depth, real_depth, tmp, alpha, beta, BETA_CUT_EV, b.getEvalute());
		}
	} else {
		if(max < alpha) {
			boardHash[hash & hash_cutter] = Hash(hash, local_move, depth, real_depth, tmp, alpha, beta, ALPHA_CUT_EV, b.getEvalute());
		} else if(max > beta) {
			boardHash[hash & hash_cutter] = Hash(hash, local_move, depth, real_depth, tmp, alpha, beta, BETA_CUT_EV, b.getEvalute());
		}
	}

	return alpha;
}

uint64_t Game::perft(int depth) {
	uint64_t res = 0;
	if(depth == 0) {
		return 1;
	}

	game_board.bitBoardMoveGenerator(moveArray[depth]);

	for(int i = 0; i < moveArray[depth].count; ++i) {
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
