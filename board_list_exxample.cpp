#include <iostream>
#include "chessBoardList.h"

int main() {
  auto board = getNewBoard();
  printChessArrangement(board);
  std::cout << board[0][0].cellName << "\n";
  std::cout << board[3][0].pieceName << "\n";
  std::cout << board[4][7].pieceSymbol << "\n";
  std::cout << board[5][5].isEmpty << "\n";
  std::cout << board[6][6].cellOwner << "\n";
    
  return 0;
}

/*  board[x][y] : (x,y) is the board coordinate, from bottom left, 0~7
 *
 *  board attributes: 1. board.cellName      =  "a1" / "h8" ...
 *                    2. board.pieceName     =  "King" / "Rook" ...
 *                    3. board.pieceSymbol   =  "♔"
 *                    4. board.isEmpty       =  true / false
 *                    5. board.cellOwner     =  'w' for white / 'b' for black / 'n' for none
 *  
 */ 
