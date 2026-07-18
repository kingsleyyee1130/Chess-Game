#include <iostream>
#include "chessBoardList.h"

int main() {
  auto board = getNewBoard();
  printChessArrangement(board);
  return 0;
}

/*  board[x][y] : (x,y) is the board coordinate, from bottom left, 0~7
 *
 *  board attributes: 1. board.cellName      =  "a1" / "h8" ...
 *                    2. board.pieceName     =  "King" / "Rook" ...
 *                    3. board.pieceSymbol   =  "♔"
 *                    4. board.isEmpty       =  true / false
 *                    5. cellOwner           =  'w' for white / 'b' for black
 *  
 */ 
