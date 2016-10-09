#include "goback.hpp"

GoBack::GoBack() {}

void GoBack::goBack(Board* board) {
  board->passant_enable = passant_enable;
  board->passant_y = passant_y;
  board->passant_x = passant_x;

  board->whiteShortCastleEnable = whiteShortCastleEnable;
  board->whiteLongCastleEnable = whiteLongCastleEnable;
  board->blackShortCastleEnable = blackShortCastleEnable;
  board->blackLongCastleEnable = blackLongCastleEnable;
  board->numHalfMove = numHalfMove;
  board->move_rule_num = move_rule_num;

  board->whiteMove = !board->whiteMove;

  /*uint8_t color, enemyColor;

  if(board->whiteMove) {
    color = WHITE;
    enemyColor = BLACK;
  } else {
    color = BLACK;
    enemyColor = WHITE;
  }*/

  if(type == WS_CASTLING_MV) {
    board->board[7][4] = (KING | WHITE);
    board->board[7][5] = 0;
    board->board[7][6] = 0;
    board->board[7][7] = (ROOK | WHITE);
    board->whiteShortCastleEnable = true;
  } else if(type == WL_CASTLING_MV) {
    board->board[7][4] = (KING | WHITE);
    board->board[7][3] = 0;
    board->board[7][2] = 0;
    board->board[7][0] = (ROOK | WHITE);
  } else if(type == BS_CASTLING_MV) {
    board->board[0][4] = (KING | BLACK);
    board->board[0][5] = 0;
    board->board[0][6] = 0;
    board->board[0][7] = (ROOK | BLACK);
  } else if(type == BL_CASTLING_MV) {
    board->board[0][4] = (KING | BLACK);
    board->board[0][3] = 0;
    board->board[0][2] = 0;
    board->board[0][0] = (ROOK | BLACK);
  } else {
    board->board[fig1.y][fig1.x] = fig1.figure;
    board->board[fig2.y][fig2.x] = fig2.figure;

    if(passant_enable) {
      board->board[fig3.y][fig3.x] = fig3.figure;
    }
    /*if(type == STANDARD_MV || type == PASSANT_MV) {
      board->board[fromY][fromX] = board->board[toY][toX];
      board->board[toY][toX] = 0;
      if(type == STANDARD_MV) {
        if(attackMove) {
          board->board[toY][toX] = killedFigure;
        }
      } else if(type == PASSANT_MV) {
        if(color == WHITE) {
          board->board[passant_y + 1][passant_x] = (enemyColor | PAWN);
        } else {
          board->board[passant_y - 1][passant_x] = (enemyColor | PAWN);
        }
      }
    } else if(type == TRANSFORMATION_MV) {
      board->board[fromY][fromX] = (color | PAWN);
      board->board[toY][toX] = 0;
      if(attackMove) {
        board->board[toY][toX] = killedFigure;
      }
    }*/
  }
}
