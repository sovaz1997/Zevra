#include "game.hpp"

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

void Game::createPawnAttackCutter() {
	for(int i = 0; i < BOARD_SIZE; ++i) {
		pawnAttackCutter[i] = 0;
	}

	for(int x = 0; x < BOARD_SIZE; ++x) {
		for(int y = 0; y < BOARD_SIZE; ++y) {
			pawnAttackCutter[x] = (pawnAttackCutter[x] | cells[y][x]);
		}
	}

	for(int i = 0; i < BOARD_SIZE; ++i) {
		pawnAttackCutter[i] ^= UINT64_MAX;
	}
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

	cleanWhiteHistory();
	cleanBlackHistory();
	createPawnAttackCutter();
	clearCash();
	moveArray.resize(100);
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
