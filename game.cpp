#include "game.hpp"

Game::Game() {
	initEngine();

	/*std::ifstream f("long_opennings.pgn");
	std::string str;
	std::vector<Debute>base_vec;
	FILE* base = fopen("base.bin", "wb");

	int h = 0;
	while(std::getline(f, str)) {
		++h;
		std::cout << h << "\n";
		if(str.size() > 0) {
			if(str[0] != '[') {
				std::vector<std::string> moves = getStringArray(str);
				uciHandler("position startpos");
				for(int i = 0; i < moves.size() - 1; ++i) {

					Move mv = Move(moves[i]);
					base_vec.push_back(Debute(getColorHash(game_board), mv));
					move(moves[i]);
				}
			}
		}
	}

	std::sort(base_vec.begin(), base_vec.end());

	for(int i = 0; i < base_vec.size(); ++i) {
		Debute* deb = new Debute();
		deb->hash = base_vec[i].hash;
		deb->move = base_vec[i].move;
		//base.write(deb, 4);
		fwrite(&deb, sizeof(deb), 1, base);
		delete deb;
	}

	std::cout << "success!\n";*/

	startGame();
}

int Game::startGame() {
	std::string str;
	while(true) {
		std::getline(std::cin, str);

		if(!uciHandler(str)) {
			return 0;
		};
	}
}

bool Game::uciHandler(std::string str) {
	std::vector<std::string> cmd = getStringArray(str);
		if(cmd[0] == "isready") {
			std::cout << "readyok\n";
		} else if(cmd[0] == "position") {
			gameHash.clear();
			if(cmd[1] == "startpos") {
				game_board.setFenPosition("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
				if(cmd.size() > 3) {
					if(cmd[2] == "moves") {
						for(unsigned int i = 3; i < cmd.size(); ++i) {
							move(cmd[i]);
						}
					}
				}
			} else if(cmd[1] == "fen") {
				std::string fen;
				unsigned int pos = 2;

				for(unsigned int i = 2; i < 8; ++i) {
					fen += cmd[i];
					if(i != cmd.size() - 1) {
						fen.push_back(' ');
					}
					++pos;
				}

				game_board.setFenPosition(fen);
				if(cmd.size() > pos) {
					if(cmd[pos] == "moves") {
						for(unsigned int i = pos + 1; i < cmd.size(); ++i) {
							move(cmd[i]);
						}
					}
				}
			}

			gameHash.push_back(getHash(game_board));

		} else if(cmd[0] == "go") {
			if(cmd[1] == "depth") {
				max_depth = std::stoi(cmd[2]);
				go();
			}
		} else if(cmd[0] == "posmoves") {
			bool shah_tmp;
			std::vector<Move>moves = generatePositionMoves(game_board, shah_tmp, true, 0);
			for(unsigned int i = 0; i < moves.size(); ++i) {
				moves[i].print();
				std::cout << "\n";
			}

			//game_board.printBoard();
			std::cout << evalute(game_board) / PAWN_EV * 100 << "\n";
			//std::cout << "info score: " << evalute(game_board) / PAWN_EV << "\n";
		} else if(cmd[0] == "move") {
			move(cmd[1]);
		} else if(cmd[0] == "quit") {
			return false;
		}

		return true;
}

void Game::go() {
	/*if(debute) {
		FILE* base_file = fopen("base.bin", "rb");
		uint64_t hash = getColorHash(game_board);
		std::vector<Debute>possibleMove;
		Debute deb;

		fseek(base_file, 0, SEEK_END);
		int l = -1;
		int r = ftell(base_file) /sizeof(deb);
		while (l < r - 1) {
			int m = (l + r) / 2;
			fseek(base_file, m * sizeof(deb), SEEK_SET);
			fread(&deb, sizeof(deb), 1, base_file);
			if (deb.hash < getColorHash(game_board)) {
				l = m;
			}
			else {
				r = m;
			}
		}
			for(; fread(&deb, sizeof(deb), 1, base_file); ++r) {
				fseek(base_file, r * sizeof(deb), SEEK_SET);
				if(hash != deb.hash) {
					break;
				}

				possibleMove.push_back(deb);
			}

		if(possibleMove.size() > 0) {
			srand(time(0));
			Move mv = possibleMove[rand() % possibleMove.size()].move;
			move(mv.getMoveString());
			possibleMove.clear();
		} else {
			debute = false;
		}
		fclose(base_file);
	}*/

	//if(!debute) {
		 whiteKiller.clear();
		 blackKiller.clear();
		 whiteKiller.resize(100);
		 blackKiller.resize(100);


		variant.clear();
		variant.resize(max_depth);
		double win;
		std::vector<uint64_t>hash;

		pv_best.clear();
		std::vector<Move>pv;
		whiteUp = BLACK_WIN;
		blackUp = WHITE_WIN;

		bool basis = false;
		movesCounter = 0;
		double start_timer = clock();
		//boardHash.clear();

		for(int i = max_depth; i <= max_depth; ++i) {
			if(i == max_depth) {
				basis = true;
			}

			if(game_board.isWhiteMove()) {
				win = minimax_white(game_board, -INFINITY, INFINITY, 0, i, 0, gameHash, basis, pv, true);
				//std::cout << "info " << movesCounter << " positions; " << (double)movesCounter / ((end_timer - start_timer) / 1000000) << " position/sec\n";
			} else {
				movesCounter = 0;
				double start_timer = clock();
				win = minimax_black(game_board, -INFINITY, INFINITY, 0, i, 0, gameHash, basis, pv, true);
				double end_timer = clock();
				//std::cout << "info " << movesCounter << " positions; " << (double)movesCounter / ((end_timer - start_timer) / 1000000) << " position/sec\n";
			}
		}
		double end_timer = clock();

		if(win > BLACK_WIN + 10000 && win < WHITE_WIN - 10000) {
				//std::cout << "info score: " << std::fixed << std::setprecision(2) << win / PAWN_EV << "\n";
		} else if(win < 0) {
			//std::cout << "info score: " << "black win in " <<  abs(win - BLACK_WIN) << " plies" << "\n";
		} else {
			//std::cout << "info score: " << "white win in " <<  abs(win - WHITE_WIN) << " plies" << "\n";
		}
	//}

	for(int i = 0; i < pv_best.size(); ++i) {
		pv_best[i].print();
		std::cout << " ";
	}

	std::cout << "\n";
}

bool Game::move(std::string mv) {
	bool tmp_shah = false;
	std::vector<Move>moves = generatePositionMoves(game_board, tmp_shah, true, 0);

	for(unsigned int i = 0; i < moves.size(); ++i) {
		if(moves[i].getMoveString() == mv) {
			Board tmp = game_board;
			game_board.move(moves[i]);
			bool shah = false;
			std::vector<Move>moves = generatePositionMoves(game_board, shah, true, 0);

			if(shah) {
				game_board = tmp;
				continue;
			}

			gameHash.push_back(getHash(game_board));

			return true;
		}
	}

	return false;
}

std::vector<std::string> Game::getStringArray(std::string str) {
	std::vector<std::string> result;
	std::string tmp;
	for(unsigned int i = 0; i < str.size(); ++i) {
		if(str[i] == ' ') {
			result.push_back(tmp);
			tmp.clear();
		} else {
			tmp.push_back(str[i]);
		}
	}

	result.push_back(tmp);

	return result;
}

void Game::initEngine() {
	cache_gen();
	whiteKiller.resize(100);
	blackKiller.resize(100);
	boardHash.resize(pow(2, hash_width));
	zobristGenerate();
	hash_cutter = pow(2, hash_width) - 1;

	for(int i = 0; i < BOARD_SIZE; ++i) {
		for(int j = 0; j < BOARD_SIZE; ++j) {
			for(int k = 0; k < BOARD_SIZE; ++k) {
				for(int m = 0; m < BOARD_SIZE; ++m) {
					evalute_cells_size[i][j][k][m]  = sqrt(pow(i - k, 2) + pow(j - m, 2)) + 5;
				}
			}
		}
	}

	std::cout << "uciok\n";
}

double Game::minimax_white(Board b, double alpha, double beta, int depth, int max_depth, int real_depth, std::vector<uint64_t> hash, bool basis, std::vector<Move>pv, bool usedNullMove) {
	++movesCounter;
	/*if(max_depth - real_depth > 4) {
		std::cout << "info nodes " << movesCounter << "\n";
	}*/

	uint64_t pos_hash = getHash(b);
	uint64_t board_hash = getColorHash(b);

	if(depth > 0) {
	int third_repeat = 1;
		for(int i = 0; i < hash.size(); ++i) {
			if(hash[i] == pos_hash) {
				++third_repeat;
			}

			if(third_repeat >= 3) {
				return 0;
			}
		}
	}

	if(boardHash[board_hash & hash_cutter].enable && boardHash[board_hash & hash_cutter].depth == max_depth - real_depth) {
	/*	if(depth == 0 && basis) {
			//game_board.move(local_move);
			std::cout << "info pv " << boardHash[board_hash & hash_cutter].move.getMoveString();
			std::cout << " score ";
			if(boardHash[board_hash & hash_cutter].evalute > BLACK_WIN + 10000 && boardHash[board_hash & hash_cutter].evalute < WHITE_WIN - 10000) {
			std::cout << "cp " << (int)(boardHash[board_hash & hash_cutter].evalute / PAWN_EV * 100);
			} else if(boardHash[board_hash & hash_cutter].evalute < 0) {
				std::cout << "mate " <<  -abs(boardHash[board_hash & hash_cutter].evalute) - BLACK_WIN;
			} else {
				std::cout << "mate " <<  abs(boardHash[board_hash & hash_cutter].evalute) - WHITE_WIN;
			}
			std::cout << " nodes " << movesCounter << "\n";
			std::cout << "bestmove " << boardHash[board_hash & hash_cutter].move.getMoveString() << "\n";
			gameHash.push_back(getHash(game_board));
		}*/

	//	return boardHash[board_hash & hash_cutter].evalute;
	}

	int num_moves = 0;

	if(depth == max_depth) {
		double eval = evalute(b);
		/*if(eval >= alpha && eval <= beta) {
			bool printing = true;
			if(game_board.whiteMove) {
				if(eval > whiteUp) {
					whiteUp = eval;
					printing = true;
				}
			} else {
				if(eval < blackUp) {
					blackUp = eval;
					printing = true;
				}
			}

			pv_best = pv;
			if(printing) {
				std::cout << "info pv ";
				for(int i = 0; i < pv_best.size(); ++i) {
					pv_best[i].print();
					std::cout << " ";
				}
				std::cout << "score cp " << eval / PAWN_EV * 100 << "\n";
			}
		}
		//return force_minimax_white(b, real_depth + 1, hash, basis);*/
		return eval;
	}

	double max = BLACK_WIN;

	bool null_mv = nullMoveEnable;

	bool tmp_shah;
	std::vector<Move>moves = generatePositionMoves(b, tmp_shah, true, real_depth);

	Move local_move;

	if(moves.size() > 0) {
		local_move = moves[0];
	}

	for(unsigned int i = 0; i < moves.size(); ++i) {
		Board tmp_brd = b;
		tmp_brd.move(moves[i]);

		bool shah = false;

		if(!shahIsNot(tmp_brd, WHITE)) {
			std::vector<Move>moves_shah = generatePositionMoves(tmp_brd, shah, true, real_depth);
			if(shah) {
				continue;
			}
		}

		++num_moves;

		double tmp;

		pv.push_back(moves[i]);
		hash.push_back(pos_hash);
		tmp = minimax_black(tmp_brd, alpha, beta, depth + 1, max_depth, real_depth + 1, hash, basis, pv, true);
		pv.pop_back();

		if(nullMoveEnable && (max_depth - depth) >= 3 && tmp_brd.getFigure(moves[i].toX, moves[i].toY) == 0 && shahIsNot(tmp_brd, BLACK) && usedNullMove) {
			double value = minimax_black(tmp_brd, alpha, beta, depth + 3, max_depth, real_depth + 3, hash, basis, pv, false);
			if(value >= beta) {
				return beta;
			}
		}

		if(tmp > max) {
			max = tmp;
			local_move = moves[i];
		}
		if(tmp > alpha) {
			alpha = tmp;
			if(b.getFigure(moves[i].toY, moves[i].toX) == 0 && local_move.quality() && b.getFigure(moves[i].fromY, moves[i].fromX) | COLOR_SAVE == WHITE) {
				whiteKiller[real_depth] = Killer(local_move);
			}
		}
		if(max >= beta) {
			if(hashEnable) {
				if(!boardHash[board_hash & hash_cutter].enable) {
					boardHash[board_hash & hash_cutter] = Hash(board_hash, local_move, max_depth - real_depth, max);
				} else {
					if(boardHash[board_hash & hash_cutter].depth <= max_depth - real_depth) {
						boardHash[board_hash & hash_cutter] = Hash(board_hash, local_move, max_depth - real_depth, max);
					}
				}
			}

			if(depth == 0) {
				std::cout << "info pv " << local_move.getMoveString();
				std::cout << " nodes " << movesCounter;
				if(basis) {
					std::cout << " score ";
					if(max > BLACK_WIN + 10000 && max < WHITE_WIN - 10000) {
						std::cout << "cp " << (int) (max / PAWN_EV * 100);
					} else if(max < 0) {
						std::cout << "mate " <<  -abs(max - BLACK_WIN);
					} else {
						std::cout << "mate " <<  abs(max - WHITE_WIN);
					}
					std::cout << "bestmove " << local_move.getMoveString();
					gameHash.push_back(getHash(game_board));
				}
				std::cout << "\n";
			}

			variant[max_depth - real_depth - 1] = moves[i].getMoveString();
			printVariant();
			return max;
		}
	}



	if(num_moves == 0) {
		bool shah = false;
		Board tmp = b;
		tmp.whiteMove = false;
		std::vector<Move>moves_shah = generatePositionMoves(tmp, shah, true, real_depth);

		if(shah) {
			return BLACK_WIN + real_depth;
		} else {
			return 0;
		}
	}

	if(depth == 0 && num_moves > 0) {
		std::cout << "info pv " << local_move.getMoveString();
		std::cout << " nodes " << movesCounter;
		if(basis) {
			std::cout << " score ";
			if(max > BLACK_WIN + 10000 && max < WHITE_WIN - 10000) {
				std::cout << "cp " << (int) (max / PAWN_EV * 100);
			} else if(max < 0) {
				std::cout << "mate " <<  -abs(max - BLACK_WIN);
			} else {
				std::cout << "mate " <<  abs(max - WHITE_WIN);
			}

			std::cout << "\nbestmove " << local_move.getMoveString();
			gameHash.push_back(getHash(game_board));
		}
		std::cout << "\n";
	}

	printVariant();

	if(hashEnable) {
		if(!boardHash[board_hash & hash_cutter].enable) {
			boardHash[board_hash & hash_cutter] = Hash(board_hash, local_move, max_depth - real_depth, max);
		} else {
			if(boardHash[board_hash & hash_cutter].depth <= max_depth - real_depth) {
				boardHash[board_hash & hash_cutter] = Hash(board_hash, local_move, max_depth - real_depth, max);
			}
		}
	}

	return max;
}


double Game::minimax_black(Board b, double alpha, double beta, int depth, int max_depth, int real_depth, std::vector<uint64_t> hash, bool basis, std::vector<Move>pv, bool usedNullMove) {
	++movesCounter;

	/*if(max_depth - real_depth > 4) {
		std::cout << "info nodes " << movesCounter << "\n";
	}*/

	uint64_t pos_hash = getHash(b);
	uint64_t board_hash = getColorHash(b);

	if(depth > 0) {
		int third_repeat = 1;
		for(int i = 0; i < hash.size(); ++i) {
			if(hash[i] == pos_hash) {
				++third_repeat;
			}

			if(third_repeat >= 3) {
				return 0;
			}
		}
	}

	if(boardHash[board_hash & hash_cutter].enable && boardHash[board_hash & hash_cutter].depth == max_depth - real_depth) {
		/*if(depth == 0 && basis) {
			//game_board.move(local_move);
			std::cout << "score ";
			if(boardHash[board_hash & hash_cutter].evalute > BLACK_WIN + 10000 && boardHash[board_hash & hash_cutter].evalute < WHITE_WIN - 10000) {
			std::cout << "cp " << -(int)(boardHash[board_hash & hash_cutter].evalute / PAWN_EV * 100);
			} else if(boardHash[board_hash & hash_cutter].evalute < 0) {
				std::cout << "mate " <<  abs(boardHash[board_hash & hash_cutter].evalute) - BLACK_WIN;
			} else {
				std::cout << "mate " <<  -abs(boardHash[board_hash & hash_cutter].evalute) - WHITE_WIN;
			}

			std::cout << "\nbestmove " << boardHash[board_hash & hash_cutter].move.getMoveString() << "\n";
			gameHash.push_back(getHash(game_board));
		}*/

	//	return boardHash[board_hash & hash_cutter].evalute;
	}

	int num_moves = 0;
	if(depth == max_depth) {
		double eval = evalute(b);
		/*if(eval >= alpha && eval <= beta) {
			bool printing = true;
			if(game_board.whiteMove) {
				if(eval > whiteUp) {
					whiteUp = eval;
					printing = true;
				}
			} else {
				if(eval < blackUp) {
					blackUp = eval;
					printing = true;
				}
			}

			pv_best = pv;
			if(printing) {
				std::cout << "info pv ";
				for(int i = 0; i < pv_best.size(); ++i) {
					pv_best[i].print();
					std::cout << " ";
				}
				std::cout << "score cp " << eval / PAWN_EV * 100 << "\n";
			}
		}
		//return force_minimax_black(b, real_depth + 1, hash, basis);
	//	return evalute(b);*/
		return eval;
	}

	double min = WHITE_WIN;

	bool tmp_shah;
	std::vector<Move>moves = generatePositionMoves(b, tmp_shah, true, real_depth);

	Move local_move;

	if(moves.size() > 0) {
		local_move = moves[0];
	}

//	bool null_mv = nullMoveEnable;

	for(unsigned int i = 0; i < moves.size(); ++i) {
		Board tmp_brd = b;
		tmp_brd.move(moves[i]);

		bool shah = false;
		if(!shahIsNot(tmp_brd, BLACK)) {
			std::vector<Move>moves_shah = generatePositionMoves(tmp_brd, shah, true, real_depth);
			if(shah) {
				continue;
			}
		}

		++num_moves;
		double tmp;
		pv.push_back(moves[i]);
		hash.push_back(pos_hash);
		tmp = minimax_white(tmp_brd, alpha, beta, depth + 1, max_depth, real_depth + 1, hash, basis, pv, true);
		pv.pop_back();

		if((max_depth - depth) >= 3 && tmp_brd.getFigure(moves[i].toX, moves[i].toY) == 0 && shahIsNot(tmp_brd, WHITE) && nullMoveEnable && usedNullMove) {
			double value = minimax_white(tmp_brd, alpha, beta, depth + 3, max_depth, real_depth + 3, hash, basis, pv, false);
			if(value <= alpha) {
				return alpha;
			}

			//null_mv = false;
		}

		if(tmp < min) {
			min = tmp;
			local_move = moves[i];
		}

		if(tmp < beta) {
			beta = tmp;
			if(b.getFigure(moves[i].toY, moves[i].toX) == 0 && local_move.quality() && b.getFigure(moves[i].fromY, moves[i].fromX) | COLOR_SAVE == BLACK) {
				blackKiller[real_depth] = Killer(local_move);
			}
		}

		if(min <= alpha) {
			if(hashEnable) {
				if(!boardHash[board_hash & hash_cutter].enable) {
					boardHash[board_hash & hash_cutter] = Hash(board_hash, local_move, max_depth - real_depth, min);
				} else {
					if(boardHash[board_hash & hash_cutter].depth <= max_depth - real_depth) {
						boardHash[board_hash & hash_cutter] = Hash(board_hash, local_move, max_depth - real_depth, min);
					}
				}
			}

			if(depth == 0) {
				std::cout << "info pv " << local_move.getMoveString();
				std::cout << " nodes " << movesCounter;
				if(basis) {
					std::cout << " score ";
					if(min > BLACK_WIN + 10000 && min < WHITE_WIN - 10000) {
						std::cout << "cp " << -(int) (min / PAWN_EV * 100);
					} else if(min < 0) {
						std::cout << "mate " <<  abs(min - BLACK_WIN);
					} else {
						std::cout << "mate " <<  -abs(min - WHITE_WIN);
					}

					std::cout << "\nbestmove " << local_move.getMoveString();
					gameHash.push_back(getHash(game_board));
				}
				std::cout << "\n";
			}

			variant[max_depth - real_depth - 1] = moves[i].getMoveString();
			printVariant();

			return min;
		}
	}

	if(num_moves == 0) {
		bool shah = false;
		Board tmp = b;
		tmp.whiteMove = true;
		std::vector<Move>moves_shah = generatePositionMoves(tmp, shah, true, real_depth);

		if(shah) {
			return WHITE_WIN - real_depth;
		} else {
			return 0;
		}
	}

	if(depth == 0 && num_moves > 0) {
		std::cout << "info pv " << local_move.getMoveString();
		std::cout << " nodes " << movesCounter;
		if(basis) {
			//game_board.move(local_move);
			std::cout << " score ";
			if(min > BLACK_WIN + 10000 && min < WHITE_WIN - 10000) {
			std::cout << "cp " << -(int)(min / PAWN_EV * 100);
			} else if(min < 0) {
				std::cout << "mate " <<  abs(min - BLACK_WIN);
			} else {
				std::cout << "mate " <<  -abs(min - WHITE_WIN);
			}

			std::cout << "\nbestmove " << local_move.getMoveString();
			gameHash.push_back(getHash(game_board));
		}
		std::cout << "\n";
	}

	printVariant();

	if(hashEnable) {
		if(!boardHash[board_hash & hash_cutter].enable) {
			boardHash[board_hash & hash_cutter] = Hash(board_hash, local_move, max_depth - real_depth, min);
		} else {
			if(boardHash[board_hash & hash_cutter].depth <= max_depth - real_depth) {
				boardHash[board_hash & hash_cutter] = Hash(board_hash, local_move, max_depth - real_depth, min);
			}
		}
	}

	return min;
}

double Game::force_minimax_white(Board b, int real_depth, std::vector<uint64_t> hash, bool basis) {
	++movesCounter;

	uint64_t pos_hash = getHash(b);
	uint64_t board_hash = getColorHash(b);


	int num_moves = 0;

	double max = BLACK_WIN;

	bool tmp_shah;
	std::vector<Move>moves = generatePositionMoves(b, tmp_shah, true, real_depth);

	Move local_move;

	if(moves.size() > 0) {
		local_move = moves[0];
	}

	for(unsigned int i = 0; i < moves.size(); ++i) {
		Board save = b;

		if(b.getFigure(moves[i].toY, moves[i].toX) == 0 || moves[i].getAttackPrice(b) < 0) {
			b = save;
			break;
		}

		b.move(moves[i]);

		bool shah = false;

		++num_moves;

		double tmp;

		tmp = force_minimax_black(b, real_depth + 1, hash, basis);

		b = save;
		if(tmp > max) {
			max = tmp;
			local_move = moves[i];
		}
	}

	if(num_moves == 0 && num_moves == moves.size()) {
		bool shah = false;
		Board tmp = b;
		tmp.whiteMove = false;
		std::vector<Move>moves_shah = generatePositionMoves(tmp, shah, true, real_depth);

		if(shah) {
			return BLACK_WIN + real_depth;
		} else {
			return 0;
		}
	}

	if(num_moves == 0) {
		return evalute(b);
	}

	printVariant();

	return max;
}

double Game::force_minimax_black(Board b, int real_depth, std::vector<uint64_t> hash, bool basis) {
	++movesCounter;

	uint64_t pos_hash = getHash(b);
	uint64_t board_hash = getColorHash(b);


	int num_moves = 0;

	double min = WHITE_WIN;

	bool tmp_shah;
	std::vector<Move>moves = generatePositionMoves(b, tmp_shah, true, real_depth);

	Move local_move;

	if(moves.size() > 0) {
		local_move = moves[0];
	}

	for(unsigned int i = 0; i < moves.size(); ++i) {
		Board save = b;

		if(b.getFigure(moves[i].toY, moves[i].toX) == 0 || moves[i].getAttackPrice(b) < 0) {
			b = save;
			break;
		}

		b.move(moves[i]);

		bool shah = false;

		++num_moves;

		double tmp;

		tmp = force_minimax_white(b, real_depth + 1, hash, basis);

		b = save;
		if(tmp < min) {
			min = tmp;
			local_move = moves[i];
		}
	}

	if(num_moves == 0 && num_moves == moves.size()) {
		bool shah = false;
		Board tmp = b;
		tmp.whiteMove = true;
		std::vector<Move>moves_shah = generatePositionMoves(tmp, shah, true, real_depth);

		if(shah) {
			return WHITE_WIN - real_depth;
		} else {
			return 0;
		}
	}

	if(num_moves == 0) {
		return evalute(b);
	}

	printVariant();

	return min;
}

//position fen 1r1qkb1r/pppbnp1p/3pp1p1/8/3PP3/PPN1BN2/1P3PPP/R2Q1RK1 b k - 0 10

double Game::evalute(Board b) {
	double material = 0;
	double position = 0;
	double all_material = 0;
	double security_king = 0;

	int xWhiteKingPos, yWhiteKingPos, xBlackKingPos, yBlackKingPos, whitePawns = 0, blackPawns = 0;;

	for(int y = 0; y < BOARD_SIZE; ++y) {
		for(int x = 0; x < BOARD_SIZE; ++x) {
			int tmp = material;
			if(b.getFigure(y, x) == 0) {
				continue;
			}

			if((b.getFigure(y, x) & COLOR_SAVE) == WHITE) {
				if((b.getFigure(y, x) & TYPE_SAVE) == PAWN) {
					material += PAWN_EV;
					all_material += PAWN_EV;
					++whitePawns;
				} else if((b.getFigure(y, x) & TYPE_SAVE) == KNIGHT) {
					material += KNIGHT_EV;
					all_material += KNIGHT_EV;
				} else if((b.getFigure(y, x) & TYPE_SAVE) == BISHOP) {
					material += BISHOP_EV;
					all_material += BISHOP_EV;
				} else if((b.getFigure(y, x) & TYPE_SAVE) == ROOK) {
					material += ROOK_EV;
					all_material += ROOK_EV;
				} else if((b.getFigure(y, x) & TYPE_SAVE) == QUEEN) {
					material += QUEEN_EV;
					all_material += QUEEN_EV;
				} else if((b.getFigure(y, x) & TYPE_SAVE) == KING) {
					xWhiteKingPos = x;
					yWhiteKingPos = y;
				}
			} else {
				if((b.getFigure(y, x) & TYPE_SAVE) == PAWN) {
					material -= PAWN_EV;
					all_material += PAWN_EV;
					++blackPawns;
				} else if((b.getFigure(y, x) & TYPE_SAVE) == KNIGHT) {
					material -= KNIGHT_EV;
					all_material += KNIGHT_EV;
				} else if((b.getFigure(y, x) & TYPE_SAVE) == BISHOP) {
					material -= BISHOP_EV;
					all_material += BISHOP_EV;
				} else if((b.getFigure(y, x) & TYPE_SAVE) == ROOK) {
					material -= ROOK_EV;
					all_material += ROOK_EV;
				} else if((b.getFigure(y, x) & TYPE_SAVE) == QUEEN) {
					material -= QUEEN_EV;
					all_material += QUEEN_EV;
				} else if((b.getFigure(y, x) & TYPE_SAVE) == KING) {
					xBlackKingPos = x;
					yBlackKingPos = y;
				}
			}
		}
	}

	for(int y = 0; y < BOARD_SIZE; ++y) {
		for(int x = 0; x < BOARD_SIZE; ++x) {
			if(b.getFigure(y, x) == 0) {
				continue;
			}

			double whiteKingDistance, blackKingDistance;
			if(xWhiteKingPos >= 0 && xWhiteKingPos < 8 && yWhiteKingPos >= 0 && yWhiteKingPos < 8 && xBlackKingPos >= 0 && xBlackKingPos < 8 && yBlackKingPos >= 0 && yBlackKingPos < 8) {
				whiteKingDistance = evalute_cells_size[y][x][yWhiteKingPos][xWhiteKingPos];
				blackKingDistance = evalute_cells_size[y][x][yBlackKingPos][xBlackKingPos];
			} else {
				whiteKingDistance = 5;
				blackKingDistance = 5;
			}

			if((b.getFigure(y, x) & COLOR_SAVE) == WHITE) {
				if((b.getFigure(y, x) & TYPE_SAVE) == PAWN) {
					position += whitePawnMatr[y][x];
					security_king += (500 / whiteKingDistance);
					security_king -= (100 / blackKingDistance);
				} else if((b.getFigure(y, x) & TYPE_SAVE) == KNIGHT) {
					position += knightMatr[y][x];
					security_king += (300 / whiteKingDistance);
					security_king -= (300 / blackKingDistance);
				} else if((b.getFigure(y, x) & TYPE_SAVE) == BISHOP) {
					position += bishopMatr[y][x];
					security_king += (400 / whiteKingDistance);
					security_king -= (200 / blackKingDistance);
				} else if((b.getFigure(y, x) & TYPE_SAVE) == ROOK) {
					position += rookMatr[y][x];
					security_king += (200 / whiteKingDistance);
					security_king -= (400 / blackKingDistance);
				} else if((b.getFigure(y, x) & TYPE_SAVE) == QUEEN) {
					position += queenMatr[y][x];
					security_king += (100 / whiteKingDistance);
					security_king -= (500 / blackKingDistance);
				} else if((b.getFigure(y, x) & TYPE_SAVE) == KING) {
					position += ((kingDebuteMatr[y][x] * (all_material / ALL_MATERIAL)) + kingEndGameMatr[y][x] *  (1 - all_material / ALL_MATERIAL));
				}
			} else {
				if((b.getFigure(y, x) & TYPE_SAVE) == PAWN) {
					position -= whitePawnMatr[BOARD_SIZE - 1 - y][x];
					security_king -= (500 / blackKingDistance);
					security_king += (100 / whiteKingDistance);
				} else if((b.getFigure(y, x) & TYPE_SAVE) == KNIGHT) {
					position -= knightMatr[BOARD_SIZE - 1 - y][x];
					security_king -= (300 / blackKingDistance);
					security_king += (300 / whiteKingDistance);
				} else if((b.getFigure(y, x) & TYPE_SAVE) == BISHOP) {
					position -= bishopMatr[BOARD_SIZE - 1 - y][x];
					security_king -= (400 / blackKingDistance);
					security_king += (200 / whiteKingDistance);
				} else if((b.getFigure(y, x) & TYPE_SAVE) == ROOK) {
					position -= rookMatr[BOARD_SIZE - 1 - y][x];
					security_king -= (200 / blackKingDistance);
					security_king += (400 / whiteKingDistance);
				} else if((b.getFigure(y, x) & TYPE_SAVE) == QUEEN) {
					position -= queenMatr[BOARD_SIZE - 1 - y][x];
					security_king -= (100 / blackKingDistance);
					security_king += (500 / whiteKingDistance);
				} else if((b.getFigure(y, x) & TYPE_SAVE) == KING) {
					position -= ((kingDebuteMatr[BOARD_SIZE - 1 - y][x] * (all_material / ALL_MATERIAL)) + kingEndGameMatr[BOARD_SIZE - 1 - y][x] *  (1 - all_material / ALL_MATERIAL));
				}
			}
		}
	}

	/*if(all_material <= 25000) {
		material += whitePawns * PAWN_EV;
		material -= blackPawns * PAWN_EV;
	}*/

//	std::cout << material << " " << position << "\n";
	return material + position/* + security_king / 1000*/;
}

std::vector<Move> Game::generatePositionMoves(Board b, bool & shah, bool withCastling, int depth) {
	std::vector<Move>result;

	int num_attacks = 0;

	uint8_t color;

	if(b.isWhiteMove()) {
		color = WHITE;
	} else {
		color = BLACK;
	}

	for(int y = 0; y < BOARD_SIZE; ++y) {
		for(int x = 0; x < BOARD_SIZE; ++x) {
			if(b.getFigure(y, x) != 0) {
				if((b.getFigure(y, x) & COLOR_SAVE) == color) {
					MoveList current_list = move_list[b.getFigure(y, x)][y][x];
					if(current_list.first == NULL) { continue; }
					MoveItem* it = current_list.first;
					bool stopped = false;

					do {
						if(it->move.fromX >= 0 && it->move.fromX < 8 && it->move.fromY >= 0 && it->move.fromY < 8 && it->move.toX >= 0 && it->move.toX < 8 && it->move.toY >= 0 && it->move.toY < 8) {
							if(withCastling) {
								if(it->move.moveType == WS_CASTLING_MV && b.whiteShortCastleEnable) {
									if(b.getFigure(y, x + 1) == 0 && b.getFigure(y, x + 2) == 0) {
										Board tmp = b;
										tmp.whiteMove = false;
										bool shah_tmp = false;
										std::vector<Move> move_test = generatePositionMoves(tmp, shah_tmp, false, 0);

										for(unsigned int i = 0; i < move_test.size(); ++i) {
											if((move_test[i].toY == 7 && move_test[i].toX == 6) || (move_test[i].toY == 7 && move_test[i].toX == 5) || (move_test[i].toY == 7 && move_test[i].toX == 4)) {
												break;
											}

											if(i == move_test.size() - 1) {
												if(!shah_tmp) {
													result.push_back(it->move);
												}
											}
										}
									}
								}

								if(it->move.moveType == WL_CASTLING_MV && b.whiteLongCastleEnable) {
									if(b.getFigure(y, x - 1) == 0 && b.getFigure(y, x - 2) == 0 && b.getFigure(y, x - 3) == 0) {
										Board tmp = b;
										tmp.whiteMove = false;
										bool shah_tmp = false;
										std::vector<Move> move_test = generatePositionMoves(tmp, shah_tmp, false, 0);

										for(unsigned int i = 0; i < move_test.size(); ++i) {
											if((move_test[i].toY == 7 && move_test[i].toX == 2) || (move_test[i].toY == 7 && move_test[i].toX == 3) || (move_test[i].toY == 7 && move_test[i].toX == 4)) {
												break;
											}

											if(i == move_test.size() - 1) {
												if(!shah_tmp) {
													result.push_back(it->move);
												}
											}
										}
									}
								}

								if(it->move.moveType == BS_CASTLING_MV && b.blackShortCastleEnable) {
									if(b.getFigure(y, x + 1) == 0 && b.getFigure(y, x + 2) == 0) {
										Board tmp = b;
										tmp.whiteMove = true;
										bool shah_tmp = false;
										std::vector<Move> move_test = generatePositionMoves(tmp, shah_tmp, false, 0);

										for(unsigned int i = 0; i < move_test.size(); ++i) {
											if((move_test[i].toY == 0 && move_test[i].toX == 6) || (move_test[i].toY == 0 && move_test[i].toX == 5) || (move_test[i].toY == 0 && move_test[i].toX == 4)) {
												break;
											}

											if(i == move_test.size() - 1) {
												if(!shah_tmp) {
													result.push_back(it->move);
												}
											}
										}
									}
								}

								if(it->move.moveType == BL_CASTLING_MV && b.blackLongCastleEnable) {
									if(b.getFigure(y, x - 1) == 0 && b.getFigure(y, x - 2) == 0 && b.getFigure(y, x - 3) == 0) {
										Board tmp = b;
										tmp.whiteMove = true;
										bool shah_tmp = false;
										std::vector<Move> move_test = generatePositionMoves(tmp, shah_tmp, false, 0);
										for(unsigned int i = 0; i < move_test.size(); ++i) {
											if((move_test[i].toY == 0 && move_test[i].toX == 2) || (move_test[i].toY == 0 && move_test[i].toX == 3) || (move_test[i].toY == 0 && move_test[i].toX == 4)) {
												break;
											}

											if(i == move_test.size() - 1) {
												Board tmp = b;
												if(!shah_tmp) {
													result.push_back(it->move);
												}
											}
										}
									}
								}
							}

							if(it->move.moveType != WS_CASTLING_MV && it->move.moveType != WL_CASTLING_MV && it->move.moveType != BS_CASTLING_MV && it->move.moveType != BL_CASTLING_MV) {
								if((b.getFigure(y, x) & TYPE_SAVE) != PAWN) {
									if(b.getFigure(it->move.toY, it->move.toX) == 0) {
										result.push_back(it->move);
									} else if((b.getFigure(it->move.toY, it->move.toX) & COLOR_SAVE) == color || (b.getFigure(it->move.toY, it->move.toX) & TYPE_SAVE) == KING) {
										if((b.getFigure(it->move.toY, it->move.toX) & TYPE_SAVE) == KING && (b.getFigure(it->move.toY, it->move.toX) & COLOR_SAVE) != color) {
											shah = true;
										}
										if(it->specialNext != NULL) {
											it = it->specialNext;
											continue;
										} else {
											break;
										}
									} else {
										//result.push_back(it->move);
										result.insert(result.begin(), it->move);
										++num_attacks;
										if(it->specialNext != NULL) {
											it = it->specialNext;
											continue;
										} else {
											break;
										}
									}

								} else {
									if(it->move.fromX == it->move.toX) {
										if(b.getFigure(it->move.toY, it->move.toX) != 0 && it->specialNext != NULL) {
											it = it->specialNext;
											continue;
										} else if(game_board.getFigure(it->move.toY, it->move.toX) == 0) {
											result.push_back(it->move);
										}
									} else if(b.getFigure(it->move.toY, it->move.toX) != 0) {
										if(it->move.moveType != PASSANT_MV) {
											if((b.getFigure(it->move.toY, it->move.toX) & COLOR_SAVE) == color || (b.getFigure(it->move.toY, it->move.toX) & TYPE_SAVE) == KING) {
												if((b.getFigure(it->move.toY, it->move.toX) & TYPE_SAVE) == KING && (b.getFigure(it->move.toY, it->move.toX) & COLOR_SAVE) != color) {
													shah = true;
												}

												if(it->specialNext != NULL) {
													it = it->specialNext;
													continue;
												}
											} else  {
												//result.push_back(it->move);
												result.insert(result.begin(), it->move);
												++num_attacks;

												if(it->specialNext != NULL) {
													it = it->specialNext;
													continue;
												}
											}
										}
									} else if(it->move.moveType == PASSANT_MV) {
										if(b.getFigure(it->move.toY, it->move.toX) == 0 &&
											 (b.getFigure(it->move.passant_y, it->move.passant_x) & TYPE_SAVE) == PAWN &&
											 (b.getFigure(it->move.passant_y, it->move.passant_x) & COLOR_SAVE) != color &&
											 b.passant_enable &&
											 it->move.toY == b.passant_y &&
											 it->move.toX == b.passant_x) {
											 result.push_back(it->move);
										}
									}
								}

								}
							}

							if(it->next != NULL || it->specialNext != NULL) {
								if(it->next != NULL) {
									it = it->next;
								} else {
									it = it->specialNext;
								}
							} else {
								stopped = true;
							}

					} while(!stopped);
				}
			}
		}
	}

	for(int i = 0; i < num_attacks; ++i) {
		for(int j = 0; j < num_attacks; ++j) {
			if(result[j].getAttackPrice(b) < result[j + 1].getAttackPrice(b)) {
				Move tmp = result[j];
				result[j] = result[j + 1];
				result[j + 1] = tmp;
			}
		}
	}

	bool killer_has = false;

	for(unsigned int i = 0; i < result.size(); ++i) {
		if(b.isWhiteMove()) {
			if(result[i].equal(whiteKiller[depth].move) && whiteKiller[depth].enable && whiteKiller[depth].move.quality()) {
				result[i] = result[0];
				result[0] = whiteKiller[depth].move;
				killer_has = true;
				break;
			}
		} else {
			if(result[i].equal(blackKiller[depth].move) && blackKiller[depth].enable && blackKiller[depth].move.quality()) {
				result[i] = result[0];
				result[0] = blackKiller[depth].move;
				killer_has = true;
				break;
			}
		}
	}

	if(result.size() > 1 && killer_has) {
		if(b.getFigure(result[1].toY, result[1].toX) != 0) {
			Move tmp = result[0];
			result[0] = result[1];
			result[1] = tmp;
		}
	}

	double eval = evalute(b);
	int mul = 1;
	if(!b.whiteMove) {
		mul = -1;
	}

	/*for(int i = num_attacks; i < result.size(); ++i) {
		Board tmp_brd = b;
		tmp_brd.move(result[i]);
		result[i].simple_eval = (eval - evalute(tmp_brd)) * mul;
	}

	for(int i = num_attacks + 1; i < result.size(); ++i) {
		for(int j = i; j > num_attacks && getPriceCell(b, result[j-1].fromY, result[j-1].fromX) < getPriceCell(b, result[j].fromY, result[j].fromX); --j) {
			Move tmp = result[j-1];
			result[j-1] = result[j];
			result[j] = tmp;
		}
	}

	std::sort(result.begin() + num_attacks, result.end());*/

	if(boardHash[getColorHash(b) & hash_cutter].enable && boardHash[getColorHash(b) & hash_cutter].depth >= max_depth - depth && boardHash[getColorHash(b) & hash_cutter].hash == getColorHash(b)) {
		for(unsigned int i = 0; i < result.size(); ++i) {
			if(result[i].equal(boardHash[getColorHash(b) & hash_cutter].move)) {
				Move tmp = result[i];
				result[i] = result[0];
				result[0] = tmp;
				break;
			}
		}
	}

	return result;
}

void Game::moveListGenerator() {
	/* --- PAWN | WHITE --- */
	for(int y = 1; y < BOARD_SIZE - 1; ++y) {
		for(int x = 0; x < BOARD_SIZE; ++x) {
			Move mv = Move(y, x, y - 1, x);
			MoveItem* tmp;
			bool usedTmp = false;

			if(y == 1) {
				mv.moveType = TRANSFORMATION_MV;
				mv.figureTransform = (QUEEN | WHITE);
				move_list[PAWN | WHITE][y][x].add(mv);
				mv.figureTransform = (ROOK | WHITE);
				move_list[PAWN | WHITE][y][x].add(mv);
				mv.figureTransform = (BISHOP | WHITE);
				move_list[PAWN | WHITE][y][x].add(mv);
				mv.figureTransform = (KNIGHT | WHITE);
				move_list[PAWN | WHITE][y][x].add(mv);
				bitboard[PAWN | WHITE][y][x] += cells[y - 1][x];

			} else {
				tmp = move_list[PAWN | WHITE][y][x].add(mv);
				bitboard[PAWN | WHITE][y][x] += cells[y - 1][x];

				if(y == 6) {
					mv = Move(y, x, y - 2, x);

					move_list[PAWN | WHITE][y][x].add(mv);
					bitboard[PAWN | WHITE][y][x] += cells[y - 2][x];
				}
			}

			if(x > 0) {
				mv = Move(y, x, y - 1, x - 1);
				if(y == 1) {
					mv.moveType = TRANSFORMATION_MV;
					mv.figureTransform = (QUEEN | WHITE);
					move_list[PAWN | WHITE][y][x].add(mv);
					mv.figureTransform = (ROOK | WHITE);
					move_list[PAWN | WHITE][y][x].add(mv);
					mv.figureTransform = (BISHOP | WHITE);
					move_list[PAWN | WHITE][y][x].add(mv);
					mv.figureTransform = (KNIGHT | WHITE);
					move_list[PAWN | WHITE][y][x].add(mv);
					bitboard[PAWN | WHITE][y][x] += cells[y - 1][x - 1];
				} else {
					MoveItem* pawn_atck = move_list[PAWN | WHITE][y][x].add(mv);
					bitboard[PAWN | WHITE][y][x] += cells[y - 1][x - 1];
					if(!usedTmp && y == 6) {
						//pawn_atck->move.print();
						tmp->specialNext = pawn_atck;

						usedTmp = true;
					}

					if(y == 3) {
						mv.moveType = PASSANT_MV;
						mv.passant_x = x - 1;
						mv.passant_y = y;
						move_list[PAWN | WHITE][y][x].add(mv);

					}
				}
			}

			if(x < BOARD_SIZE - 1) {
				mv = Move(y, x, y - 1, x + 1);
				if(y == 1) {
					mv.moveType = TRANSFORMATION_MV;
					mv.figureTransform = (QUEEN | WHITE);
					move_list[PAWN | WHITE][y][x].add(mv);
					mv.figureTransform = (ROOK | WHITE);
					move_list[PAWN | WHITE][y][x].add(mv);
					mv.figureTransform = (BISHOP | WHITE);
					move_list[PAWN | WHITE][y][x].add(mv);
					mv.figureTransform = (KNIGHT | WHITE);
					move_list[PAWN | WHITE][y][x].add(mv);
					bitboard[PAWN | WHITE][y][x] += cells[y - 1][x + 1];
				} else {
					MoveItem* pawn_atck = move_list[PAWN | WHITE][y][x].add(mv);
					bitboard[PAWN | WHITE][y][x] += cells[y - 1][x + 1];

					if(!usedTmp && y == 6) {
						tmp->specialNext = pawn_atck;
						usedTmp = true;
					}

					if(y == 3) {
						mv.moveType = PASSANT_MV;
						mv.passant_x = x + 1;
						mv.passant_y = y;
						move_list[PAWN | WHITE][y][x].add(mv);
					}
				}
			}
		}
	}

	/* --- PAWN | BLACK --- */
	for(int y = 1; y < BOARD_SIZE - 1; ++y) {
		for(int x = 0; x < BOARD_SIZE; ++x) {
			Move mv = Move(y, x, y + 1, x);
			MoveItem* tmp;
			bool usedTmp = false;

			if(y == 6) {
				mv.moveType = TRANSFORMATION_MV;
				mv.figureTransform = (QUEEN | BLACK);
				move_list[PAWN | BLACK][y][x].add(mv);
				mv.figureTransform = (ROOK | BLACK);
				move_list[PAWN | BLACK][y][x].add(mv);
				mv.figureTransform = (BISHOP | BLACK);
				move_list[PAWN | BLACK][y][x].add(mv);
				mv.figureTransform = (KNIGHT | BLACK);
				move_list[PAWN | BLACK][y][x].add(mv);
				bitboard[PAWN | BLACK][y][x] += cells[y + 1][x];
			} else {
				tmp = move_list[PAWN | BLACK][y][x].add(mv);
				bitboard[PAWN | BLACK][y][x] += cells[y + 1][x];

				if(y == 1) {
					mv = Move(y, x, y + 2, x);
					move_list[PAWN | BLACK][y][x].add(mv);
					bitboard[PAWN | BLACK][y][x] += cells[y + 2][x];
				}
			}

			if(x > 0) {
				mv = Move(y, x, y + 1, x - 1);
				if(y == 6) {
					mv.moveType = TRANSFORMATION_MV;
					mv.figureTransform = (QUEEN | BLACK);
					move_list[PAWN | BLACK][y][x].add(mv);
					mv.figureTransform = (ROOK | BLACK);
					move_list[PAWN | BLACK][y][x].add(mv);
					mv.figureTransform = (BISHOP | BLACK);
					move_list[PAWN | BLACK][y][x].add(mv);
					mv.figureTransform = (KNIGHT | BLACK);
					move_list[PAWN | BLACK][y][x].add(mv);
					bitboard[PAWN | BLACK][y][x] += cells[y + 1][x - 1];
				} else {
					MoveItem* pawn_atck = move_list[PAWN | BLACK][y][x].add(mv);
					bitboard[PAWN | BLACK][y][x] += cells[y + 1][x - 1];
					if(!usedTmp && y == 1) {
						tmp->specialNext = pawn_atck;
						usedTmp = true;
					}

					if(y == 4) {
						mv.moveType = PASSANT_MV;
						mv.passant_x = x - 1;
						mv.passant_y = y;
						move_list[PAWN | BLACK][y][x].add(mv);
					}
				}
			}

			if(x < BOARD_SIZE - 1) {
				mv = Move(y, x, y + 1, x + 1);
				if(y == 6) {
					mv.moveType = TRANSFORMATION_MV;
					mv.figureTransform = (QUEEN | BLACK);
					move_list[PAWN | BLACK][y][x].add(mv);
					mv.figureTransform = (ROOK | BLACK);
					move_list[PAWN | BLACK][y][x].add(mv);
					mv.figureTransform = (BISHOP | BLACK);
					move_list[PAWN | BLACK][y][x].add(mv);
					mv.figureTransform = (KNIGHT | BLACK);
					move_list[PAWN | BLACK][y][x].add(mv);
					bitboard[PAWN | BLACK][y][x] += cells[y + 1][x + 1];
				} else {
					MoveItem* pawn_atck = move_list[PAWN | BLACK][y][x].add(mv);
					bitboard[PAWN | BLACK][y][x] += cells[y + 1][x + 1];

					if(!usedTmp && y == 1) {
						tmp->specialNext = pawn_atck;
						usedTmp = true;
					}

					if(y == 4) {
						mv.moveType = PASSANT_MV;
						mv.passant_x = x + 1;
						mv.passant_y = y;
						move_list[PAWN | BLACK][y][x].add(mv);
					}
				}
			}
		}
	}

	/* --- KNIGHT --- */

	for(int y = 0; y < BOARD_SIZE; ++y) {
		for(int x = 0; x < BOARD_SIZE; ++x) {
			Move mv;

			if(y - 2 >= 0 && x - 1 >= 0) {
				mv = Move(y, x, y - 2, x - 1);
				move_list[KNIGHT | WHITE][y][x].add(mv);
				move_list[KNIGHT | BLACK][y][x].add(mv);

				bitboard [KNIGHT | WHITE][y][x] += cells[y - 2][x - 1];
				bitboard [KNIGHT | BLACK][y][x] += cells[y - 2][x - 1];
			}
			if(y - 2 >= 0 && x + 1 <= BOARD_SIZE - 1) {
				mv = Move(y, x, y - 2, x + 1);
				move_list[KNIGHT | WHITE][y][x].add(mv);
				move_list[KNIGHT | BLACK][y][x].add(mv);
				bitboard [KNIGHT | WHITE][y][x] += cells[y - 2][x + 1];
				bitboard [KNIGHT | BLACK][y][x] += cells[y - 2][x + 1];
			}

			if(y - 1 >= 0 && x - 2 >= 0) {
				mv = Move(y, x, y - 1, x - 2);
				move_list[KNIGHT | WHITE][y][x].add(mv);
				move_list[KNIGHT | BLACK][y][x].add(mv);
				bitboard [KNIGHT | WHITE][y][x] += cells[y - 1][x - 2];
				bitboard [KNIGHT | BLACK][y][x] += cells[y - 1][x - 2];
			}

			if(y - 1 >= 0 && x + 2 <= BOARD_SIZE - 1) {
				mv = Move(y, x, y - 1, x + 2);
				move_list[KNIGHT | WHITE][y][x].add(mv);
				move_list[KNIGHT | BLACK][y][x].add(mv);
				bitboard [KNIGHT | WHITE][y][x] += cells[y - 1][x + 2];
				bitboard [KNIGHT | BLACK][y][x] += cells[y - 1][x + 2];
			}

			if(y + 2 <= BOARD_SIZE - 1 && x - 1 >= 0) {
				mv = Move(y, x, y + 2, x - 1);
				move_list[KNIGHT | WHITE][y][x].add(mv);
				move_list[KNIGHT | BLACK][y][x].add(mv);
				bitboard [KNIGHT | WHITE][y][x] += cells[y + 2][x - 1];
				bitboard [KNIGHT | BLACK][y][x] += cells[y + 2][x - 1];
			}

			if(y + 2 <= BOARD_SIZE - 1 && x + 1 <= BOARD_SIZE - 1) {
				mv = Move(y, x, y + 2, x + 1);
				move_list[KNIGHT | WHITE][y][x].add(mv);
				move_list[KNIGHT | BLACK][y][x].add(mv);
				bitboard [KNIGHT | WHITE][y][x] += cells[y + 2][x + 1];
				bitboard [KNIGHT | BLACK][y][x] += cells[y + 2][x + 1];
			}

			if(y + 1 <= BOARD_SIZE - 1 && x - 2 >= 0) {
				mv = Move(y, x, y + 1, x - 2);
				move_list[KNIGHT | WHITE][y][x].add(mv);
				move_list[KNIGHT | BLACK][y][x].add(mv);
				bitboard [KNIGHT | WHITE][y][x] += cells[y + 1][x - 2];
				bitboard [KNIGHT | BLACK][y][x] += cells[y + 1][x - 2];
			}


			if(y + 1 <= BOARD_SIZE - 1 && x + 2 <= BOARD_SIZE - 1) {
				mv = Move(y, x, y + 1, x + 2);
				move_list[KNIGHT | WHITE][y][x].add(mv);
				move_list[KNIGHT | BLACK][y][x].add(mv);
				bitboard [KNIGHT | WHITE][y][x] += cells[y + 1][x + 2];
				bitboard [KNIGHT | BLACK][y][x] += cells[y + 1][x + 2];
			}
		}
	}

	/* --- BISHOP --- */
	for(int y = 0; y < BOARD_SIZE; ++y) {
		for(int x = 0; x < BOARD_SIZE; ++x) {
			std::vector<MoveItem*> specialItems_w(4, NULL);
			std::vector<MoveItem*> specialItems_b(4, NULL);

			if(y < BOARD_SIZE - 1 && x < BOARD_SIZE - 1) {
				for(int xPos = x + 1, yPos = y + 1, i = 0; yPos < BOARD_SIZE && xPos < BOARD_SIZE; ++xPos, ++yPos, ++i) {
					Move mv = Move(y, x, yPos, xPos);

					if(i == 0) {
						specialItems_w[0] = move_list[BISHOP | WHITE][y][x].add(mv);
						specialItems_b[0] = move_list[BISHOP | BLACK][y][x].add(mv);
					} else {
						move_list[BISHOP | WHITE][y][x].add(mv);
						move_list[BISHOP | BLACK][y][x].add(mv);
					}

					bitboard [BISHOP | WHITE][y][x] += cells[yPos][xPos];
					bitboard [BISHOP | BLACK][y][x] += cells[yPos][xPos];
				}
			}

			if(y > 0 && x < BOARD_SIZE - 1) {
				for(int xPos = x + 1, yPos = y - 1, i = 0; yPos >= 0 && xPos < BOARD_SIZE; ++xPos, --yPos, ++i) {
					Move mv = Move(y, x, yPos, xPos);

					if(i == 0) {
						specialItems_w[1] = move_list[BISHOP | WHITE][y][x].add(mv);
						specialItems_b[1] = move_list[BISHOP | BLACK][y][x].add(mv);
					} else {
						move_list[BISHOP | WHITE][y][x].add(mv);
						move_list[BISHOP | BLACK][y][x].add(mv);
					}

					bitboard [BISHOP | WHITE][y][x] += cells[yPos][xPos];
					bitboard [BISHOP | BLACK][y][x] += cells[yPos][xPos];
				}
			}

			if(y < BOARD_SIZE - 1 && x > 0) {
				for(int xPos = x - 1, yPos = y + 1, i = 0; yPos < BOARD_SIZE && xPos >= 0; --xPos, ++yPos, ++i) {
					Move mv = Move(y, x, yPos, xPos);

					if(i == 0) {
						specialItems_w[2] = move_list[BISHOP | WHITE][y][x].add(mv);
						specialItems_b[2] = move_list[BISHOP | BLACK][y][x].add(mv);
					} else {
						move_list[BISHOP | WHITE][y][x].add(mv);
						move_list[BISHOP | BLACK][y][x].add(mv);
					}

					bitboard [BISHOP | WHITE][y][x] += cells[yPos][xPos];
					bitboard [BISHOP | BLACK][y][x] += cells[yPos][xPos];
				}
			}

			if(y > 0 && x > 0) {
				for(int xPos = x - 1, yPos = y - 1, i = 0; yPos >= 0 && xPos >= 0; --xPos, --yPos, ++i) {
					Move mv = Move(y, x, yPos, xPos);

					if(i == 0) {
						specialItems_w[3] = move_list[BISHOP | WHITE][y][x].add(mv);
						specialItems_b[3] = move_list[BISHOP | BLACK][y][x].add(mv);
					} else {
						move_list[BISHOP | WHITE][y][x].add(mv);
						move_list[BISHOP | BLACK][y][x].add(mv);
					}

					bitboard [BISHOP | WHITE][y][x] += cells[yPos][xPos];
					bitboard [BISHOP | BLACK][y][x] += cells[yPos][xPos];
				}
			}

			MoveItem* it_w = move_list[BISHOP | WHITE][y][x].first;
			MoveItem* it_b = move_list[BISHOP | BLACK][y][x].first;

			if(y < BOARD_SIZE - 1 && x < BOARD_SIZE - 1) {
				for(int xPos = x + 1, yPos = y + 1; yPos < BOARD_SIZE && xPos < BOARD_SIZE; ++xPos, ++yPos) {
					if(specialItems_w[1] != NULL) {
						it_w->specialNext = specialItems_w[1];
						it_b->specialNext = specialItems_b[1];
					} else if(specialItems_w[2] != NULL) {
						it_w->specialNext = specialItems_w[2];
						it_b->specialNext = specialItems_b[2];
					} else if(specialItems_w[3] != NULL) {
						it_w->specialNext = specialItems_w[3];
						it_b->specialNext = specialItems_b[3];
					} else {
						while(it_w->next != NULL) {
							it_w->specialNext = NULL;
							it_b->specialNext = NULL;

							it_w = it_w->next;
							it_b = it_b->next;
						}
						break;
					}

					it_w = it_w->next;
					it_b = it_b->next;
				}
			}

			if(y > 0 && x < BOARD_SIZE - 1) {
				for(int xPos = x + 1, yPos = y - 1; yPos >= 0 && xPos < BOARD_SIZE; ++xPos, --yPos) {
					if(specialItems_w[2] != NULL) {
						it_w->specialNext = specialItems_w[2];
						it_b->specialNext = specialItems_b[2];
					} else if(specialItems_w[3] != NULL) {
						it_w->specialNext = specialItems_w[3];
						it_b->specialNext = specialItems_b[3];
					} else {
						while(it_w->next != NULL) {
							it_w->specialNext = NULL;
							it_b->specialNext = NULL;

							it_w = it_w->next;
							it_b = it_b->next;
						}
						break;
					}

					it_w = it_w->next;
					it_b = it_b->next;
				}
			}

			if(y < BOARD_SIZE - 1 && x > 0) {
				for(int xPos = x - 1, yPos = y + 1; yPos < BOARD_SIZE && xPos >= 0; --xPos, ++yPos) {
					if(specialItems_w[3] != NULL) {
						it_w->specialNext = specialItems_w[3];
						it_b->specialNext = specialItems_b[3];
					} else {
						while(it_w->next != NULL) {
							it_w->specialNext = NULL;
							it_b->specialNext = NULL;

							it_w = it_w->next;
							it_b = it_b->next;
						}
						break;
					}

					it_w = it_w->next;
					it_b = it_b->next;
				}
			}

			while(it_w->next != NULL) {
				it_w->specialNext = NULL;
				it_b->specialNext = NULL;

				it_w = it_w->next;
				it_b = it_b->next;
			}
		}
	}

	/* --- ROOK --- */
	for(int y = 0; y < BOARD_SIZE; ++y) {
		for(int x = 0; x < BOARD_SIZE; ++x) {
			std::vector<MoveItem*> specialItems_w(4, NULL);
			std::vector<MoveItem*> specialItems_b(4, NULL);

			if(y < BOARD_SIZE - 1) {
				for(int yPos = y + 1, i = 0; yPos < BOARD_SIZE; ++yPos, ++i) {
					Move mv = Move(y, x, yPos, x);

					if(i == 0) {
						specialItems_w[0] = move_list[ROOK | WHITE][y][x].add(mv);
						specialItems_b[0] = move_list[ROOK | BLACK][y][x].add(mv);
					} else {
						move_list[ROOK | WHITE][y][x].add(mv);
						move_list[ROOK | BLACK][y][x].add(mv);
					}

					bitboard [ROOK | WHITE][y][x] += cells[yPos][x];
					bitboard [ROOK | BLACK][y][x] += cells[yPos][x];
				}
			}

			if(y > 0) {
				for(int yPos = y - 1, i = 0; yPos >= 0; --yPos, ++i) {
					Move mv = Move(y, x, yPos, x);

					if(i == 0) {
						specialItems_w[1] = move_list[ROOK | WHITE][y][x].add(mv);
						specialItems_b[1] = move_list[ROOK | BLACK][y][x].add(mv);
					} else {
						move_list[ROOK | WHITE][y][x].add(mv);
						move_list[ROOK | BLACK][y][x].add(mv);
					}

					bitboard [ROOK | WHITE][y][x] += cells[yPos][x];
					bitboard [ROOK | BLACK][y][x] += cells[yPos][x];
				}
			}

			if(x < BOARD_SIZE - 1) {
				for(int xPos = x + 1, i = 0; xPos < BOARD_SIZE; ++xPos, ++i) {
					Move mv = Move(y, x, y, xPos);

					if(i == 0) {
						specialItems_w[2] = move_list[ROOK | WHITE][y][x].add(mv);
						specialItems_b[2] = move_list[ROOK | BLACK][y][x].add(mv);
					} else {
						move_list[ROOK | WHITE][y][x].add(mv);
						move_list[ROOK | BLACK][y][x].add(mv);
					}

					bitboard [ROOK | WHITE][y][x] += cells[y][xPos];
					bitboard [ROOK | BLACK][y][x] += cells[y][xPos];
				}
			}

			if(x > 0) {
				for(int xPos = x - 1, i = 0; xPos >= 0; --xPos, ++i) {
					Move mv = Move(y, x, y, xPos);

					if(i == 0) {
						specialItems_w[3] = move_list[ROOK | WHITE][y][x].add(mv);
						specialItems_b[3] = move_list[ROOK | BLACK][y][x].add(mv);
					} else {
						move_list[ROOK | WHITE][y][x].add(mv);
						move_list[ROOK | BLACK][y][x].add(mv);
					}

					bitboard [ROOK | WHITE][y][x] += cells[y][xPos];
					bitboard [ROOK | BLACK][y][x] += cells[y][xPos];
				}
			}

			MoveItem* it_w = move_list[ROOK | WHITE][y][x].first;
			MoveItem* it_b = move_list[ROOK | BLACK][y][x].first;

			if(y < BOARD_SIZE - 1) {
				for(int yPos = y + 1; yPos < BOARD_SIZE; ++yPos) {
					if(specialItems_w[1] != NULL) {
						it_w->specialNext = specialItems_w[1];
						it_b->specialNext = specialItems_b[1];
					} else if(specialItems_w[2] != NULL) {
						it_w->specialNext = specialItems_w[2];
						it_b->specialNext = specialItems_b[2];
					} else if(specialItems_w[3] != NULL) {
						it_w->specialNext = specialItems_w[3];
						it_b->specialNext = specialItems_b[3];
					} else {
						while(it_w->next != NULL) {
							it_w->specialNext = NULL;
							it_b->specialNext = NULL;

							it_w = it_w->next;
							it_b = it_b->next;
						}
						break;
					}

					it_w = it_w->next;
					it_b = it_b->next;
				}
			}

			if(y > 0) {
				for(int yPos = y - 1; yPos >= 0; --yPos) {
					if(specialItems_w[2] != NULL) {
						it_w->specialNext = specialItems_w[2];
						it_b->specialNext = specialItems_b[2];
					} else if(specialItems_w[3] != NULL) {
						it_w->specialNext = specialItems_w[3];
						it_b->specialNext = specialItems_b[3];
					} else {
						while(it_w->next != NULL) {
							it_w->specialNext = NULL;
							it_b->specialNext = NULL;

							it_w = it_w->next;
							it_b = it_b->next;
						}
						break;
					}

					it_w = it_w->next;
					it_b = it_b->next;
				}
			}

			if(x < BOARD_SIZE - 1) {
				for(int xPos = x + 1; xPos < BOARD_SIZE; ++xPos) {
					if(specialItems_w[3] != NULL) {
						it_w->specialNext = specialItems_w[3];
						it_b->specialNext = specialItems_b[3];
					} else {
						while(it_w->next != NULL) {
							it_w->specialNext = NULL;
							it_b->specialNext = NULL;

							it_w = it_w->next;
							it_b = it_b->next;
						}
						break;
					}

					it_w = it_w->next;
					it_b = it_b->next;
				}
			}

			while(it_w->next != NULL) {
				it_w->specialNext = NULL;
				it_b->specialNext = NULL;

				it_w = it_w->next;
				it_b = it_b->next;
			}
		}
	}

	/* --- QUEEN --- */
	for(int y = 0; y < BOARD_SIZE; ++y) {
		for(int x = 0; x < BOARD_SIZE; ++x) {
			std::vector<MoveItem*> specialItems_w(8, NULL);
			std::vector<MoveItem*> specialItems_b(8, NULL);

			if(y < BOARD_SIZE - 1 && x < BOARD_SIZE - 1) {
				for(int xPos = x + 1, yPos = y + 1, i = 0; yPos < BOARD_SIZE && xPos < BOARD_SIZE; ++xPos, ++yPos, ++i) {
					Move mv = Move(y, x, yPos, xPos);

					if(i == 0) {
						specialItems_w[0] = move_list[QUEEN | WHITE][y][x].add(mv);
						specialItems_b[0] = move_list[QUEEN | BLACK][y][x].add(mv);
					} else {
						move_list[QUEEN | WHITE][y][x].add(mv);
						move_list[QUEEN | BLACK][y][x].add(mv);
					}

					bitboard [QUEEN | WHITE][y][x] += cells[yPos][xPos];
					bitboard [QUEEN | BLACK][y][x] += cells[yPos][xPos];
				}
			}

			if(y > 0 && x < BOARD_SIZE - 1) {
				for(int xPos = x + 1, yPos = y - 1, i = 0; yPos >= 0 && xPos < BOARD_SIZE; ++xPos, --yPos, ++i) {
					Move mv = Move(y, x, yPos, xPos);

					if(i == 0) {
						specialItems_w[1] = move_list[QUEEN | WHITE][y][x].add(mv);
						specialItems_b[1] = move_list[QUEEN | BLACK][y][x].add(mv);
					} else {
						move_list[QUEEN | WHITE][y][x].add(mv);
						move_list[QUEEN | BLACK][y][x].add(mv);
					}

					bitboard [QUEEN | WHITE][y][x] += cells[yPos][xPos];
					bitboard [QUEEN | BLACK][y][x] += cells[yPos][xPos];
				}
			}

			if(y < BOARD_SIZE - 1 && x > 0) {
				for(int xPos = x - 1, yPos = y + 1, i = 0; yPos < BOARD_SIZE && xPos >= 0; --xPos, ++yPos, ++i) {
					Move mv = Move(y, x, yPos, xPos);

					if(i == 0) {
						specialItems_w[2] = move_list[QUEEN | WHITE][y][x].add(mv);
						specialItems_b[2] = move_list[QUEEN | BLACK][y][x].add(mv);
					} else {
						move_list[QUEEN | WHITE][y][x].add(mv);
						move_list[QUEEN | BLACK][y][x].add(mv);
					}

					bitboard [QUEEN | WHITE][y][x] += cells[yPos][xPos];
					bitboard [QUEEN | BLACK][y][x] += cells[yPos][xPos];
				}
			}

			if(y > 0 && x > 0) {
				for(int xPos = x - 1, yPos = y - 1, i = 0; yPos >= 0 && xPos >= 0; --xPos, --yPos, ++i) {
					Move mv = Move(y, x, yPos, xPos);

					if(i == 0) {
						specialItems_w[3] = move_list[QUEEN | WHITE][y][x].add(mv);
						specialItems_b[3] = move_list[QUEEN | BLACK][y][x].add(mv);
					} else {
						move_list[QUEEN | WHITE][y][x].add(mv);
						move_list[QUEEN | BLACK][y][x].add(mv);
					}

					bitboard [QUEEN | WHITE][y][x] += cells[yPos][xPos];
					bitboard [QUEEN | BLACK][y][x] += cells[yPos][xPos];
				}
			}

			if(y < BOARD_SIZE - 1) {
				for(int yPos = y + 1, i = 0; yPos < BOARD_SIZE; ++yPos, ++i) {
					Move mv = Move(y, x, yPos, x);

					if(i == 0) {
						specialItems_w[4] = move_list[QUEEN | WHITE][y][x].add(mv);
						specialItems_b[4] = move_list[QUEEN | BLACK][y][x].add(mv);
					} else {
						move_list[QUEEN | WHITE][y][x].add(mv);
						move_list[QUEEN | BLACK][y][x].add(mv);
					}

					bitboard [QUEEN | WHITE][y][x] += cells[yPos][x];
					bitboard [QUEEN | BLACK][y][x] += cells[yPos][x];
				}
			}

			if(y > 0) {
				for(int yPos = y - 1, i = 0; yPos >= 0; --yPos, ++i) {
					Move mv = Move(y, x, yPos, x);

					if(i == 0) {
						specialItems_w[5] = move_list[QUEEN | WHITE][y][x].add(mv);
						specialItems_b[5] = move_list[QUEEN | BLACK][y][x].add(mv);
					} else {
						move_list[QUEEN | WHITE][y][x].add(mv);
						move_list[QUEEN | BLACK][y][x].add(mv);
					}

					bitboard [QUEEN | WHITE][y][x] += cells[yPos][x];
					bitboard [QUEEN | BLACK][y][x] += cells[yPos][x];
				}
			}

			if(x < BOARD_SIZE - 1) {
				for(int xPos = x + 1, i = 0; xPos < BOARD_SIZE; ++xPos, ++i) {
					Move mv = Move(y, x, y, xPos);

					if(i == 0) {
						specialItems_w[6] = move_list[QUEEN | WHITE][y][x].add(mv);
						specialItems_b[6] = move_list[QUEEN | BLACK][y][x].add(mv);
					} else {
						move_list[QUEEN | WHITE][y][x].add(mv);
						move_list[QUEEN | BLACK][y][x].add(mv);
					}

					bitboard [QUEEN | WHITE][y][x] += cells[y][xPos];
					bitboard [QUEEN | BLACK][y][x] += cells[y][xPos];
				}
			}

			if(x > 0) {
				for(int xPos = x - 1, i = 0; xPos >= 0; --xPos, ++i) {
					Move mv = Move(y, x, y, xPos);

					if(i == 0) {
						specialItems_w[7] = move_list[QUEEN | WHITE][y][x].add(mv);
						specialItems_b[7] = move_list[QUEEN | BLACK][y][x].add(mv);
					} else {
						move_list[QUEEN | WHITE][y][x].add(mv);
						move_list[QUEEN | BLACK][y][x].add(mv);
					}

					bitboard [QUEEN | WHITE][y][x] += cells[y][xPos];
					bitboard [QUEEN | BLACK][y][x] += cells[y][xPos];
				}
			}

			MoveItem* it_w = move_list[QUEEN | WHITE][y][x].first;
			MoveItem* it_b = move_list[QUEEN | BLACK][y][x].first;

			if(y < BOARD_SIZE - 1 && x < BOARD_SIZE - 1) {
				for(int xPos = x + 1, yPos = y + 1; yPos < BOARD_SIZE && xPos < BOARD_SIZE; ++xPos, ++yPos) {
					if(specialItems_w[1] != NULL) {
						it_w->specialNext = specialItems_w[1];
						it_b->specialNext = specialItems_b[1];
					} else if(specialItems_w[2] != NULL) {
						it_w->specialNext = specialItems_w[2];
						it_b->specialNext = specialItems_b[2];
					} else if(specialItems_w[3] != NULL) {
						it_w->specialNext = specialItems_w[3];
						it_b->specialNext = specialItems_b[3];
					} else if(specialItems_w[4] != NULL) {
						it_w->specialNext = specialItems_w[4];
						it_b->specialNext = specialItems_b[4];
					} else if(specialItems_w[5] != NULL) {
						it_w->specialNext = specialItems_w[5];
						it_b->specialNext = specialItems_b[5];
					} else if(specialItems_w[6] != NULL) {
						it_w->specialNext = specialItems_w[6];
						it_b->specialNext = specialItems_b[6];
					} else if(specialItems_w[7] != NULL) {
						it_w->specialNext = specialItems_w[7];
						it_b->specialNext = specialItems_b[7];
					} else {
						while(it_w->next != NULL) {
							it_w->specialNext = NULL;
							it_b->specialNext = NULL;

							it_w = it_w->next;
							it_b = it_b->next;
						}
						break;
					}

					it_w = it_w->next;
					it_b = it_b->next;
				}
			}

			if(y > 0 && x < BOARD_SIZE - 1) {
				for(int xPos = x + 1, yPos = y - 1; yPos >= 0 && xPos < BOARD_SIZE; ++xPos, --yPos) {
					if(specialItems_w[2] != NULL) {
						it_w->specialNext = specialItems_w[2];
						it_b->specialNext = specialItems_b[2];
					} else if(specialItems_w[3] != NULL) {
						it_w->specialNext = specialItems_w[3];
						it_b->specialNext = specialItems_b[3];
					} else if(specialItems_w[4] != NULL) {
						it_w->specialNext = specialItems_w[4];
						it_b->specialNext = specialItems_b[4];
					} else if(specialItems_w[5] != NULL) {
						it_w->specialNext = specialItems_w[5];
						it_b->specialNext = specialItems_b[5];
					} else if(specialItems_w[6] != NULL) {
						it_w->specialNext = specialItems_w[6];
						it_b->specialNext = specialItems_b[6];
					} else if(specialItems_w[7] != NULL) {
						it_w->specialNext = specialItems_w[7];
						it_b->specialNext = specialItems_b[7];
					} else {
						while(it_w->next != NULL) {
							it_w->specialNext = NULL;
							it_b->specialNext = NULL;

							it_w = it_w->next;
							it_b = it_b->next;
						}
						break;
					}

					it_w = it_w->next;
					it_b = it_b->next;
				}
			}

			if(y < BOARD_SIZE - 1 && x > 0) {
				for(int xPos = x - 1, yPos = y + 1; yPos < BOARD_SIZE && xPos >= 0; --xPos, ++yPos) {
					if(specialItems_w[3] != NULL) {
						it_w->specialNext = specialItems_w[3];
						it_b->specialNext = specialItems_b[3];
					} else if(specialItems_w[4] != NULL) {
						it_w->specialNext = specialItems_w[4];
						it_b->specialNext = specialItems_b[4];
					} else if(specialItems_w[5] != NULL) {
						it_w->specialNext = specialItems_w[5];
						it_b->specialNext = specialItems_b[5];
					} else if(specialItems_w[6] != NULL) {
						it_w->specialNext = specialItems_w[6];
						it_b->specialNext = specialItems_b[6];
					} else if(specialItems_w[7] != NULL) {
						it_w->specialNext = specialItems_w[7];
						it_b->specialNext = specialItems_b[7];
					} else {
						while(it_w->next != NULL) {
							it_w->specialNext = NULL;
							it_b->specialNext = NULL;

							it_w = it_w->next;
							it_b = it_b->next;
						}
						break;
					}

					it_w = it_w->next;
					it_b = it_b->next;
				}
			}

			if(y > 0 && x > 0) {
				for(int xPos = x - 1, yPos = y - 1; yPos >= 0 && xPos >= 0; --xPos, --yPos) {
					if(specialItems_w[4] != NULL) {
						it_w->specialNext = specialItems_w[4];
						it_b->specialNext = specialItems_b[4];
					} else if(specialItems_w[5] != NULL) {
						it_w->specialNext = specialItems_w[5];
						it_b->specialNext = specialItems_b[5];
					} else if(specialItems_w[6] != NULL) {
						it_w->specialNext = specialItems_w[6];
						it_b->specialNext = specialItems_b[6];
					} else if(specialItems_w[7] != NULL) {
						it_w->specialNext = specialItems_w[7];
						it_b->specialNext = specialItems_b[7];
					} else {
						while(it_w->next != NULL) {
							it_w->specialNext = NULL;
							it_b->specialNext = NULL;

							it_w = it_w->next;
							it_b = it_b->next;
						}
						break;
					}

					it_w = it_w->next;
					it_b = it_b->next;
				}
			}

			if(y < BOARD_SIZE - 1) {
				for(int yPos = y + 1; yPos < BOARD_SIZE; ++yPos) {
					if(specialItems_w[5] != NULL) {
						it_w->specialNext = specialItems_w[5];
						it_b->specialNext = specialItems_b[5];
					} else if(specialItems_w[6] != NULL) {
						it_w->specialNext = specialItems_w[6];
						it_b->specialNext = specialItems_b[6];
					} else if(specialItems_w[7] != NULL) {
						it_w->specialNext = specialItems_w[7];
						it_b->specialNext = specialItems_b[7];
					} else {
						while(it_w->next != NULL) {
							it_w->specialNext = NULL;
							it_b->specialNext = NULL;

							it_w = it_w->next;
							it_b = it_b->next;
						}
						break;
					}

					it_w = it_w->next;
					it_b = it_b->next;
				}
			}

			if(y > 0) {
				for(int yPos = y - 1; yPos >= 0; --yPos) {
					if(specialItems_w[6] != NULL) {
						it_w->specialNext = specialItems_w[6];
						it_b->specialNext = specialItems_b[6];
					} else if(specialItems_w[7] != NULL) {
						it_w->specialNext = specialItems_w[7];
						it_b->specialNext = specialItems_b[7];
					} else {
						while(it_w->next != NULL) {
							it_w->specialNext = NULL;
							it_b->specialNext = NULL;

							it_w = it_w->next;
							it_b = it_b->next;
						}
						break;
					}

					it_w = it_w->next;
					it_b = it_b->next;
				}
			}

			if(x < BOARD_SIZE - 1) {
				for(int xPos = x + 1; xPos < BOARD_SIZE; ++xPos) {
					if(specialItems_w[7] != NULL) {
						it_w->specialNext = specialItems_w[7];
						it_b->specialNext = specialItems_b[7];
					} else {
						while(it_w->next != NULL) {
							it_w->specialNext = NULL;
							it_b->specialNext = NULL;

							it_w = it_w->next;
							it_b = it_b->next;
						}
						break;
					}

					it_w = it_w->next;
					it_b = it_b->next;
				}
			}

			while(it_w->next != NULL) {
				it_w->specialNext = NULL;
				it_b->specialNext = NULL;

				it_w = it_w->next;
				it_b = it_b->next;
			}

		}
	}

	/* --- KING --- */

	for(int y = 0; y < BOARD_SIZE; ++y) {
		for(int x = 0; x < BOARD_SIZE; ++x) {
			if(y - 1 >= 0 && x - 1 >= 0) {
				Move mv(y, x, y - 1, x - 1);
				move_list[KING | WHITE][y][x].add(mv);
				move_list[KING | BLACK][y][x].add(mv);
				bitboard [KING | WHITE][y][x] += cells[y - 1][x - 1];
				bitboard [KING | BLACK][y][x] += cells[y - 1][x - 1];
			}

			if(y - 1 >= 0) {
				Move mv(y, x, y - 1, x);
				move_list[KING | WHITE][y][x].add(mv);
				move_list[KING | BLACK][y][x].add(mv);
				bitboard [KING | WHITE][y][x] += cells[y - 1][x];
				bitboard [KING | BLACK][y][x] += cells[y - 1][x];
			}

			if(y - 1 >= 0 && x + 1 < BOARD_SIZE) {
				Move mv(y, x, y - 1, x + 1);
				move_list[KING | WHITE][y][x].add(mv);
				move_list[KING | BLACK][y][x].add(mv);
				bitboard [KING | WHITE][y][x] += cells[y - 1][x + 1];
				bitboard [KING | BLACK][y][x] += cells[y - 1][x + 1];
			}

			if(x - 1 >= 0) {
				Move mv(y, x, y, x - 1);
				move_list[KING | WHITE][y][x].add(mv);
				move_list[KING | BLACK][y][x].add(mv);
				bitboard [KING | WHITE][y][x] += cells[y][x - 1];
				bitboard [KING | BLACK][y][x] += cells[y][x - 1];
			}

			if(x + 1 < BOARD_SIZE) {
				Move mv(y, x, y, x + 1);
				move_list[KING | WHITE][y][x].add(mv);
				move_list[KING | BLACK][y][x].add(mv);
				bitboard [KING | WHITE][y][x] += cells[y][x + 1];
				bitboard [KING | BLACK][y][x] += cells[y][x + 1];
			}

			if(y + 1 < BOARD_SIZE && x - 1 >= 0) {
				Move mv(y, x, y + 1, x - 1);
				move_list[KING | WHITE][y][x].add(mv);
				move_list[KING | BLACK][y][x].add(mv);
				bitboard [KING | WHITE][y][x] += cells[y + 1][x - 1];
				bitboard [KING | BLACK][y][x] += cells[y + 1][x - 1];
			}

			if(y + 1 < BOARD_SIZE) {
				Move mv(y, x, y + 1, x);
				move_list[KING | WHITE][y][x].add(mv);
				move_list[KING | BLACK][y][x].add(mv);
				bitboard [KING | WHITE][y][x] += cells[y + 1][x];
				bitboard [KING | BLACK][y][x] += cells[y + 1][x];
			}

			if(y + 1 < BOARD_SIZE && x + 1 < BOARD_SIZE) {
				Move mv(y, x, y + 1, x + 1);
				move_list[KING | WHITE][y][x].add(mv);
				move_list[KING | BLACK][y][x].add(mv);
				bitboard [KING | WHITE][y][x] += cells[y + 1][x + 1];
				bitboard [KING | BLACK][y][x] += cells[y + 1][x + 1];
			}

			if(y == 7 && x == 4) {
				Move mv(y, x, y, x + 2);
				mv.moveType = WS_CASTLING_MV;
				move_list[KING | WHITE][y][x].add(mv);

				mv = Move(y, x, y, x - 2);
				mv.moveType = WL_CASTLING_MV;
				move_list[KING | WHITE][y][x].add(mv);
			}

			if(y == 0 && x == 4) {
				Move mv(y, x, y, x + 2);
				mv.moveType = BS_CASTLING_MV;
				move_list[KING | BLACK][y][x].add(mv);

				mv = Move(y, x, y, x - 2);
				mv.moveType = BL_CASTLING_MV;
				move_list[KING | BLACK][y][x].add(mv);
			}
		}
	}
}

void Game::printBitBoard(uint64_t bit_board) {
	for(int y = 0; y < BOARD_SIZE; ++y) {
		for(int x = 0; x < BOARD_SIZE; ++x) {
			if(bit_board & cells[y][x]) {
				std::cout << 1;
			} else {
				std::cout << 0;
			}
		}
		std::cout << "\n";
	}
}

void Game::cache_gen() {
	uint64_t mul = 1;
	for(unsigned int y = 0; y < BOARD_SIZE; ++y) {
		for(unsigned int x = 0; x < BOARD_SIZE; ++x) {
			cells[y][x] = mul;
			mul *= 2;
		}
	}

	for(int i = 0; i < 32; ++i) {
		for(int j = 0; j < BOARD_SIZE; ++j) {
			for(int k = 0; k < BOARD_SIZE; ++k) {
				bitboard[i][j][k] = 0;
			}
		}
	}

	moveListGenerator();
}

bool Game::shahIsNot(Board & b, uint8_t color) {
	uint8_t enemyColor;
	if(color == WHITE) {
		enemyColor = BLACK;
	} else {
		enemyColor = WHITE;
	}

	uint64_t mask = 0;

	uint64_t kingPos;

	for(unsigned int y = 0; y < BOARD_SIZE; ++y) {
		for(unsigned int x = 0; x < BOARD_SIZE; ++x) {
			if(b.getFigure(y, x) != 0 && (b.getFigure(y, x) & COLOR_SAVE) == enemyColor) {
				mask = (mask | bitboard[b.getFigure(y, x)][y][x]);
			}

			if(b.getFigure(y, x) == (KING | color)) {
				kingPos = cells[y][x];
			}
		}
	}

	return !(mask & kingPos);
}

/*uint64_t Game::getHash(Board & b) {
	uint64_t res = 0;

	for(int y = 0; y < BOARD_SIZE; ++y) {
		for(int x = 0; x < BOARD_SIZE; ++x) {
			res += (cells[y][x] * 8967) * (b.getFigure(y, x) + 8967);
		}
	}

	return res;
}*/

uint64_t Game::getColorHash(Board & b) {
	uint64_t res = getHash(b);

	if(!b.isWhiteMove()) {
		res = (res * res + 1);
	}

	return res;
}

uint64_t Game::getHash(Board & b) {
	uint64_t hash = 0;

	for(unsigned int y = 0; y < 8; ++y) {
		for(unsigned int x = 0; x < 8; ++x) {
			/*int k = 0;

			if(b.getFigure(y, x) == 0) { k = 1; }
			else if(b.getFigure(y, x) & TYPE_SAVE == PAWN) { k = 2; }
			else if(b.getFigure(y, x) & TYPE_SAVE == KNIGHT) { k = 3; }
			else if(b.getFigure(y, x) & TYPE_SAVE == BISHOP) { k = 4; }
			else if(b.getFigure(y, x) & TYPE_SAVE == ROOK) { k = 5; }
			else if(b.getFigure(y, x) & TYPE_SAVE == QUEEN) { k = 6; }
			else if(b.getFigure(y, x) & TYPE_SAVE == KING) { k = 7; }

			if(b.getFigure(y, x) & COLOR_SAVE == BLACK) {
				k += 6;
			}
			uint64_t res = 1;
			for(unsigned int i = 0; i < (y+1) * 8 + (x+1); ++i) {
				res = res + k*(100000000*((y+1)+((x+1)^17))*k*(12^k));
				hash += res;
			}

			hash += res;*/
			if(b.getFigure(y, x) != 0) {
				hash = (hash ^ zobrist[b.getFigure(y, x)][y][x]);
			}
		}
	}

	return hash;
}

void Game::zobristGenerate() {
	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_int_distribution<unsigned long long> dis;

	for(int i = 0; i < 32; ++i) {
		for(int j = 0; j < BOARD_SIZE; ++j) {
			for(int k = 0; k < BOARD_SIZE; ++k) {
				zobrist[i][j][k] = dis(gen);
			}
		}
	}
}

void Game::printVariant() {
	if(print_variant_enable) {
		for(int i = 0; i < variant.size(); ++i) {
			std::cout << variant[i] << " ";
		}

		std::cout << "\n";
	}
}

double Game::getPriceCell(Board & b, int y, int x) {
	if((b.getFigure(y, x) & TYPE_SAVE) == KING) {
		return 0;
	} else if((b.getFigure(y, x) & TYPE_SAVE) == QUEEN) {
		return QUEEN_EV;
	} else if((b.getFigure(y, x) & TYPE_SAVE) == ROOK) {
		return ROOK_EV;
	} else if((b.getFigure(y, x) & TYPE_SAVE) == BISHOP) {
		return BISHOP_EV;
	} else if((b.getFigure(y, x) & TYPE_SAVE) == KNIGHT) {
		return KNIGHT_EV;
	} else if((b.getFigure(y, x) & TYPE_SAVE) == PAWN) {
		return PAWN_EV;
	} else {
		return 0;
	}
}
