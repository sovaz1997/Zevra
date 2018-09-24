#include "game.hpp"

int Game::negamax(BitBoard & b, int alpha, int beta, int depth, int real_depth, int rule, bool inNullMove, bool cut) {
	alpha = std::max(-WHITE_WIN, alpha);
	beta = std::min(WHITE_WIN, beta);
	
	++nodesCounter;

	uint64_t hash = b.getColorHash();
	Hash* currentHash = &boardHash[hash & hash_cutter];

	int oldAlpha = alpha;

	if(b.third_repeat[hash & hash_cutter] >= 3) {
		return 0;
	}


	if((currentHash->flag != EMPTY && currentHash->key == hash) && b.third_repeat[hash & hash_cutter] <= 1) {
		if(real_depth > 0 && currentHash->depth >= depth || depth == 0) {
			int score = currentHash->score;

			if(score > WHITE_WIN - 100) {
				score -= real_depth;
			} else if(score < -WHITE_WIN + 100) {
				score += real_depth;
			}

			if(currentHash->flag == BETA && score >= beta) {
				return score;
			} else if(currentHash->flag == ALPHA && score <= alpha) {
				return score;
			} else if(currentHash->flag == EXACT/* && score > alpha && score < beta*/) {
				return score;
			}

			/*if(currentHash->flag == BETA) {
				alpha = std::max(alpha, score);
				
			} else if(currentHash->flag == ALPHA) {
				beta = std::min(beta, score);
			} else {
				alpha = std::max(alpha, score);
				beta = std::min(beta, score);
			}

			if(alpha >= beta) {
				return alpha;
			}*/
		}
	}

	if(depth <= 0 || real_depth >= 100) {
		return quies(b, alpha, beta, rule, real_depth);
	}

	if(depth > 4) {
		if(is_input_available()) {
			std::string input_str;
			std::getline(std::cin, input_str);
			if(input_str == "stop") {
				stopped = true;
			} else if(input_str == "isready") {
				std::cout << "readyok" << std::endl;
			}
		}
	}

	if(real_depth) {
		max_real_depth = std::max(max_real_depth, real_depth);
	} else {
		max_real_depth = 0;
	}

	int eval = alpha;
	int nextDepth = depth - 1;
	int extensions = 0;

	if(depth > 2) {
		if((rule == FIXED_TIME && timer.getTime() >= time) || stopped) {
			return 0;
		}
	}

	uint8_t color = colorExtended[b.currentState.color];
	uint8_t enemyColor = colorExtended[!b.currentState.color];

	int tmp;
	bool inCheck = b.inCheck(color);
	bool onPV = ((beta - alpha) > 1 && real_depth > 0);

	//Null Move Pruning
	int R = 2;// + depth / 6;

	if(!inNullMove && !inCheck && depth > R && real_depth > 0 && !onPV) {
		b.makeNullMove();
		int eval = -negamax(b, -beta, -beta + 1, nextDepth - R, real_depth + 1, rule, true, true);
		b.unMakeNullMove();

		if(eval >= beta) {
			return eval;
		}
	}

	int num_moves = 0;

	b.bitBoardMoveGenerator(moveArray[real_depth]);
	sortAttacks(moveArray[real_depth]);
	sortMoves(moveArray[real_depth], real_depth);

	BitMove* local_move = nullptr;

	if(moveArray[real_depth].count > 0) {
		local_move = &moveArray[real_depth].moveArray[0];
	}

	int low_moves_count = 0;

	for(unsigned int i = 0; i < moveArray[real_depth].count; ++i) {
		b.move(moveArray[real_depth].moveArray[i]);

		if(game_board.inCheck(color)) {
			b.goBack();
			continue;
		}

		if(num_moves == 0) {
			local_move = &moveArray[real_depth].moveArray[i];
		}

		++num_moves;

		if(real_depth == 0 && depth >= 12) {
			if((rule != FIXED_TIME || timer.getTime() < time) && !stopped) {
				std::cout << "info currmove " << moveArray[real_depth].moveArray[i].getMoveString() << " currmovenumber " << num_moves << " time " << (int64_t)((clock() - start_timer) / (CLOCKS_PER_SEC / 1000)) << " nodes " << nodesCounter << " nps " << (int64_t)(nodesCounter / ((clock() - start_timer) / CLOCKS_PER_SEC)) << std::endl;
			}	
		}

		extensions = 0;
		if(b.inCheck(enemyColor)) {
			++extensions;
		}
		nextDepth = depth - 1;

		bool quiteMove = (!moveArray[real_depth].moveArray[i].isAttack && !extensions && !onPV);// && !whiteKiller[real_depth].move.equal(*local_move) && !blackKiller[real_depth].move.equal(*local_move) && !onPV);

		//Futility Pruning
		if(!inNullMove && nextDepth <= 2 && quiteMove && !onPV) {
			if(-b.getEvaluate() + PAWN_EV.mg / 2 <= alpha && !b.inCheck(color)) {
				++nodesCounter;
				b.goBack();
				continue;
			}
		}

		if(quiteMove) {
			++low_moves_count;
		}

		int reductions = 0;

		if(low_moves_count > 3) {
			reductions = 1 + low_moves_count / 6;
		}

		nextDepth -= reductions;


		if(num_moves == 1) {
			tmp = -negamax(b, -beta, -alpha, nextDepth + extensions, real_depth + 1, rule, inNullMove, true);	
		} else {
			tmp = -negamax(b, -(alpha + 1), -alpha, nextDepth + extensions, real_depth + 1, rule, inNullMove, true);

			if(reductions > 0 && tmp > alpha) {
				nextDepth += reductions;
				tmp = -negamax(b, -(alpha + 1), -alpha, nextDepth + extensions, real_depth + 1, rule, inNullMove, true);
			}

			if(tmp > alpha && tmp < beta) {
				tmp = -negamax(b, -beta, -alpha, nextDepth + extensions, real_depth + 1, rule, inNullMove, true);
			}
		}

		b.goBack();

		if(tmp > eval) {
			eval = tmp;
		}

		if(tmp > alpha) {
			alpha = tmp;
			local_move = &moveArray[real_depth].moveArray[i];
			recordHash(depth, tmp, tmp<beta?EXACT:BETA, hash, moveArray[real_depth].moveArray[i], real_depth);

			if(!local_move->isAttack) {
				historySort[b.currentState.color][local_move->fromY][local_move->fromX][local_move->toY][local_move->toX] += std::pow(depth, 2);
			}
		}
		
		if(alpha >= beta) {
			//if(!local_move->isAttack) {
				//historySort[b.currentState.color][local_move->fromY][local_move->fromX][local_move->toY][local_move->toX] += std::pow(depth, 2);
			
				if(color == WHITE) {
					whiteKiller[real_depth] = Killer(*local_move);
				} else {
					blackKiller[real_depth] = Killer(*local_move);
				}
			//}
			break;
		}
	}

	if(num_moves == 0) {
		if(game_board.inCheck(color)) {
			return BLACK_WIN + real_depth;
		} else {
			return 0;
		}
	}

	if(alpha == oldAlpha) {
		recordHash(depth, eval, ALPHA, hash, *local_move, real_depth);
	}

	if(real_depth == 0) {
		if((rule == FIXED_TIME && timer.getTime() >= time) || stopped) {
			return 0;
		}

		bestMove = *local_move;
		bestScore = alpha;
	}

	if(real_depth == 0) {
		if(num_moves >= 0) {
			int score_type = NORMAL;
			if(eval >= beta) {
				score_type = LOWERBOUND;
			} else if(eval <= alpha) {
				score_type = UPPERBOUND;
			}

			std::cout << "info depth " << max_depth << " time " << (int64_t)((clock() - start_timer) / (CLOCKS_PER_SEC / 1000)) << " nodes " << nodesCounter << " nps " << (int64_t)(nodesCounter / ((clock() - start_timer) / CLOCKS_PER_SEC)) << " hashfull " << (int)(hash_filled / max_hash_filled * 1000) << " seldepth " << max_real_depth;
			std::cout << " ";
			printScore(eval, NORMAL);
			std::cout << " pv ";
			printPV(depth);
		} else {
			std::cout << std::endl;
		}
	}

	return eval;
}

uint64_t Game::perft(int depth) {
	uint64_t res = 0;
	if(depth == 0) {
		++combinations;

		return 1;
	}

	game_board.bitBoardMoveGenerator(moveArray[depth]);

	for(unsigned int i = 0; i < moveArray[depth].count; ++i) {
		game_board.move(moveArray[depth].moveArray[i]);
		if(game_board.currentState.color) {
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

int Game::quies(BitBoard & b, int alpha, int beta, int rule, int real_depth) {
	int val = b.getEvaluate();

	if (val < alpha - QUEEN_EV.mg) {
   		return alpha;
	}

	if(val >= beta) {
		return val;
	}

	if(alpha < val) {
		alpha = val;
	}

	b.bitBoardAttackMoveGenerator(moveArray[real_depth]);
	sortAttacks(moveArray[real_depth]);

	for(unsigned int i = 0; i < moveArray[real_depth].count; ++i) {
		++nodesCounter;
		b.move(moveArray[real_depth].moveArray[i]);

		if(game_board.inCheck(colorExtended[!game_board.currentState.color])) {
			b.goBack();
			continue;
		}

		val = -quies(b, -beta, -alpha, rule, real_depth + 1);
		b.goBack();

		if(val >= beta) {
			return beta;
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
		return false;
	}

	if((hash->flag != EMPTY) && hash->depth > depth && hash->age == hashAge) {
		return false;
	}

	if(score > WHITE_WIN - 100) {
		score += real_depth;
	} else if(score < -WHITE_WIN + 100) {
		score -= real_depth;
	}

	if(hash->flag == EMPTY) {
		++hash_filled;
	}

	hash->depth = depth;
	hash->score = score;
	hash->flag = flag;
	hash->key = key;
	hash->age = hashAge;

	if(flag != ALPHA) {
		hash->setMove(move);
	}

	return true;
}

std::vector<BitMove> Game::extractPV(int depth) {
	int k;
	std::vector<BitMove> result;
	MoveArray moves;
	bool stopped;
	int stack_count = 0;
	for(k = 0; k < depth + 10; ++k) {
		stopped = true;
		uint64_t hash = game_board.getColorHash();
		Hash* currentHash = &boardHash[hash & hash_cutter];

		if(currentHash->flag == EXACT || currentHash->flag == BETA) {
			game_board.bitBoardMoveGenerator(moves);

			bool enable;
			BitMove mv = game_board.getMove(currentHash->fromY, currentHash->fromX, currentHash->toY, currentHash->toX, currentHash->replaced, currentHash->replacedFigure, enable);// currentHash->getMove();

			for(int i = 0; i < moves.count; ++i) {
				if(moves.moveArray[i].equal(mv)) {
					game_board.move(mv);
					++stack_count;
					if(game_board.currentState.color) {
						if(game_board.inCheck(BLACK)) {
							stopped = true;
						} else {
							stopped = false;
						}
					} else {
						if(game_board.inCheck(WHITE)) {
							stopped = true;
						} else {
							stopped = false;
						}
					}
				}
			}

			if(stopped || !enable) {
				break;
			}

			result.emplace_back(mv);
		} else {
			break;
		}
	}

	for(int i = 0; i < stack_count; ++i) {
		game_board.goBack();
	}

	return result;
}


bool Game::testMovePossible(BitMove move) {
	MoveArray moves;
	game_board.bitBoardMoveGenerator(moves);

	for(unsigned int i = 0; i < moves.count; ++i) {
		if(move.equal(moves.moveArray[i])) {
			return true;
		}
	}

	return false;
}

void Game::printPV(int depth) {
	std::vector<BitMove> main_pv = extractPV(depth);
	for(unsigned int i = 0; i < main_pv.size(); ++i) {
		std::cout << main_pv[i].getMoveString() << " ";
	}
	std::cout << std::endl;
}