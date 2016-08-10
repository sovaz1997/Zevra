#include "board.hpp"

Board::Board() {
	board.resize(BOARD_SIZE);
	for(uint8_t i = 0; i < board.size(); ++i) {
		board[i].resize(BOARD_SIZE);
		for(uint8_t j = 0; j < board.size(); ++j) {
			board[i][j] = 0;
		}
	}

	cleanBoard();
}

uint8_t Board::createFigure(uint8_t fig, uint8_t type) {
	return (fig | type);
}

uint8_t Board::createFigure(char FEN_byte) {
	if(FEN_byte == 'P') {
		return createFigure(PAWN, WHITE);
	} else if(FEN_byte == 'N') {
		return createFigure(KNIGHT, WHITE);
	} else if(FEN_byte == 'B') {
		return createFigure(BISHOP, WHITE);
	} else if(FEN_byte == 'R') {
		return createFigure(ROOK, WHITE);
	} else if(FEN_byte == 'Q') {
		return createFigure(QUEEN, WHITE);
	} else if(FEN_byte == 'K') {
		return createFigure(KING, WHITE);
	} else if(FEN_byte == 'p') {
		return createFigure(PAWN, BLACK);
	} else if(FEN_byte == 'n') {
		return createFigure(KNIGHT, BLACK);
	} else if(FEN_byte == 'b') {
		return createFigure(BISHOP, BLACK);
	} else if(FEN_byte == 'r') {
		return createFigure(ROOK, BLACK);
	} else if(FEN_byte == 'q') {
		return createFigure(QUEEN, BLACK);
	} else if(FEN_byte == 'k') {
		return createFigure(KING, BLACK);
	}

	return 0;
}

void Board::changeFigureType(uint8_t & fig, uint8_t type) {
	fig = (fig & COLOR_SAVE) | type;
}

void Board::changeFigureColor(uint8_t & fig, uint8_t color) {
	fig = (fig & TYPE_SAVE) | color;
}

void Board::setFenPosition(std::string FEN) {
	cleanBoard();

	int pointer = 0;

	std::string str;
	for(unsigned int i = pointer; i < FEN.size() && FEN[i] != ' '; ++i, ++pointer) {
		str.push_back(FEN[pointer]);
	}

	setFen(str);
	str.clear();
	++pointer;

	if(FEN[pointer] == 'w') {
		whiteMove = true;
	} else if(FEN[pointer] == 'b') {
		whiteMove = false;
	}

	pointer += 2;

	for(unsigned int i = pointer; i < FEN.size() && FEN[i] != ' '; ++i, ++pointer) {
		str.push_back(FEN[pointer]);
	}

	setCastling(str);
	str.clear();
	++pointer;

	for(unsigned int i = pointer; i < FEN.size() && FEN[i] != ' '; ++i, ++pointer) {
		str.push_back(FEN[pointer]);
	}

	setPassant(str);
	str.clear();
	++pointer;

	for(unsigned int i = pointer; i < FEN.size() && FEN[i] != ' '; ++i, ++pointer) {
		str.push_back(FEN[pointer]);
	}

	move_rule_num = std::stoi(str);
	str.clear();
	++pointer;

	for(unsigned int i = pointer; i < FEN.size() && FEN[i] != ' '; ++i, ++pointer) {
		str.push_back(FEN[pointer]);
	}

	numHalfMove = std::stoi(str);
}

std::string Board::getFenPosition() {
	std::string res;
	int k = 0;
	for(int y = 0; y < BOARD_SIZE; ++y) {
		k = 0;
		for(int x = 0; x < BOARD_SIZE; ++x) {
			if(board[y][x] == 0) {
				++k;
			} else if((board[y][x] & COLOR_SAVE) == WHITE) {
				if(k > 0) {
					res.push_back((char)(k + '0'));
					k = 0;
				}

				if((board[y][x] & TYPE_SAVE) == PAWN) {
					res.push_back('P');
				} else if((board[y][x] & TYPE_SAVE) == KNIGHT) {
					res.push_back('N');
				} else if((board[y][x] & TYPE_SAVE) == BISHOP) {
					res.push_back('B');
				} else if((board[y][x] & TYPE_SAVE) == ROOK) {
					res.push_back('R');
				} else if((board[y][x] & TYPE_SAVE) == QUEEN) {
					res.push_back('Q');
				} else if((board[y][x] & TYPE_SAVE) == KING) {
					res.push_back('K');
				}
			} else if((board[y][x] & COLOR_SAVE) == BLACK) {
				if(k > 0) {
					res.push_back((char)(k + '0'));
					k = 0;
				}

				if((board[y][x] & TYPE_SAVE) == PAWN) {
					res.push_back('p');
				} else if((board[y][x] & TYPE_SAVE) == KNIGHT) {
					res.push_back('n');
				} else if((board[y][x] & TYPE_SAVE) == BISHOP) {
					res.push_back('b');
				} else if((board[y][x] & TYPE_SAVE) == ROOK) {
					res.push_back('r');
				} else if((board[y][x] & TYPE_SAVE) == QUEEN) {
					res.push_back('q');
				} else if((board[y][x] & TYPE_SAVE) == KING) {
					res.push_back('k');
				}
			}

			if(k > 0 && x == BOARD_SIZE - 1) {
				res.push_back((char)(k + '0'));
			}
		}

		if(y != BOARD_SIZE - 1) {
			res += '/';
		} else {
			res.push_back(' ');
		}
	}

	if(whiteMove) {
		res += "w ";
	} else {
		res += "b ";
	}

	if(!whiteShortCastleEnable && !whiteLongCastleEnable && !blackShortCastleEnable && !blackLongCastleEnable) {
		res += "-";
	} else {
		if(whiteShortCastleEnable) {
			res.push_back('K');
		}

		if(whiteLongCastleEnable) {
			res.push_back('Q');
		}

		if(blackShortCastleEnable) {
			res.push_back('k');
		}

		if(blackLongCastleEnable) {
			res.push_back('q');
		}
	}

	res.push_back(' ');

	if(!passant_enable) {
		res += "-";
	} else {
		res.push_back((char)(passant_x + 'a'));
		res.push_back(7 - passant_y + '1');
	}

	res.push_back(' ');
	res += std::to_string(move_rule_num);
	res.push_back(' ');
	res += std::to_string(numHalfMove);

	return res;
}

void Board::setFen(std::string FEN) {
	for(unsigned int i = 0, pos = 0; pos < 64; ++i) {
		if(FEN[i] == '/') {
			continue;
		}

		if(FEN[i] >= '1' && FEN[i] <= '8') {
			pos += (FEN[i] - '0');
			continue;
		}
		putFigureOnBoard(createFigure(FEN[i]), pos / BOARD_SIZE, pos % BOARD_SIZE);

		++pos;
	}
}

void Board::setCastling(std::string FEN) {
	if(FEN[0] != '-') {
		for(unsigned int i = 0; i < FEN.size(); ++i) {
			if(FEN[i] == 'K') {
				whiteShortCastleEnable = true;
			} else if(FEN[i] == 'Q') {
				whiteLongCastleEnable = true;
			} else if(FEN[i] == 'k') {
				blackShortCastleEnable = true;
			} else if(FEN[i] == 'q') {
				blackLongCastleEnable = true;
			}
		}
	}
}

void Board::setPassant(std::string FEN) {
	if(FEN[0] != '-') {
		passant_enable = true;
		passant_x = FEN[0] - 'a';
		passant_y = 7 - (FEN[1] - '1');
	}
}

void Board::putFigureOnBoard(uint8_t figure, int y, int x) {
	board[y][x] = figure;
}

void Board::printBoard() {
	/*for(unsigned int y = 0; y < board.size(); ++y) {
		std::cout << (8 - y);
		for(unsigned int x = 0; x < board.size(); ++x) {
			if(board[y][x] != 0) {
				if((board[y][x] & COLOR_SAVE) == WHITE) {
					if((board[y][x] & TYPE_SAVE) == PAWN) {
						std::cout << "♙";
					} else if((board[y][x] & TYPE_SAVE) == KNIGHT) {
						std::cout << "♘";
					} else if((board[y][x] & TYPE_SAVE) == BISHOP) {
						std::cout << "♗";
					} else if((board[y][x] & TYPE_SAVE) == ROOK) {
						std::cout << "♖";
					} else if((board[y][x] & TYPE_SAVE) == QUEEN) {
						std::cout << "♕";
					} else if((board[y][x] & TYPE_SAVE) == KING) {
						std::cout << "♔";
					} else {
						std::cout << '.';
					}
				} else if((board[y][x] & COLOR_SAVE) == BLACK) {
					if((board[y][x] & TYPE_SAVE) == PAWN) {
						std::cout << "♟";
					} else if((board[y][x] & TYPE_SAVE) == KNIGHT) {
						std::cout << "♞";
					} else if((board[y][x] & TYPE_SAVE) == BISHOP) {
						std::cout << "♝";
					} else if((board[y][x] & TYPE_SAVE) == ROOK) {
						std::cout << "♜";
					} else if((board[y][x] & TYPE_SAVE) == QUEEN) {
						std::cout << "♛";
					} else if((board[y][x] & TYPE_SAVE) == KING) {
						std::cout << "♚";
					} else {
						std::cout << '.';
					}
				} else {
					std::cout << '.';
				}
			} else {
				std::cout << '.';
			}

		}
		std::cout << "\n";
	}
	std::cout << ' ';
	for(int i = 0; i < BOARD_SIZE; ++i) {
		std::cout << (char)(i + 'a');
	}

	std::cout << "\n" << getFenPosition() << "\n";*/
}

void Board::cleanBoard() {
	for(uint8_t y = 0; y < board.size(); ++y) {
		for(uint8_t x = 0; x < board.size(); ++x) {
			board[y][x] = 0;
		}
	}

	whiteShortCastleEnable = false;
	whiteLongCastleEnable = false;
	blackShortCastleEnable = false;
	blackLongCastleEnable = false;

	numHalfMove = 0;
	move_rule_num = 0;
	passant_enable = false;
	whiteMove = true;
}

uint8_t Board::getFigure(int y, int x) {
	if(y >= 0 && y < 8 && x >= 0 && x < 8) {
		return board[y][x];
	} else {
		return board[0][0];
	}
}

void Board::setFigure(uint8_t figure, int y, int x) {
	board[y][x] = figure;
}

void Board::move(Move & mv) {
	passant_enable = false;

	if(getFigure(mv.fromY, mv.fromX) == (PAWN | WHITE) && mv.fromY - mv.toY == 2) {
		passant_x = mv.fromX;
		passant_y = mv.fromY - 1;
		passant_enable = true;
	}

	if(getFigure(mv.fromY, mv.fromX) == (PAWN | BLACK) && mv.toY - mv.fromY == 2) {
		passant_x = mv.fromX;
		passant_y = mv.fromY + 1;
		passant_enable = true;
	}

	if((getFigure(mv.fromY, mv.fromX) & TYPE_SAVE) != PAWN && getFigure(mv.toY, mv.toX) == 0) {
		++move_rule_num;
	} else {
		move_rule_num = 0;
	}

	if(getFigure(mv.fromY, mv.fromX) == (KING | WHITE)) {
		whiteShortCastleEnable = false;
		whiteLongCastleEnable = false;
	}

	if(getFigure(mv.fromY, mv.fromX) == (KING | BLACK)) {
		blackShortCastleEnable = false;
		blackLongCastleEnable = false;
	}

	if(getFigure(mv.fromY, mv.fromX) == (ROOK | WHITE)) {
		if(mv.fromY == 7 && mv.fromX == 0) {
			whiteLongCastleEnable = false;
		} else if(mv.fromY == 7 && mv.fromX == 7) {
			whiteShortCastleEnable = false;
		}
	}

	if(getFigure(mv.fromY, mv.fromX) == (ROOK | BLACK)) {
		if(mv.fromY == 0 && mv.fromX == 0) {
			blackLongCastleEnable = false;
		} else if(mv.fromY == 0 && mv.fromX == 7) {
			blackShortCastleEnable = false;
		}
	}

	mv.go(this);
	whiteMove = !whiteMove;
	if(whiteMove) {
		++numHalfMove;
	}
}

bool Board::isWhiteMove() {
	return whiteMove;
}

 void Board::printStringBoard() {
    for(int y = 0; y < BOARD_SIZE; ++y) {
      for(int x = 0; x < BOARD_SIZE; ++x) {
        if(getFigure(y, x) == 0) {
          std::cout << "em";
        } else if((getFigure(y, x) & COLOR_SAVE) == WHITE) {
          if((getFigure(y, x) & TYPE_SAVE) == PAWN) {
            std::cout << "wp";
          } else if((getFigure(y, x) & TYPE_SAVE) == KNIGHT) {
            std::cout << "wh";
          } else if((getFigure(y, x) & TYPE_SAVE) == BISHOP) {
            std::cout << "we";
          } else if((getFigure(y, x) & TYPE_SAVE) == ROOK) {
            std::cout << "wr";
          } else if((getFigure(y, x) & TYPE_SAVE) == QUEEN) {
            std::cout << "wq";
          } else if((getFigure(y, x) & TYPE_SAVE) == KING) {
            std::cout << "wk";
          }
        } else if((getFigure(y, x) & COLOR_SAVE) == BLACK) {
          if((getFigure(y, x) & TYPE_SAVE) == PAWN) {
            std::cout << "bp";
          } else if((getFigure(y, x) & TYPE_SAVE) == KNIGHT) {
            std::cout << "bh";
          } else if((getFigure(y, x) & TYPE_SAVE) == BISHOP) {
            std::cout << "be";
          } else if((getFigure(y, x) & TYPE_SAVE) == ROOK) {
            std::cout << "br";
          } else if((getFigure(y, x) & TYPE_SAVE) == QUEEN) {
            std::cout << "bq";
          } else if((getFigure(y, x) & TYPE_SAVE) == KING) {
            std::cout << "bk";
          }
        }
      }
    }
  }
  
