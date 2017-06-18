#include "game.hpp"

int64_t Game::negamax(BitBoard & b, int64_t alpha, int64_t beta, int depth, int real_depth, int rule, bool inNullMove, bool pv) {
	++nodesCounter;

	if(depth >= 5) {
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

	int64_t oldAlpha = alpha;

	bool extended = false;

	int64_t eval = -WHITE_WIN;

	int nextDepth = depth - 1;
	int extensions = 0;
	if(depth > 2) {
		if((rule == FIXED_TIME && timer.getTime() >= time) || stopped) {
			return 0;
		}
	}

	uint8_t color, enemyColor;
	if(b.whiteMove) {
		color = WHITE;
		enemyColor = BLACK;
	} else {
		color = BLACK;
		enemyColor = WHITE;
	}

	if(depth <= 0 || real_depth >= 100) {
		return quies(b, alpha, beta, rule, real_depth);
	}

	int tmp;

	uint64_t hash = b.getColorHash();
	Hash* currentHash = &boardHash[hash & hash_cutter];
	if((currentHash->flag != EMPTY && currentHash->key == hash && !extended )) {
	//if((currentHash->flag != EMPTY && currentHash->key == hash && !extended && !option.nullMovePrunningEnable) || (currentHash->flag != EMPTY && currentHash->key == hash && inNullMove && !extended && option.nullMovePrunningEnable)) {
		if(real_depth > 0 && currentHash->depth >= depth) {
			double score = currentHash->score;

			if(score > WHITE_WIN - 100) {
				score -= real_depth;
			} else if(score < -WHITE_WIN + 100) {
				score += real_depth;
			}

			if(currentHash->flag == BETA) {
				if(score >= beta) {
					return beta;
				}
			} else if(currentHash->flag == ALPHA) {
				if(score <= alpha) {
					return alpha;
				}
			} else if(currentHash->flag == EXACT) {
				if(score <= alpha) {
					return alpha;
				}
				if(score >= beta) {
					return beta;
				}
			}
		}

		if(currentHash->flag != ALPHA && real_depth > 0) {
			b.move(currentHash->move);
			tmp = -negamax(b, -beta, -alpha, depth - 1, real_depth + 1, rule, inNullMove, false);
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

	bool onPV = (beta - alpha) > 1;

	if(option.nullMovePruningEnable) { //Null Move Pruning
		if(!inNullMove && !extended && !inCheck && !onPV && depth > 2 && (b.popcount64(b.white_bit_mask | b.black_bit_mask) > 6) && real_depth > 0) {
			b.makeNullMove();
			int R = 2 + depth / 6;

			double value = -negamax(b, -beta, -beta + 1, depth - R - 1, real_depth + 1, rule, true, false);
			if(value >= beta) {
				b.unMakeNullMove();
				return beta;
			}

			b.unMakeNullMove();
		}
	}

	if(option.futility_pruning && !extended && !inCheck && !b.attacked && depth <= 2 && !inNullMove /*&& !onPV*/) { //Futility pruning
		if(b.getEvalute() - PAWN_EV / 2 >= beta) {
			return beta;
		}
	}

	if(option.razoring && !extended && !inCheck && !b.attacked && !inNullMove && depth <= 4/* && !onPV*/) { //Razoring
		if(b.getEvalute() - QUEEN_EV >= beta) {
			return beta;
		}
	}

	/*bool iid_test_complete = false;
	if(depth > 2 && !inNullMove && !onPV && !currentHash->flag == EMPTY) { //IID
		iid_test_complete = true;
		negamax(b, -beta, -alpha, 2, real_depth + 1, rule, inNullMove, true);
	}*/

	int num_moves = 0;

	b.bitBoardMoveGenerator(moveArray[real_depth]);

	/*if(iid_test_complete) {
		for(int i = 0; i < moveArray[real_depth].count; ++i) {
			if(moveArray[real_depth].moveArray[i].equal(iid_move)) {
				moveArray[real_depth].moveArray[i].fromHash = true;
				break;
			}
		}
	}*/

	sortAttacks(moveArray[real_depth]);
	sortMoves(moveArray[real_depth], real_depth);

	BitMove local_move;

	if(moveArray[real_depth].count > 0) {
		local_move = moveArray[real_depth].moveArray[0];
	}

	int low_moves_count = 0;

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

		if(real_depth == 0 && depth >= 9) {
			if((rule != FIXED_TIME || timer.getTime() < time) && !stopped) {
				std::cout << "info currmove " << moveArray[real_depth].moveArray[i].getMoveString() << " currmovenumber " << num_moves << " time " << (int64_t)((clock() - start_timer) / (CLOCKS_PER_SEC / 1000)) << " nodes " << nodesCounter << " nps " << (int64_t)(nodesCounter / ((clock() - start_timer) / CLOCKS_PER_SEC)) << std::endl;
			}	
		}

		extensions = 0;
		if((b.inCheck(enemyColor) && option.checkExtensions)) {
			++extensions;
		}

		nextDepth = depth - 1;
		nextDepth += extensions;

		Killer* killer;
		if(color == WHITE) {
			killer = &whiteKiller[real_depth];
		} else {
			killer = &blackKiller[real_depth];
		}

		
		Killer* secondKiller;
		if(color == WHITE) {
			secondKiller = &whiteSecondKiller[real_depth];
		} else {
			secondKiller = &blackSecondKiller[real_depth];
		}
		
		double reduction = 0;

		if(!b.inCheck(enemyColor) && !extensions && !inNullMove && !moveArray[real_depth].moveArray[i].isAttack && !onPV && !inCheck && !moveArray[real_depth].moveArray[i].replaced /*&& (!moveArray[real_depth].moveArray[i].equal(killer->move) || !killer->enable) && (!moveArray[real_depth].moveArray[i].equal(secondKiller->move) || !secondKiller->enable)*/) {
			++low_moves_count;

			if(low_moves_count > 3) {
				reduction = 1 + low_moves_count / 6;
				nextDepth -= reduction;
			}
		}

		if(num_moves == 1) {
			tmp = -negamax(b, -beta, -alpha, nextDepth, real_depth + 1, rule, inNullMove, false);	
		} else {
			tmp = -negamax(b, -(alpha + 1), -alpha, nextDepth, real_depth + 1, rule, inNullMove, false);
			
			if(reduction > 0 && tmp > alpha) {
				nextDepth += reduction;
				tmp = -negamax(b, -(alpha + 1), -alpha, nextDepth, real_depth + 1, rule, inNullMove, false);
			}

			if(tmp > alpha && tmp < beta) {
				tmp = -negamax(b, -beta, -alpha, nextDepth, real_depth + 1, rule, inNullMove, false);
			}
		}

		b.goBack();

		if(tmp > eval) {
			eval = tmp;
		}

		if(tmp > alpha) {
			alpha = tmp;
			local_move = moveArray[real_depth].moveArray[i];
			recordHash(depth, tmp, tmp<beta?EXACT:BETA, hash, moveArray[real_depth].moveArray[i], real_depth);
		}
		
		if(alpha >= beta) {
			if(!local_move.isAttack) {
				whiteHistorySort[local_move.fromY][local_move.fromX][local_move.toY][local_move.toX] += std::pow(depth, 2);

				if(color == WHITE) {
					if(std::abs(alpha) >= WHITE_WIN - 100) {
						whiteMateKiller[real_depth] = Killer(local_move);
					}

					if(whiteKiller[real_depth].enable) {
						whiteSecondKiller[real_depth] = Killer(whiteKiller[real_depth].move);
					}
					whiteKiller[real_depth] = Killer(local_move);
				} else {
					if(std::abs(alpha) >= WHITE_WIN - 100) {
						blackMateKiller[real_depth] = Killer(local_move);
					}

					if(blackKiller[real_depth].enable) {
						blackSecondKiller[real_depth] = Killer(blackKiller[real_depth].move);
					}
					blackKiller[real_depth] = Killer(local_move);
				}
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
		if((rule == FIXED_TIME && timer.getTime() >= time) || stopped) {
			return 0;
		}

		bestMove = local_move;
		bestScore = alpha;
	}

	iid_move = local_move;

	if(alpha == oldAlpha) {
		recordHash(depth, alpha, ALPHA, hash, local_move, real_depth);
	}

	if(real_depth == 0) {
		if(num_moves >= 0) {
			std::cout << "info depth " << max_depth << " time " << (int64_t)((clock() - start_timer) / (CLOCKS_PER_SEC / 1000)) << " nodes " << nodesCounter << " nps " << (int64_t)(nodesCounter / ((clock() - start_timer) / CLOCKS_PER_SEC)) << " hashfull " << (int)(hash_filled / max_hash_filled * 1000) << " seldepth " << max_real_depth;
			std::cout << " ";
			printScore(eval);
			std::cout << " pv ";
			printPV(depth);
		} else {
			std::cout << std::endl;
		}
	}

	return alpha;
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

int64_t Game::quies(BitBoard & b, int64_t alpha, int64_t beta, int rule, int real_depth) {
	int64_t val = b.getEvalute();

	if (val < alpha - QUEEN_EV) {
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

		if(val > alpha) {
			alpha = val;
		}

		if(val >= beta) {
			return beta;
		}
	}

	return alpha;
}

bool Game::recordHash(int depth, int score, int flag, uint64_t key, BitMove move, int real_depth) {
	if(!game_board.hash_enable) {
		return false;
	}

	Hash* hash = &boardHash[key & hash_cutter];

	if(flag == ALPHA && (hash->flag == EXACT || hash->flag == BETA)) {
		return false;
	}

	if((hash->flag != EMPTY) && hash->depth > depth + hash_decrement /*&& hash->age == hashAge*/) {
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

	hash->depth = depth + hash_decrement;
	hash->score = score;
	hash->flag = flag;
	hash->key = key;
	hash->age = hashAge;

	if(flag != ALPHA) {
		/*if(flag == EXACT) {
			hash->recordPrev();
		}*/

		hash->move = move;
	}

	return true;
}

std::vector<BitMove> Game::extractPV(int depth) {
	int k;
	std::vector<BitMove> result;
	bool stopped = false;
	for(k = 0; k < depth + 10; ++k) {
		uint64_t hash = game_board.getColorHash();
		Hash* currentHash = &boardHash[hash & hash_cutter];

		if(currentHash->flag == EXACT || currentHash->flag == BETA) {
			if(currentHash->flag == BETA) {
				if(!currentHash->back()) {
					break;
				}
			}

			while(!testMovePossible(currentHash->move)) {
				if(!currentHash->back()) {
          stopped = true;
					break;
				}
			}

			if(stopped) {
				break;
			}

			result.emplace_back(currentHash->move);
			game_board.move(currentHash->move);
		} else {
			break;
		}
	}

	for(int i = 0; i < k; ++i) {
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

int64_t Game::negamax_elementary(BitBoard & b, int64_t alpha, int64_t beta, int depth, int real_depth, int rule, bool inNullMove) {
	++nodesCounter;

	if(depth >= 5) {
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

	int64_t eval = -WHITE_WIN;

	int nextDepth = depth - 1;
	if(depth > 2) {
		if((rule == FIXED_TIME && timer.getTime() >= time) || stopped) {
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
		return b.getEvalute();
	}

	int num_moves = 0;

	b.bitBoardMoveGenerator(moveArray[real_depth]);
	sortAttacks(moveArray[real_depth]);
	sortMoves(moveArray[real_depth], real_depth);

	BitMove local_move;

	if(moveArray[real_depth].count > 0) {
		local_move = moveArray[real_depth].moveArray[0];
	}

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

		if(real_depth == 0 && depth >= 9) {
			std::cout << "info currmove " << moveArray[real_depth].moveArray[i].getMoveString() << " currmovenumber " << num_moves << " time " << (int64_t)((clock() - start_timer) / (CLOCKS_PER_SEC / 1000)) << " nodes " << nodesCounter << " nps " << (int64_t)(nodesCounter / ((clock() - start_timer) / CLOCKS_PER_SEC)) << std::endl;
		}

		int tmp;

		tmp = -negamax_elementary(b, -(alpha + 1), -alpha, nextDepth, real_depth + 1, rule, inNullMove);

		if(tmp > alpha && tmp < beta) {
			tmp = -negamax_elementary(b, -beta, -alpha, nextDepth, real_depth + 1, rule, inNullMove);
		}

		b.goBack();


		if(tmp > eval) {
			eval = tmp;
		}

		if(tmp > alpha) {
			alpha = tmp;
			local_move = moveArray[real_depth].moveArray[i];


			if(!local_move.isAttack) {
				if(color == WHITE) {
//					whiteHistorySort[local_move.fromY][local_move.fromX][local_move.toY][local_move.toX] += pow(depth, 2);
					whiteKiller[real_depth] = Killer(local_move);
				} else {
//					blackHistorySort[local_move.fromY][local_move.fromX][local_move.toY][local_move.toX] += pow(depth, 2);
					blackKiller[real_depth] = Killer(local_move);
				}
			}
		}

		if(alpha >= beta) {
			if(!local_move.isAttack) {
				if(color == WHITE) {
					whiteHistorySort[local_move.fromY][local_move.fromX][local_move.toY][local_move.toX] += pow(depth, 2);
					//whiteKiller[real_depth] = Killer(local_move);
				} else {
					blackHistorySort[local_move.fromY][local_move.fromX][local_move.toY][local_move.toX] += pow(depth, 2);
					//blackKiller[real_depth] = Killer(local_move);
				}
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
		if((rule == FIXED_TIME && timer.getTime() >= time) || stopped) {
			return 0;
		}

		bestMove = local_move;
		bestScore = alpha;
	}

	if(real_depth == 0) {
		if(num_moves >= 0) {
			std::cout << "info depth " << max_depth << " time " << (int64_t)((clock() - start_timer) / (CLOCKS_PER_SEC / 1000)) << " nodes " << nodesCounter << " nps " << (int64_t)(nodesCounter / ((clock() - start_timer) / CLOCKS_PER_SEC));
			std::cout << " ";
			printScore(eval);
			std::cout << " pv ";
			printPV(depth);
		} else {
			std::cout << std::endl;
		}
	}

	return alpha;
}