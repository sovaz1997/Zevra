#include "game.hpp"

/*PV Game::negamax(Board & b, double alpha, double beta, int depth, int real_depth, std::vector<uint64_t> hash, bool basis, std::vector<Move>pv, bool usedNullMove, int rule, bool capture) {
	int nextDepth = depth - 1;
	PV bpv;
	if(rule == FIXED_TIME && timer.getTime() >= time) {
		return 0;
	}

	++movesCounter;

	uint8_t color;
	if(b.isWhiteMove()) {
		color = WHITE;
	} else {
		color = BLACK;
	}

	//ПРОВЕРКА НА ПОВТОРЕНИЕ ПОЗИЦИИ/ПРАВИЛА 50 ХОДОВ

	uint64_t pos_hash = getHash(b);
	uint64_t board_hash = getColorHash(b, pos_hash);
	//hash.push_back(pos_hash);
	//if(depth > 0) {
	//	int third_repeat = 1;
	//		for(unsigned int i = 0; i < hash.size(); ++i) {
	//			if(hash[i] == pos_hash) {
	//				++third_repeat;
	//			}
	//			if(third_repeat >= 3  || b.move_rule_num >= 50) {
	//				return 0;
	//			}
	//	}
	//}

	if(depth <= 0) {
		//return evalute(b);

		//if(eval > alpha && eval < beta && eval >= whiteUp) {
		//	pv_best = pv_tmp;
		//	whiteUp = eval;
		//}
		return PV(quies(b, alpha, beta, rule));
	}

	//if(depth >= 4 && usedNullMove) {
	//	if(!inCheck(b, color)) {
	//		b.whiteMove = !b.whiteMove;
	//		double tmp = -negamax(b, beta - 1, beta, nextDepth - 3, real_depth + 1, hash, basis, pv, false, rule, capture);
	//		b.whiteMove = !b.whiteMove;

	//		if(tmp >= beta) {
	//			return beta;
	//		}
	//	}
	//}

	//if(inCheck(b, color) && real_depth < 100) {
	//	--nextDepth;
	//}

	int num_moves = 0;

	double max = BLACK_WIN;

	bool tmp_shah;
	std::vector<Move>moves = b.bitBoardMoveGenerator();//generatePositionMoves(b, tmp_shah, true, real_depth);
	sortMoves(moves, d);

	Move local_move;

	if(moves.size() > 0) {
		local_move = moves[0];
	}

	PV tmp;

	for(unsigned int i = 0; i < moves.size(); ++i) {
		if(rule == FIXED_TIME) {
			if(timer.getTime() >= time) {
				return 0;
			}
		}

		bool capt = false;
		if(b.board[moves[i].toY][moves[i].toX] != 0) {
			capt = true;
		}

		b.move(moves[i]);

		if(inCheck(b, color)) {
			b.goBack();
			continue;
		}

		if(num_moves == 0) {
			local_move = moves[i];
		}

		++num_moves;

		pv.push_back(moves[i]);

		if(moves[i].fromHash && boardHash[board_hash & hash_cutter].type_mv == ALPHA_CUT_EV && boardHash[board_hash & hash_cutter].enable && boardHash[board_hash & hash_cutter].hash == board_hash && boardHash[board_hash & hash_cutter].depth >= depth) {
			alpha = boardHash[board_hash & hash_cutter].alpha;
//		beta = boardHash[board_hash & hash_cutter].beta;
	}

		//if(depth <= MIN_DEPTH) {
		//	tmp = alpha + 1;
		//} else {
		//	tmp = alpha + 1;
		//	int D = std::max(MIN_DEPTH, depth / 2 + 1);

		//	while(D < depth - 1 && tmp > alpha) {
		//		tmp = -negamax(b, -beta, -alpha, nextDepth - 1, real_depth + 1, hash, basis, pv, true, rule, capt);
		//		++D;
		//	}
		//}

		//if(depth >= max_depth - MIN_DEPTH) {
		//	tmp = alpha + 1;
		//} else {
		//	tmp = -negamax(b, -beta, -alpha, nextDepth + 1, real_depth + 1, hash, basis, pv, true, rule, capt);
		//}

		//if(tmp > alpha) {
		if(pv_tmp.size() < (unsigned int) (real_depth + 1)) {
			pv_tmp.resize(real_depth + 1);
			pv_best.resize(real_depth + 1);
		}

		pv_tmp[real_depth] = local_move;
		//if(num_moves <= 1) {
			tmp = negamax(b, -(alpha+1), -alpha, nextDepth, real_depth + 1, hash, basis, pv, true, rule, capt);
			tmp.evalute = -tmp.evalute;
			if(tmp.evalute > alpha && tmp.evalute < beta) {
					tmp = negamax(b, -beta, -alpha, nextDepth, real_depth + 1, hash, basis, pv, true, rule, capt);
					tmp.evalute = -tmp.evalute;
			}


		//} else {
			//tmp = negamax(b, -alpha - 1, -alpha, nextDepth, real_depth + 1, hash, basis, pv, true, rule, capt);
			//tmp.evalute = -tmp.evalute;
		//}

		//if(tmp.evalute > alpha) {
		//	tmp = negamax(b, -beta, -alpha, nextDepth, real_depth + 1, hash, basis, pv, true, rule, capt);
		//	tmp.evalute = -tmp.evalute;
		//}
		//}

		b.goBack();
		pv.pop_back();

		if(real_depth == 0 && num_moves > 1) {
				std::cout << "info depth " << max_depth << " currmove " << moves[i].getMoveString() << " currmovenumber " << num_moves;

			if(num_moves > 1) {
				std::cout << " ";
				printScore(alpha);
				std::cout << " pv " << local_move.getMoveString() << " ";
				for(unsigned int i = 0; i < bpv.size(); ++i) {
					//std::cout << pv_best[i].getMoveString() << " ";
					std::cout << bpv.pv[i].getMoveString() << " ";
				}
				std::cout << "nodes " << movesCounter << " nps " << (int)(movesCounter / ((clock() - start_timer) / CLOCKS_PER_SEC)) <<
				" time " << (int)((clock() - start_timer) / (CLOCKS_PER_SEC / 1000)) << "\n";
			} else {
				std::cout << "\n";
			}
		}

		if(tmp.evalute > max) {
			max = tmp.evalute;
			bpv = tmp;
			local_move = moves[i];
		}

		if(tmp.evalute > alpha) {
			alpha = tmp.evalute;
			//if(real_depth == 0) {
				//pv_best[real_depth] = local_move;


			//}
		}

		if(max >= beta) {
			bpv.push(local_move);
			//pv_best = pv_tmp;

			if(b.board[moves[i].toY][moves[i].toX] == 0) {
				if(color == WHITE) {
					whiteHistorySort[moves[i].fromY][moves[i].fromX][moves[i].toY][moves[i].toX] += pow(depth, 2);
					whiteKiller[real_depth] = Killer(moves[i]);
				} else {
					blackHistorySort[moves[i].fromY][moves[i].fromX][moves[i].toY][moves[i].toX] += pow(depth, 2);
					blackKiller[real_depth] = Killer(moves[i]);
				}
			}

			if(boardHash[board_hash & hash_cutter].enable) {
				if(boardHash[board_hash & hash_cutter].depth <= depth && board_hash == boardHash[board_hash & hash_cutter].hash) {
					boardHash[board_hash & hash_cutter] = Hash(board_hash, local_move, depth, tmp.evalute, alpha, beta, ALPHA_CUT_EV);
				}
			} else {
				boardHash[board_hash & hash_cutter] = Hash(board_hash, local_move, depth, tmp.evalute, alpha, beta, ALPHA_CUT_EV);
			}

			if(real_depth == 0 && basis) {
				bestmove = local_move;
				bestMove = bestmove;
				bestScore = max;
			}

			bpv.evalute = max;
			return bpv;
		}
	}

	if(num_moves == 0) {
		if(inCheck(b, color)) {
			bpv.evalute = BLACK_WIN + real_depth;
			return bpv;
		} else {
			bpv.evalute = 0;
			return bpv;
		}
	}

	bpv.push(local_move);

	if(real_depth == 0 && basis) {
		bestmove = local_move;
		bestMove = bestmove;
		bestScore = max;
	}

	bpv.evalute = max;
	return bpv;
}*/

double Game::negamax(BitBoard & b, double alpha, double beta, int depth, int real_depth, int rule) {
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

	if(depth <= 0) {
		return quies(b, alpha, beta, rule, real_depth);
	}

	int num_moves = 0;

	double max = BLACK_WIN;

	bool tmp_shah;
	b.bitBoardMoveGenerator(moveArray[real_depth]);
	sortAttacks(moveArray[real_depth]);
	sortMoves(moveArray[real_depth], real_depth);

	BitMove local_move;

	if(moveArray[real_depth].count > 0) {
		local_move = moveArray[real_depth].moveArray[0];
	}

	int tmp;

	//uint64_t hash = b.getColorHash();

	/*if(boardHash[hash & hash_cutter].enable && boardHash[hash & hash_cutter].hash == hash) {
		if(boardHash[hash & hash_cutter].depth >= depth) {
			if(boardHash[hash & hash_cutter].type_mv == REAL_EV) {
				for(unsigned int i = 0; i < moveArray[real_depth].count; ++i) {
					if(moveArray[real_depth].moveArray[i].equal(boardHash[hash & hash_cutter].move)) {
						if(abs(boardHash[hash & hash_cutter].evalute) < 100000) {
							if(real_depth == 0) {
								bestMove = boardHash[hash & hash_cutter].move;
							}
							return boardHash[hash & hash_cutter].evalute;
						}
					}
				}
			}
		}
	}*/

	for(unsigned int i = 0; i < moveArray[real_depth].count; ++i) {
		++movesCounter;
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

		//if(moves[i].fromHash && boardHash[board_hash & hash_cutter].type_mv == ALPHA_CUT_EV && boardHash[board_hash & hash_cutter].enable && boardHash[board_hash & hash_cutter].hash == board_hash && boardHash[board_hash & hash_cutter].depth >= depth) {
		//	alpha = boardHash[board_hash & hash_cutter].alpha;
		//	beta = boardHash[board_hash & hash_cutter].beta;
		//}

		tmp = -negamax(b, -beta, -alpha, nextDepth, real_depth + 1, rule);

		/*if(num_moves <= 1) {
			tmp = -negamax(b, -(alpha+1), -alpha, nextDepth, real_depth + 1, rule);
			if(tmp > alpha && tmp < beta) {
					tmp = -negamax(b, -beta, -alpha, nextDepth, real_depth + 1, rule);
			}

		} else {
			tmp = -negamax(b, -alpha - 1, -alpha, nextDepth, real_depth + 1, rule);

		}

		if(tmp > alpha) {
			tmp = -negamax(b, -beta, -alpha, nextDepth, real_depth + 1, rule);
		}*/

		b.goBack();

		if(real_depth == 0 && num_moves > 1) {
				std::cout << "info depth " << max_depth << " currmove " << moveArray[real_depth].moveArray[i].getMoveString() << " currmovenumber " << num_moves;

			if(num_moves > 1) {
				std::cout << " ";
				printScore(alpha);
				std::cout << " nodes " << movesCounter << " nps " << (int)(movesCounter / ((clock() - start_timer) / CLOCKS_PER_SEC)) <<
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
			//if(real_depth == 0) {
				//pv_best[real_depth] = local_move;


			//}

			if(!local_move.isAttack) {
				if(color == WHITE) {
					//whiteHistorySort[local_move.fromY][local_move.fromX][local_move.toY][local_move.toX] += pow(depth, 2);
					whiteKiller[real_depth] = Killer(local_move);
				} else {
					//blackHistorySort[local_move.fromY][local_move.fromX][local_move.toY][local_move.toX] += pow(depth, 2);
					blackKiller[real_depth] = Killer(local_move);
				}
			}
		}

		if(max >= beta) {
			//pv_best = pv_tmp;

			/*if(boardHash[hash & hash_cutter].enable) {
				if(boardHash[hash & hash_cutter].depth <= depth) {
					boardHash[hash & hash_cutter] = Hash(hash, local_move, depth, tmp, alpha, beta, REAL_EV);
				}
			} else {
				boardHash[hash & hash_cutter] = Hash(hash, local_move, depth, tmp, alpha, beta, REAL_EV);
			}*/

			if(real_depth == 0) {
				bestmove = local_move;
				bestMove = bestmove;
				bestScore = max;
			}

			return max;
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
		bestmove = local_move;
		bestMove = bestmove;
		bestScore = max;
	}

	/*if(boardHash[hash & hash_cutter].enable) {
		if(boardHash[hash & hash_cutter].depth <= depth) {
			boardHash[hash & hash_cutter] = Hash(hash, local_move, depth, tmp, alpha, beta, REAL_EV);
		}
	} else {
		boardHash[hash & hash_cutter] = Hash(hash, local_move, depth, tmp, alpha, beta, REAL_EV);
	}*/

	return max;
}

uint64_t Game::perft(int depth) {
	uint64_t res = 0;
	if(depth == 0) {
		return 1;
	}
	/*std::vector<Move>moves = */game_board.bitBoardMoveGenerator(moveArray[depth]);

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
	/*if(rule == FIXED_TIME && timer.getTime() >= time) {
		return 0;
	}*/

	//b.evaluteAll();
	double val = b.getEvalute();

	if(val >= beta) {
		return beta;
	}

	if(alpha < val) {
		alpha = val;
	}

	bool tmp_shah = false;
	b.bitBoardAttackMoveGenerator(moveArray[real_depth]);
	sortAttacks(moveArray[real_depth]);
	//sortMoves(moves, 0);

	for(unsigned int i = 0; i < moveArray[real_depth].count && alpha < beta; ++i) {
		/*if(rule == FIXED_TIME && timer.getTime() >= time) {
			return 0;
		}*/

		++movesCounter;
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

/*bool Game::isEndGame(Board& b) {
	int num_figures = 0;
	for(unsigned int y = 0; y < BOARD_SIZE; ++y) {
		for(unsigned int x = 0; x < BOARD_SIZE; ++x) {
			if(b.board[y][x] != 0) {
				++num_figures;
			}
		}
	}

	return num_figures <= 7;
}*/
