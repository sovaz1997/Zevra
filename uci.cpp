#include "game.hpp"

bool Game::uciHandler(std::string str) {
	std::vector<std::string> cmd = getStringArray(str);
		if(cmd[0] == "isready") {
			std::cout << "readyok\n";
		} else if(cmd[0] == "position") {
			gameHash.clear();
			gameHash.resize(0);
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
			  goFixedDepth();
			} else if(cmd[1] == "movetime") {
				goFixedTime(std::stoi(cmd[2]) - 100);
			}
		} else if(cmd[0] == "posmoves") {
			bool shah_tmp;
			std::vector<Move>moves = generatePositionMoves(game_board, shah_tmp, true, 0);
			for(unsigned int i = 0; i < moves.size(); ++i) {
				moves[i].print();
				std::cout << "\n";
			}

			std::cout << evalute(game_board) / PAWN_EV * 100 << "\n";
			std::cout << game_board.getFenPosition() << "\n";
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
			bool shah_tmp;
			std::cout << "!";
			for(unsigned int i = 0; i < 100000; ++i) {
				generatePositionMoves(game_board, shah_tmp, true, 0);
			}
			std::cout << "!";
		} else if(cmd[0] == "goback") {
			game_board.goBack();
		}

		return true;
}
