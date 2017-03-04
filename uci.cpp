/*
  Zevra, a UCI chess playing engine
  Copyright (C) 2016-2017 Oleg Smirnov (author)
  Zevra is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version.
  Zevra is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
			game_board.bitBoardMoveGenerator(moves);

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
		} else if(cmd[0] == "ucinewgame") {
			clearCash();
		} else if(cmd[0] == "stress") {
			game_board.stress = 0;
			double st = clock();
			for(unsigned int i = 0; i < 1000000; ++i) {
				game_board.bitBoardMoveGenerator(moveArray[0]);
			}
			std::cout << (int64_t)(game_board.stress / ((clock() - st) / CLOCKS_PER_SEC)) << std::endl;
		} else if(cmd[0] == "goback") {
			game_board.goBack();
			--hash_decrement;
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
					option.nullMovePrunningEnable = true;
				} else if(cmd[4] == "false") {
					option.nullMovePrunningEnable = false;
				}
			} else if(cmd[2] == "LMR" && cmd[3] == "value") {
				if(cmd[4] == "true") {
					option.lmrEnable = true;
				} else if(cmd[4] == "false") {
					option.lmrEnable = false;
				}
			}
		}

		return true;
}

void Game::idPrint() {
	std::cout << "id name Zevra v1.3.1 r340" << std::endl;
	std::cout << "id author sovaz1997" << std::endl;
}
