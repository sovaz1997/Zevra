#include "game.hpp"

bool Game::uciHandler(std::string str) {
	std::vector<std::string> cmd = getStringArray(str);
		if(cmd[0] == "isready") {
			std::cout << "readyok\n";
		} else if(cmd[0] == "position") {
			gameHash.clear();
			gameHash.resize(0);
			if(cmd[1] == "startpos") {
				game_board.setFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
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

				game_board.setFen(fen);
				if(cmd.size() > pos) {
					if(cmd[pos] == "moves") {
						for(unsigned int i = pos + 1; i < cmd.size(); ++i) {
							move(cmd[i]);
						}
					}
				}
			}

			//gameHash.push_back(getHash(game_board));
		} else if(cmd[0] == "go") {
			if(cmd[1] == "depth") {
				max_depth = std::stoi(cmd[2]);
			  goFixedDepth();
			} else if(cmd[1] == "movetime") {
				goFixedTime(std::stoi(cmd[2]));
			} else {
				wtime = 0, btime = 0;
				winc = 0, binc = 0, movestogo = 0, movestogoEnable = false;
				for(int i = 1; i < cmd.size(); ++i) {
					if(cmd[i] == "wtime") {
						wtime = std::stoi(cmd[i+1]);
					} else if(cmd[i] == "btime") {
						btime = std::stoi(cmd[i+1]);
					} else if(cmd[i] == "winc") {
						winc = std::stoi(cmd[i+1]);
					} else if(cmd[i] == "binc") {
						binc = std::stoi(cmd[i+1]);
					} else if(cmd[i] == "movestogo") {
						movestogoEnable = true;
						movestogo = std::stoi(cmd[i+1]);
					}
				}

				goTournament();
			}
		} else if(cmd[0] == "posmoves") {
			MoveArray moves;
			game_board.bitBoardMoveGenerator(moves);//generatePositionMoves(game_board, shah_tmp, true, 0);

			for(unsigned int i = 0; i < moves.count; ++i) {
				std::cout << moves.moveArray[i].getMoveString();
				std::cout << "\n";
			}

			game_board.evaluteAll();
			std::cout << game_board.evalute / PAWN_EV * 100 << "\n";
			std::cout << game_board.getFen() << "\n";

			std::cout << "inCheck (WHITE) : " << game_board.inCheck(WHITE) << "\n";
			std::cout << "inCheck (BLACK) : " << game_board.inCheck(BLACK) << "\n";
			std::cout << "color_hash: " << game_board.getColorHash() << "\n";
		} else if(cmd[0] == "move") {
			move(cmd[1]);
		} else if(cmd[0] == "quit") {
			return false;
		} else if(cmd[0] == "uci") {
			std::cout << "id name Zevra\n";
			std::cout << "id author sovaz1997\n";
			std::cout << "uciok\n";
		} else if(cmd[0] == "quit") {
			return false;
		} else if(cmd[0] == "stress") {
			game_board.stress = 0;
			double st = clock();
			for(unsigned int i = 0; i < 1000000; ++i) {
				game_board.bitBoardMoveGenerator(moveArray[0]);
			}
			std::cout << (int)(game_board.stress / ((clock() - st) / CLOCKS_PER_SEC)) << "\n";
		} else if(cmd[0] == "goback") {
			game_board.goBack();
		} else if(cmd[0] == "perft") {
			int k;
			std::cin >> k;
			double st = clock();
			uint64_t count = perft(k);
			std::cout << count << " " << (int)((double)count / (((double)clock() - (double)st) / (double)CLOCKS_PER_SEC)) << "\n";
		}

		return true;
}
