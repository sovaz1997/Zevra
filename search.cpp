#include "game.hpp"

double Game::negamax(Board & b, double alpha, double beta, int depth, int real_depth, std::vector<uint64_t> hash, bool basis, std::vector<Move>pv, bool usedNullMove, int rule, bool capture) {
	int nextDepth = depth + 1;
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

	/*uint64_t pos_hash = getHash(b);
	uint64_t board_hash = getColorHash(b, pos_hash);
	hash.push_back(pos_hash);
	if(depth > 0) {
		int third_repeat = 1;
			for(unsigned int i = 0; i < hash.size(); ++i) {
				if(hash[i] == pos_hash) {
					++third_repeat;
				}
				if(third_repeat >= 3  || b.move_rule_num >= 50) {
					return 0;
				}
		}
	}*/

	if(depth >= max_depth) {
		return quies(b, alpha, beta, rule);
	}

	if(inCheck(b, color) && real_depth < 100) {
		nextDepth;
	}

	int num_moves = 0;

	double max = BLACK_WIN;

	bool tmp_shah;
	std::vector<Move>moves = generatePositionMoves(b, tmp_shah, true, real_depth);

	Move local_move;

	if(moves.size() > 0) {
		local_move = moves[0];
	}

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

		double tmp;

		pv.push_back(moves[i]);
			tmp = -negamax(b, -beta, -alpha, nextDepth, real_depth + 1, hash, basis, pv, true, rule, capt);
			b.goBack();
			pv.pop_back();

		if(real_depth == 0) {
				std::cout << "info depth " << max_depth << " currmove " << moves[i].getMoveString() << " currmovenumber " << num_moves;

			if(num_moves > 1) {
				std::cout << " ";
				printScore(alpha);
				std::cout << " pv ";
				for(unsigned int i = 0; i < pv_best.size(); ++i) {
					std::cout << pv_best[i].getMoveString() << " ";
				}
				std::cout << "nodes " << movesCounter << " nps " << (int)(movesCounter / ((clock() - start_timer) / CLOCKS_PER_SEC)) <<
				" time " << (int)((clock() - start_timer) / (CLOCKS_PER_SEC / 1000)) << "\n";
			} else {
				std::cout << "\n";
			}
		}

		if(tmp > max) {
			max = tmp;
			local_move = moves[i];

			if(pv_tmp.size() < (unsigned int) (real_depth + 1)) {
				pv_tmp.resize(real_depth + 1);
			}

		}

		if(tmp > alpha) {
			alpha = tmp;

			pv_tmp[real_depth] = local_move;

			if(real_depth == 0) {
				pv_best = pv_tmp;
			}
		}

		if(alpha >= beta) {
			if(b.board[moves[i].toY][moves[i].toX] == 0) {
				if(color == WHITE) {
					whiteHistorySort[moves[i].fromY][moves[i].fromX][moves[i].toY][moves[i].toX] += pow(max_depth - real_depth, max_depth - real_depth);
					whiteKiller[real_depth] = Killer(moves[i]);
				} else {
					blackHistorySort[moves[i].fromY][moves[i].fromX][moves[i].toY][moves[i].toX] += pow(max_depth - real_depth, max_depth - real_depth);
					blackKiller[real_depth] = Killer(moves[i]);
				}
			}

			if(real_depth == 0 && basis) {
				bestmove = local_move;
				bestMove = bestmove;
				bestScore = max;
			}

			return alpha;
		}
	}

	if(num_moves == 0) {
		if(inCheck(b, color)) {
			return BLACK_WIN + real_depth;
		} else {
			return 0;
		}
	}

	if(real_depth == 0 && basis) {
		bestmove = local_move;
		bestMove = bestmove;
		bestScore = max;
	}

	return max;
}

double Game::quies(Board & b, double alpha, double beta, int rule) {
	if(rule == FIXED_TIME && timer.getTime() >= time) {
		return 0;
	}

	double val = evalute(b);

	if(val >= beta) {
		return beta;
	}

	if(alpha < val) {
		alpha = val;
	}

	bool tmp_shah = false;
	std::vector<Move>moves = generatePositionMoves(b, tmp_shah, true, max_depth);

	for(unsigned int i = 0; i < moves.size() && alpha < beta; ++i) {
		if(rule == FIXED_TIME && timer.getTime() >= time) {
			return 0;
		}

		if(b.board[moves[i].toY][moves[i].toX] == 0) {
			continue;
		}

		++movesCounter;
		b.move(moves[i]);

		if(!b.isWhiteMove()) {
			if(inCheck(b, WHITE)) {
				b.goBack();
				continue;
			}
		} else {
			if(inCheck(b, BLACK)) {
				b.goBack();
				continue;
			}
		}

		val = -quies(b, -beta, -alpha, rule);
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

bool Game::isEndGame(Board& b) {
	int num_figures = 0;
	for(unsigned int y = 0; y < BOARD_SIZE; ++y) {
		for(unsigned int x = 0; x < BOARD_SIZE; ++x) {
			if(b.board[y][x] != 0) {
				++num_figures;
			}
		}
	}

	return num_figures <= 7;
}
