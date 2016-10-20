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
  	board->setFigure(KING | WHITE, 7, 4);
  	board->setFigure(0, 7, 5);
    board->setFigure(0, 7, 6);
    board->setFigure(ROOK | WHITE, 7, 7);
    board->whiteShortCastleEnable = true;
  } else if(type == WL_CASTLING_MV) {
    board->setFigure(KING | WHITE, 7, 4);
    board->setFigure(0, 7, 3);
    board->setFigure(0, 7, 2);
    board->setFigure(ROOK | WHITE, 7, 0);
  } else if(type == BS_CASTLING_MV) {
    board->setFigure(KING | BLACK, 0, 4);
    board->setFigure(0, 0, 5);
    board->setFigure(0, 0, 6);
    board->setFigure(ROOK | BLACK, 0, 7);
  } else if(type == BL_CASTLING_MV) {
    board->setFigure(KING | BLACK, 0, 4);
    board->setFigure(0, 0, 3);
    board->setFigure(0, 0, 2);
    board->setFigure(ROOK | BLACK, 0, 3);
  } else {
    board->setFigure(fig1.figure, fig1.y, fig1.x);
    board->setFigure(fig2.figure, fig2.y, fig2.x);
    if(passant_has) {
      board->setFigure(fig3.figure, fig3.y, fig3.x);
    }
  }
}
