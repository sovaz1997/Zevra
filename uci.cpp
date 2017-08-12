#include "game.hpp"

bool Game::uciHandler(std::string str) {
	std::vector<std::string> cmd = getStringArray(str);
		if(cmd[0] == "isready") {
			std::cout << "readyok" << std::endl;
		} else if(cmd[0] == "position") {
			gameHash.clear();
			gameHash.resize(0);
			hash_decrement = 0;
			if(cmd[1] == "startpos") {
				game_board.setFen(game_board.startpos_fen);
				hash_decrement = 0;
				hashAge = 0;
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
		} else if(cmd[0] == "go") {
			if(cmd[1] == "depth") {
				max_depth = std::stoi(cmd[2]);
			  goFixedDepth();
			} else if(cmd[1] == "movetime") {
				goFixedTime(std::stoi(cmd[2]));
			} else if(cmd[1] == "infinite") {
				max_depth = 99;
				goFixedDepth();
			} else {
				wtime = 0, btime = 0;
				winc = 0, binc = 0, movestogo = 0, movestogoEnable = false;
				for(unsigned int i = 1; i < cmd.size(); ++i) {
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
			game_board.bitBoardMoveGenerator(moves, stress);

			for(unsigned int i = 0; i < moves.count; ++i) {
				std::cout << moves.moveArray[i].getMoveString();
				std::cout << std::endl;
			}

			game_board.evaluteAll();
			std::cout << game_board.getEvalute() / PAWN_EV * 100 << std::endl;
			std::cout << game_board.getFen() << std::endl;

			std::cout << "inCheck (WHITE) : " << game_board.inCheck(WHITE) << std::endl;
			std::cout << "inCheck (BLACK) : " << game_board.inCheck(BLACK) << std::endl;
			std::cout << "color_hash: " << game_board.getColorHash() << std::endl;
		} else if(cmd[0] == "move") {
			move(cmd[1]);
		} else if(cmd[0] == "quit") {
			return false;
		} else if(cmd[0] == "uci") {
			idPrint();
			option.print();
			std::cout << "uciok" << std::endl;
		} else if(cmd[0] == "bench") {
			std::cout << "Benchmarking..." << std::endl;
			game_board.stress = 0;
			double st = clock();


			//while((clock() - st) / CLOCKS_PER_SEC < 15) {
				std::vector<std::future<void>> future(8);
				count_moves = std::vector<size_t> (future.size(), 0);
				std::string oldFen = game_board.getFen();
				game_board.setFen(game_board.startpos_fen);

				
				for(unsigned int count_threads = 0; count_threads < count_moves.size(); ++count_threads) {
					future[count_threads] = std::async(std::launch::async, &Game::benchmarkThreadFunction, this, count_threads);

					//thread[count_threads] = std::thread(&Game::benchmarkThreadFunction, this, count_threads);
				}

				for(unsigned int count_threads = 0; count_threads < future.size(); ++count_threads) {
					//thread[count_threads].join();
					future[count_threads].get();
				}

				size_t result = 0;
				for(unsigned int i = 0; i < count_moves.size(); ++i) {
					result += count_moves[i];
				}
			//}
			std::cout << (int64_t)(result / ((clock() - st) / CLOCKS_PER_SEC))/* / 10000*/ << " scores" << std::endl;

			game_board.setFen(oldFen);
		} else if(cmd[0] == "goback") {
			game_board.goBack();
			--hash_decrement;
			--hashAge;
		} else if(cmd[0] == "perft") {
			int k;
			k = std::stoi(cmd[1]);
			for(int i = 1; i <= k; ++i) {
				combinations = 0;
				double st = clock();
				uint64_t count = perft(i);
				std::cout << "Depth: " << i << "; count: " << combinations;
				std::cout << "; speed: " << (int64_t)((double)count / (((double)clock() - (double)st) / (double)CLOCKS_PER_SEC)) << std::endl;
			}
		} else if(cmd[0] == "setoption" && cmd[1] == "name") {
			if(cmd[2] == "nullmove" && cmd[3] == "value") {
				if(cmd[4] == "true") {
					option.nullMovePruningEnable = true;
				} else if(cmd[4] == "false") {
					option.nullMovePruningEnable = false;
				}
			} else if(cmd[2] == "razoring" && cmd[3] == "value") {
				if(cmd[4] == "true") {
					option.razoring = true;
				} else if(cmd[4] == "false") {
					option.razoring = false;
				}
			} else if(cmd[2] == "futility_pruning" && cmd[3] == "value") {
				if(cmd[4] == "true") {
					option.futility_pruning = true;
				} else if(cmd[4] == "false") {
					option.futility_pruning = false;
				}
			} else if(cmd[2] == "checkExtensions" && cmd[3] == "value") {
				if(cmd[4] == "true") {
					option.checkExtensions = true;
				} else if(cmd[4] == "false") {
					option.checkExtensions = false;
				}
			} else if(cmd[2] == "Clear" && cmd[3] == "Hash") {
				clearCash();
				std::cout << "info hashfull 0" << std::endl;
			} else if(cmd[2] == "Hash" && cmd[3] == "value") {
				int hash_size = std::stoi(cmd[4]);

				hash_size = std::min(hash_size, option.max_hash_size);
				hash_size = std::max(hash_size, option.min_hash_size);

				setHashSize(hash_size);
			} else if(cmd[2] == "UCI_AnalyseMode" && cmd[3] == "value") {
				if(cmd[4] == "true") {
					option.UCI_AnalyseMode = true;
				} else if(cmd[4] == "false") {
					option.UCI_AnalyseMode = false;
				}
			}
		}

		return true;
}

void Game::idPrint() {
	std::cout << "id name Zevra v1.5 r446" << std::endl;
	std::cout << "id author sovaz1997" << std::endl;
}

void Game::benchmarkThreadFunction(size_t counter) {
	for(unsigned int i = 0; i < 1000000; ++i) {
		game_board.bitBoardMoveGenerator(moveArray[counter], count_moves[counter]);
	}
}