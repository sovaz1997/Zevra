#include "game.hpp"

int64_t Game::negamax(BitBoard & b, int64_t alpha, int64_t beta, double depth, int real_depth, int rule, bool inNullMove) {
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


	int64_t oldAlpha = alpha;

	bool extended = false;

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
		return quies(b, alpha, beta, rule, real_depth);
	}

	if(b.inCheck(color)) {
		++nextDepth;
		extended = true;
		inNullMove = true;
	}/* else if(b.attacked) {
		nextDepth += 0.75;
		extended = true;
		inNullMove = true;
	}*/

	/*if(depth <= 2) {
		if(!extended && !b.attacked && !b.inCheck(color) && !inNullMove) {
			if(b.getEvalute() + PAWN_EV / 2 >= beta) {
				return beta;
			}
		}
	}*/

	int tmp;

	uint64_t hash = b.getColorHash();
	Hash* currentHash = &boardHash[hash & hash_cutter];

	if((currentHash->flag != EMPTY && currentHash->key == hash && !extended && !option.nullMovePrunningEnable) || (currentHash->flag != EMPTY && currentHash->key == hash && inNullMove && !extended && option.nullMovePrunningEnable)) {
		if(real_depth > 0 && currentHash->depth >= depth) {
			double score = currentHash->score;

			if(score > WHITE_WIN - 100) {
				score -= real_depth;
			} else if(score < -WHITE_WIN + 100) {
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
			tmp = -negamax(b, -beta, -alpha, depth - 1, real_depth + 1, rule, inNullMove);
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

	if(option.nullMovePrunningEnable) {
		if(!inNullMove && !inCheck && !extended && !b.attacked && real_depth > 4) {
			b.makeNullMove();
			int R = 2;
			/*if(depth > 6) {
				R = 4;
			} else {
				R = 3;
			}*/


			double value = -negamax(b, -beta, -(beta - 1), depth - R - 1, real_depth + 1, rule, true);
			if(value >= beta) {
				b.unMakeNullMove();
				return value;
			}

			b.unMakeNullMove();
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
			std::cout << "info currmove " << moveArray[real_depth].moveArray[i].getMoveString() << " currmovenumber " << num_moves << std::endl;
		}

		if(!option.lmrEnable) {
			tmp = -negamax(b, -(alpha + 1), -alpha, nextDepth, real_depth + 1, rule, inNullMove);

			if(tmp > alpha && tmp < beta) {
				tmp = -negamax(b, -beta, -alpha, nextDepth, real_depth + 1, rule, inNullMove);
			}
			

		} else {
			if(num_moves <= 3 || b.inCheck(color) || extended || inNullMove || b.attacked) {
				tmp = -negamax(b, -beta, -alpha, nextDepth, real_depth + 1, rule, inNullMove);
			} else {
				tmp = -negamax(b, -(alpha + 1), -alpha, nextDepth-1, real_depth + 1, rule, inNullMove);

				if(tmp > alpha) {
					tmp = -negamax(b, -beta, -alpha, nextDepth, real_depth + 1, rule, inNullMove);
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


			if(!local_move.isAttack) {
				if(color == WHITE) {
//					whiteHistorySort[local_move.fromY][local_move.fromX][local_move.toY][local_move.toX] += pow(depth, 2);
					whiteKiller[real_depth] = Killer(local_move);
				} else {
//					blackHistorySort[local_move.fromY][local_move.fromX][local_move.toY][local_move.toX] += pow(depth, 2);
					blackKiller[real_depth] = Killer(local_move);
				}
			}

			recordHash(depth, tmp, tmp<beta?EXACT:BETA, hash, moveArray[real_depth].moveArray[i], real_depth);
		}

		if(alpha >= beta) {
			if(!local_move.isAttack) {
				if(color == WHITE) {
					whiteHistorySort[local_move.fromY][local_move.fromX][local_move.toY][local_move.toX] += pow(depth, 2);
					whiteKiller[real_depth] = Killer(local_move);
				} else {
					blackHistorySort[local_move.fromY][local_move.fromX][local_move.toY][local_move.toX] += pow(depth, 2);
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

	if(alpha == oldAlpha) {
		recordHash(depth, alpha, ALPHA, hash, local_move, real_depth);
	}

	if(real_depth == 0) {
		if(num_moves >= 0) {
			std::cout << "info depth " << max_depth << " time " << (int)((clock() - start_timer) / (CLOCKS_PER_SEC / 1000)) << " nodes " << nodesCounter << " nps " << (int)(nodesCounter / ((clock() - start_timer) / CLOCKS_PER_SEC));
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
		/*if(combinations % 10000000 == 0) {
			std::cout << combinations << std::endl;
		}*/

		return 1;
	}

	game_board.bitBoardMoveGenerator(moveArray[depth]);

	for(unsigned int i = 0; i < moveArray[depth].count; ++i) {
		game_board.fastMove(moveArray[depth].moveArray[i]);
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

	uint8_t color = WHITE;

	/*if(!b.whiteMove) {
		color = BLACK;
	}

	if(b.inCheck(color)) {
		return negamax_elementary(b, alpha, beta, 1, real_depth, rule, false);
	}*/

	int64_t val = b.getEvalute();

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
	Hash* hash = &boardHash[key & hash_cutter];

	if(flag == ALPHA && (hash->flag == EXACT/* || hash->flag == BETA*/)) {
		return false;
	}

	if(hash->flag != EMPTY && hash->depth > depth) {
		return false;
	}

	if(score > WHITE_WIN - 100) {
		score += real_depth;
	} else if(score < -WHITE_WIN + 100) {
		score -= real_depth;
	}

	hash->depth = depth;
	hash->score = score;
	hash->flag = flag;
	hash->key = key;

	if(flag != ALPHA) {
		if(flag == EXACT) {
			hash->recordPrev();
		}

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

	bool extended = false;

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
			std::cout << "info currmove " << moveArray[real_depth].moveArray[i].getMoveString() << " currmovenumber " << num_moves << std::endl;
		}

		int tmp;

		tmp = -negamax(b, -(alpha + 1), -alpha, nextDepth, real_depth + 1, rule, inNullMove);

		if(tmp > alpha && tmp < beta) {
			tmp = -negamax(b, -beta, -alpha, nextDepth, real_depth + 1, rule, inNullMove);
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
			std::cout << "info depth " << max_depth << " time " << (int)((clock() - start_timer) / (CLOCKS_PER_SEC / 1000)) << " nodes " << nodesCounter << " nps " << (int)(nodesCounter / ((clock() - start_timer) / CLOCKS_PER_SEC));
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
