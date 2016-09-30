#include "game.hpp"

std::vector<Move> Game::generatePositionMoves(Board & b, bool & shah, bool withCastling, int depth) {
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
			if(b.board[y][x] != 0) {
				if((b.board[y][x] & COLOR_SAVE) == color) {
					MoveList* current_list = &move_list[b.board[y][x]][y][x];
					if(current_list->first == NULL) { continue; }
					MoveItem* it = current_list->first;
					bool stopped = false;

					do {
						//if(it->move.fromX >= 0 && it->move.fromX < 8 && it->move.fromY >= 0 && it->move.fromY < 8 && it->move.toX >= 0 && it->move.toX < 8 && it->move.toY >= 0 && it->move.toY < 8) {
							if(withCastling) {
								if(it->move.moveType == WS_CASTLING_MV && b.whiteShortCastleEnable) {
									if(b.board[y][x + 1] == 0 && b.board[y][x + 2] == 0 && b.board[y][x] == (KING | WHITE) && b.board[y][x + 3] == (ROOK | WHITE)) {
										if(!inCheck(b, color, 7, 6) && !inCheck(b, color, 7, 5) && !inCheck(b, color, 7, 4)) {
											result.push_back(it->move);
										}
									}
								}

								if(it->move.moveType == WL_CASTLING_MV && b.whiteLongCastleEnable) {
									if(b.board[y][x - 1] == 0 && b.board[y][x - 2] == 0 && b.board[y][x - 3] == 0 && b.board[y][x] == (KING | WHITE) && b.board[y][x - 4] == (ROOK | WHITE)) {
										if(!inCheck(b, color, 7, 2) && !inCheck(b, color, 7, 3) && !inCheck(b, color, 7, 4) && !inCheck(b, color, 7, 4)) {
											result.push_back(it->move);
										}
									}
								}

								if(it->move.moveType == BS_CASTLING_MV && b.blackShortCastleEnable) {
									if(b.board[y][x + 1] == 0 && b.board[y][x + 2] == 0 && b.board[y][x] == (KING | BLACK) && b.board[y][x + 3] == (ROOK | BLACK)) {
										if(!inCheck(b, color, 0, 6) && !inCheck(b, color, 0, 5) && !inCheck(b, color, 0, 4)) {
											result.push_back(it->move);
										}
									}
								}

								if(it->move.moveType == BL_CASTLING_MV && b.blackLongCastleEnable) {
									if(b.board[y][x - 1] == 0 && b.board[y][x - 2] == 0 && b.board[y][x - 3] == 0 && b.board[y][x] == (KING | BLACK) && b.board[y][x - 4] == (ROOK | BLACK)) {
										if(!inCheck(b, color, 0, 4) && !inCheck(b, color, 0, 3) && !inCheck(b, color, 0, 2)) {
											result.push_back(it->move);
										}
									}
								}
							}

							if(it->move.moveType != WS_CASTLING_MV && it->move.moveType != WL_CASTLING_MV && it->move.moveType != BS_CASTLING_MV && it->move.moveType != BL_CASTLING_MV) {
								if((b.board[y][x] & TYPE_SAVE) != PAWN) {
									if(b.board[it->move.toY][it->move.toX] == 0) {
										result.push_back(it->move);
									} else if((b.board[it->move.toY][it->move.toX] & COLOR_SAVE) == color || (b.board[it->move.toY][it->move.toX] & TYPE_SAVE) == KING) {
										if((b.board[it->move.toY][it->move.toX] & TYPE_SAVE) == KING && (b.board[it->move.toY][it->move.toX] & COLOR_SAVE) != color) {
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
										if(b.board[it->move.toY][it->move.toX] != 0 && it->specialNext != NULL) {
											it = it->specialNext;
											continue;
										} else if(b.board[it->move.toY][it->move.toX] == 0) {
											result.push_back(it->move);
										}
									} else if(b.board[it->move.toY][it->move.toX] != 0) {
										if(it->move.moveType != PASSANT_MV) {
											if((b.board[it->move.toY][it->move.toX] & COLOR_SAVE) == color || (b.board[it->move.toY][it->move.toX] & TYPE_SAVE) == KING) {
												if((b.board[it->move.toY][it->move.toX] & TYPE_SAVE) == KING && (b.board[it->move.toY][it->move.toX] & COLOR_SAVE) != color) {
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
										if(b.board[it->move.toY][it->move.toX] == 0 &&
											 (b.board[it->move.passant_y][it->move.passant_x] & TYPE_SAVE) == PAWN &&
											 (b.board[it->move.passant_y][it->move.passant_x] & COLOR_SAVE) != color &&
											 b.passant_enable &&
											 it->move.toY == b.passant_y &&
											 it->move.toX == b.passant_x) {
											 result.push_back(it->move);
										}
									}
								}
								}
							//}

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
		for(int j = 0; j < num_attacks - 1; ++j) {
			if(result[j].getAttackPrice(b) < result[j + 1].getAttackPrice(b)) {
				std::swap(result[j], result[j+1]);
			}
		}
	}

	if(b.isWhiteMove()) {
		for(unsigned int i = num_attacks + 1; i < result.size() - 1; ++i) {
			for(int j = i - 1; j >= num_attacks && whiteHistorySort[result[j].fromY][result[j].fromX][result[j].toY][result[j].toX] < whiteHistorySort[result[j+1].fromY][result[j+1].fromX][result[j+1].toY][result[j+1].toX]; --j) {
				std::swap(result[j], result[j+1]);
			}
		}
	} else {
		for(unsigned int i = num_attacks + 1; i < result.size() - 1; ++i) {
			for(int j = i - 1; j >= num_attacks && blackHistorySort[result[j].fromY][result[j].fromX][result[j].toY][result[j].toX] < blackHistorySort[result[j+1].fromY][result[j+1].fromX][result[j+1].toY][result[j+1].toX]; --j) {
				std::swap(result[j], result[j+1]);
			}
		}
	}

	/*if(boardHash[getColorHash(b) & hash_cutter].enable && boardHash[getColorHash(b) & hash_cutter].depth >= max_depth - depth && boardHash[getColorHash(b) & hash_cutter].hash == getColorHash(b)) {
		for(unsigned int i = 0; i < result.size(); ++i) {
			if(result[i].equal(boardHash[getColorHash(b) & hash_cutter].move)) {
				std::swap(result[0], result[i]);
				result[0].fromHash = true;
				break;
			}
		}
	}*/

	/*for(unsigned int i = num_attacks; i < result.size(); ++i) {
		if(b.isWhiteMove()) {
			if(result[i].equal(whiteKiller[depth].move) && whiteKiller[depth].enable && whiteKiller[depth].move.quality()) {
				std::swap(result[i], result[0]);
				break;
			}
		} else {
			if(result[i].equal(blackKiller[depth].move) && blackKiller[depth].enable && blackKiller[depth].move.quality()) {
				std::swap(result[i], result[0]);
				break;
			}
		}
	}*/

	return result;
}
