#include "game.hpp"

double Game::evalute(Board & b) {
	//новая функция оценки

	//оценка материального преимущества
	double material_eval = 0, all_material_eval = 0;
	std::vector<uint64_t> figureMask(32, 0);

	for(int y = 0; y < BOARD_SIZE; ++y) {
		for(int x = 0; x < BOARD_SIZE; ++x) {
			if(b.getFigure(y, x) != 0) {
				figureMask[b.getFigure(y, x)] |= cells[y][x];
			}
		}
	}

	if(insufficientMaterial(figureMask)) {
		return 0;
	}

	material_eval += popcount64(figureMask[PAWN | WHITE]) * PAWN_EV;
	material_eval -= popcount64(figureMask[PAWN | BLACK]) * PAWN_EV;
	material_eval += popcount64(figureMask[KNIGHT | WHITE]) * KNIGHT_EV;
	material_eval -= popcount64(figureMask[KNIGHT | BLACK]) * KNIGHT_EV;
	material_eval += popcount64(figureMask[BISHOP | WHITE]) * BISHOP_EV;
	material_eval -= popcount64(figureMask[BISHOP | BLACK]) * BISHOP_EV;
	material_eval += popcount64(figureMask[ROOK | WHITE]) * ROOK_EV;
	material_eval -= popcount64(figureMask[ROOK | BLACK]) * ROOK_EV;
	material_eval += popcount64(figureMask[QUEEN | WHITE]) * QUEEN_EV;
	material_eval -= popcount64(figureMask[QUEEN | BLACK]) * QUEEN_EV;

	all_material_eval += popcount64(figureMask[PAWN | WHITE]) * PAWN_EV;
	all_material_eval += popcount64(figureMask[PAWN | BLACK]) * PAWN_EV;
	all_material_eval += popcount64(figureMask[KNIGHT | WHITE]) * KNIGHT_EV;
	all_material_eval += popcount64(figureMask[KNIGHT | BLACK]) * KNIGHT_EV;
	all_material_eval += popcount64(figureMask[BISHOP | WHITE]) * BISHOP_EV;
	all_material_eval += popcount64(figureMask[BISHOP | BLACK]) * BISHOP_EV;
	all_material_eval += popcount64(figureMask[ROOK | WHITE]) * ROOK_EV;
	all_material_eval += popcount64(figureMask[ROOK | BLACK]) * ROOK_EV;
	all_material_eval += popcount64(figureMask[QUEEN | WHITE]) * QUEEN_EV;
	all_material_eval += popcount64(figureMask[QUEEN | BLACK]) * QUEEN_EV;

	//оценка позиции (положение фигур)
	double figure_state_eval = 0;

	for(int y = 0; y < BOARD_SIZE; ++y) {
		for(int x = 0; x < BOARD_SIZE; ++x) {
			if(b.getFigure(y, x) == 0) {
				continue;
			}

			if((b.getFigure(y, x) & COLOR_SAVE) == WHITE) {
				if((b.getFigure(y, x) & TYPE_SAVE) == PAWN) {
					figure_state_eval += whitePawnMatr[y][x];
				} else if((b.getFigure(y, x) & TYPE_SAVE) == KNIGHT) {
					figure_state_eval += knightMatr[y][x];
				} else if((b.getFigure(y, x) & TYPE_SAVE) == BISHOP) {
					figure_state_eval += bishopMatr[y][x];
				} else if((b.getFigure(y, x) & TYPE_SAVE) == ROOK) {
					figure_state_eval += rookMatr[y][x];
				} else if((b.getFigure(y, x) & TYPE_SAVE) == QUEEN) {
					figure_state_eval += queenMatr[y][x];
				} else if((b.getFigure(y, x) & TYPE_SAVE) == KING) {
					figure_state_eval += ((kingDebuteMatr[y][x] * (all_material_eval / ALL_MATERIAL)) + kingEndGameMatr[y][x] *  (1 - all_material_eval / ALL_MATERIAL));
				}
			} else {
				if((b.getFigure(y, x) & TYPE_SAVE) == PAWN) {
					figure_state_eval -= whitePawnMatr[BOARD_SIZE - 1 - y][x];
				} else if((b.getFigure(y, x) & TYPE_SAVE) == KNIGHT) {
					figure_state_eval -= knightMatr[BOARD_SIZE - 1 - y][x];
				} else if((b.getFigure(y, x) & TYPE_SAVE) == BISHOP) {
					figure_state_eval -= bishopMatr[BOARD_SIZE - 1 - y][x];
				} else if((b.getFigure(y, x) & TYPE_SAVE) == ROOK) {
					figure_state_eval -= rookMatr[BOARD_SIZE - 1 - y][x];
				} else if((b.getFigure(y, x) & TYPE_SAVE) == QUEEN) {
					figure_state_eval -= queenMatr[BOARD_SIZE - 1 - y][x];
				} else if((b.getFigure(y, x) & TYPE_SAVE) == KING) {
					figure_state_eval -= ((kingDebuteMatr[BOARD_SIZE - 1 - y][x] * (all_material_eval / ALL_MATERIAL)) + kingEndGameMatr[BOARD_SIZE - 1 - y][x] *  (1 - all_material_eval / ALL_MATERIAL));
				}
			}
		}
	}

	double passedPawnBonus = 0;

	uint64_t whitePassedPawnMap = 0, blackPassedPawnMap = 0;

	for(int x = 0; x < BOARD_SIZE; ++x) {
		if(((pawnAttackCutter[x] ^ UINT64_MAX) & figureMask[PAWN | WHITE]) != 0 && ((pawnAttackCutter[x] ^ UINT64_MAX) & figureMask[PAWN | BLACK]) == 0) {
			passedPawnBonus += PASSED_PAWN_BONUS;
			whitePassedPawnMap |= (pawnAttackCutter[x] ^ UINT64_MAX) & figureMask[PAWN | WHITE];
		} else if(((pawnAttackCutter[x] ^ UINT64_MAX) & figureMask[PAWN | BLACK]) != 0 && ((pawnAttackCutter[x] ^ UINT64_MAX) & figureMask[PAWN | WHITE]) == 0) {
			passedPawnBonus -= PASSED_PAWN_BONUS;
			blackPassedPawnMap |= (pawnAttackCutter[x] ^ UINT64_MAX) & figureMask[PAWN | BLACK];
		}
	}

	if((whitePassedPawnMap | blackPassedPawnMap) != 0) {
		for(int y = 0; y < BOARD_SIZE; ++y) {
			for(int x = 0; x < BOARD_SIZE; ++x) {
				if((whitePassedPawnMap & cells[y][x]) != 0) {
					passedPawnBonus += passed_pawn_line[y];
					figure_state_eval -= whitePawnMatr[y][x];
				} else if((blackPassedPawnMap & cells[y][x]) != 0) {
					passedPawnBonus -= passed_pawn_line[BOARD_SIZE - 1 - y];
					figure_state_eval += whitePawnMatr[BOARD_SIZE - 1 - y][x];
				}
			}
		}
	}

	double pawnStructure = passedPawnBonus;

	return material_eval + figure_state_eval + pawnStructure;
}

bool Game::insufficientMaterial(std::vector<uint64_t>figureMask) {
	if(
		popcount64(figureMask[PAWN | WHITE]) != 0 ||
		popcount64(figureMask[PAWN | BLACK]) != 0 ||
		popcount64(figureMask[ROOK | WHITE]) != 0 ||
		popcount64(figureMask[ROOK | BLACK]) != 0 ||
		popcount64(figureMask[QUEEN | WHITE]) != 0 ||
		popcount64(figureMask[QUEEN | BLACK]) != 0
	) {
		return false;
	}

	return popcount64(figureMask[KNIGHT | WHITE]) + popcount64(figureMask[KNIGHT | BLACK]) + popcount64(figureMask[BISHOP | WHITE]) + popcount64(figureMask[BISHOP | BLACK]) <= 1;
}

double Game::getForcastEvalute(Board & b) {
	bool tmp_shah;
	std::vector<Move>moves = generatePositionMoves(b, tmp_shah, false, 0);

	double res = 0;

	for(int i = 0; i < moves.size(); ++i) {
		if(b.getFigure(moves[i].toY, moves[i].toX) == 0) {
			break;
		}

		double tmp = getFigureEval(b, moves[i].toY, moves[i].toX);

		if(tmp > res) {
			res = tmp;
		}
	}

	return res;
}

double Game::getFigureEval(Board & b, int y, int x) {
	if((b.getFigure(y, x) & TYPE_SAVE) == PAWN) {
		return PAWN_EV;
	} else if((b.getFigure(y, x) & TYPE_SAVE) == KNIGHT) {
		return KNIGHT_EV;
	} else if((b.getFigure(y, x) & TYPE_SAVE) == BISHOP) {
		return BISHOP_EV;
	} else if((b.getFigure(y, x) & TYPE_SAVE) == ROOK) {
		return ROOK_EV;
	} else if((b.getFigure(y, x) & TYPE_SAVE) == QUEEN) {
		return QUEEN_EV;
	}

	return 0;
}

bool Game::inCheck(Board & b, uint8_t color) {
	uint8_t enemyColor;
	if(color == WHITE) {
		enemyColor = BLACK;
	} else {
		enemyColor = WHITE;
	}

	uint64_t mask = 0;

	uint64_t kingPos;

	int yKingPos = 0, xKingPos = 0, yEnemyKingPos = 0, xEnemyKingPos = 0;

	uint64_t sedentaryMap = 0;

	for(unsigned int y = 0; y < BOARD_SIZE; ++y) {
		for(unsigned int x = 0; x < BOARD_SIZE; ++x) {
			if(b.getFigure(y, x) != 0 && (b.getFigure(y, x) & COLOR_SAVE) == enemyColor) {
				mask = (mask | bitboard[b.getFigure(y, x)][y][x]);
			}

			if(b.getFigure(y, x) == (KING | color)) {
				kingPos = cells[y][x];
				yKingPos = y;
				xKingPos = x;
			}

			if(b.getFigure(y, x) == (KING | enemyColor)) {
				sedentaryMap = (sedentaryMap | bitboard[KING | enemyColor][y][x]);
			}

			if(b.getFigure(y, x) == (PAWN | enemyColor)) {
				sedentaryMap = (sedentaryMap | (bitboard[PAWN | enemyColor][y][x] & pawnAttackCutter[x]));
			}

			if(b.getFigure(y, x) == (KNIGHT | enemyColor)) {
				sedentaryMap = (sedentaryMap | bitboard[KNIGHT | enemyColor][y][x]);
			}
		}
	}

	if((kingPos & sedentaryMap) > 0) {
		return true;
	}

	if(mask & kingPos) {
		for(int y = yKingPos; y < BOARD_SIZE; ++y) {
			if(y == yKingPos) {
				continue;
			}
			if((b.getFigure(y, xKingPos) & TYPE_SAVE) == ROOK || (b.getFigure(y, xKingPos) & TYPE_SAVE) == QUEEN) {
				if((b.getFigure(y, xKingPos) & COLOR_SAVE) == enemyColor) {
					return true;
				} else {
					break;
				}
			}
			if(b.getFigure(y, xKingPos) != 0) {
				break;
			}
		}

		for(int y = yKingPos; y >= 0; --y) {
			if(y == yKingPos) {
				continue;
			}
			if((b.getFigure(y, xKingPos) & TYPE_SAVE) == ROOK || (b.getFigure(y, xKingPos) & TYPE_SAVE) == QUEEN) {
				if((b.getFigure(y, xKingPos) & COLOR_SAVE) == enemyColor) {
					return true;
				} else {
					break;
				}
			}
			if(b.getFigure(y, xKingPos) != 0) {
				break;
			}
		}

		for(int x = xKingPos; x < BOARD_SIZE; ++x) {
			if(x == xKingPos) {
				continue;
			}
			if((b.getFigure(yKingPos, x) & TYPE_SAVE) == ROOK || (b.getFigure(yKingPos, x) & TYPE_SAVE) == QUEEN) {
				if((b.getFigure(yKingPos, x) & COLOR_SAVE) == enemyColor) {
					return true;
				} else {
					break;
				}
			}
			if(b.getFigure(yKingPos, x) != 0) {
				break;
			}
		}

		for(int x = xKingPos; x >= 0; --x) {
			if(x == xKingPos) {
				continue;
			}
			if((b.getFigure(yKingPos, x) & TYPE_SAVE) == ROOK || (b.getFigure(yKingPos, x) & TYPE_SAVE) == QUEEN) {
				if((b.getFigure(yKingPos, x) & COLOR_SAVE) == enemyColor) {
					return true;
				} else {
					break;
				}
			}
			if(b.getFigure(yKingPos, x) != 0) {
				break;
			}
		}

		for(int y = yKingPos, x = xKingPos; y >= 0 && x >= 0; --y, --x) {
			if(y == yKingPos && x == xKingPos) {
				continue;
			}

			if((b.getFigure(y, x) & TYPE_SAVE) == BISHOP || (b.getFigure(y, x) & TYPE_SAVE) == QUEEN) {
				if((b.getFigure(y, x) & COLOR_SAVE) == enemyColor) {
					return true;
				} else {
					break;
				}
			}
			if(b.getFigure(y, x) != 0) {
				break;
			}
		}

		for(int y = yKingPos, x = xKingPos; y >= 0 && x < BOARD_SIZE; --y, ++x) {
			if(y == yKingPos && x == xKingPos) {
				continue;
			}

			if((b.getFigure(y, x) & TYPE_SAVE) == BISHOP || (b.getFigure(y, x) & TYPE_SAVE) == QUEEN) {
				if((b.getFigure(y, x) & COLOR_SAVE) == enemyColor) {
					return true;
				} else {
					break;
				}
			}
			if(b.getFigure(y, x) != 0) {
				break;
			}
		}

		for(int y = yKingPos, x = xKingPos; y < BOARD_SIZE && x >= 0; ++y, --x) {
			if(y == yKingPos && x == xKingPos) {
				continue;
			}

			if((b.getFigure(y, x) & TYPE_SAVE) == BISHOP || (b.getFigure(y, x) & TYPE_SAVE) == QUEEN) {
				if((b.getFigure(y, x) & COLOR_SAVE) == enemyColor) {
					return true;
				} else {
					break;
				}
			}
			if(b.getFigure(y, x) != 0) {
				break;
			}
		}

		for(int y = yKingPos, x = xKingPos; y < BOARD_SIZE && x < BOARD_SIZE; ++y, ++x) {
			if(y == yKingPos && x == xKingPos) {
				continue;
			}

			if((b.getFigure(y, x) & TYPE_SAVE) == BISHOP || (b.getFigure(y, x) & TYPE_SAVE) == QUEEN) {
				if((b.getFigure(y, x) & COLOR_SAVE) == enemyColor) {
					return true;
				} else {
					break;
				}
			}
			if(b.getFigure(y, x) != 0) {
				break;
			}
		}
	}

	return false;
}

bool Game::inZugzwang(Board & b, uint8_t color) {
	double white_mat = 0, black_mat = 0, all_material = 0;
	for(int y = 0; y < BOARD_SIZE; ++y) {
		for(int x = 0; x < BOARD_SIZE; ++x) {
			if(b.getFigure(y, x) == 0) {
				continue;
			}

			if((b.getFigure(y, x) & COLOR_SAVE) == WHITE) {
				if((b.getFigure(y, x) & TYPE_SAVE) == PAWN) {
					white_mat += PAWN_EV;
					all_material += PAWN_EV;
				} else if((b.getFigure(y, x) & TYPE_SAVE) == KNIGHT) {
					white_mat += KNIGHT_EV;
					all_material += KNIGHT_EV;
				} else if((b.getFigure(y, x) & TYPE_SAVE) == BISHOP) {
					white_mat += BISHOP_EV;
					all_material += BISHOP_EV;
				} else if((b.getFigure(y, x) & TYPE_SAVE) == ROOK) {
					white_mat += ROOK_EV;
					all_material += ROOK_EV;
				} else if((b.getFigure(y, x) & TYPE_SAVE) == QUEEN) {
					white_mat += QUEEN_EV;
					all_material += QUEEN_EV;
				}
			} else {
				if((b.getFigure(y, x) & TYPE_SAVE) == PAWN) {
					black_mat += PAWN_EV;
					all_material += PAWN_EV;
				} else if((b.getFigure(y, x) & TYPE_SAVE) == KNIGHT) {
					black_mat += KNIGHT_EV;
					all_material += KNIGHT_EV;
				} else if((b.getFigure(y, x) & TYPE_SAVE) == BISHOP) {
					black_mat += BISHOP_EV;
					all_material += BISHOP_EV;
				} else if((b.getFigure(y, x) & TYPE_SAVE) == ROOK) {
					black_mat += ROOK_EV;
					all_material += ROOK_EV;
				} else if((b.getFigure(y, x) & TYPE_SAVE) == QUEEN) {
					black_mat += QUEEN_EV;
					all_material += QUEEN_EV;
				}
			}
		}
	}

	if(all_material < 7 * BISHOP_EV) {
		return true;
	}

	/*if(color == WHITE) {
		if(all_material > 4 * BISHOP_EV) {
			if(white_mat / black_mat < 0.25) {
				return true;
			} else {
				if(white_mat / black_mat < 0.5) {
					return true;
				}
			}
		}
	} else {
		if(all_material > 4 * BISHOP_EV) {
			if(black_mat / white_mat < 0.25) {
				return true;
			} else {
				if(black_mat / white_mat < 0.5) {
					return true;
				}
			}
		}
	}*/

	return false;
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

/*
uint8_t Game::getFigureAttacks(Board & b, uint8_t color) {
	uint8_t enemyColor;

	if(color == WHITE) {
		enemyColor = BLACK;
	} else {
		enemyColor = WHITE;
	}

	uint8_t result;

	for(int y = 0; y < BOARD_SIZE; ++y) {
		for(int x = 0; x < BOARD_SIZE; ++x) {
			if(b.getFigure(y, x) != 0 && (b.getFigure(y, x) & COLOR_SAVE) == color) {
				if((b.getFigure(y, x) & TYPE_SAVE) == QUEEN || )
			}
		}
	}
}*/

int Game::popcount64(uint64_t value) {
	return __builtin_popcountll(value);
}
