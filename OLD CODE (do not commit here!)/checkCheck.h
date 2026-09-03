#ifndef CHECKCHECK_H
#define CHECKCHECK_H

#include "chessBoardList.h"
#include "gameLogic.h"


// needed a function that returns a board with possible moves for a piece
boardArray_t universalCheckPieceMoves(gameState, string);


// Prototype of helper functions in-the-making
bool isCheckingKing(gameState, Cell, char);

bool isInCheck(gameState, int, int, bool);

bool hasLegalMoves(gameState);

bool isCheckMate(gameState);

bool isStaleMate(gameState);



#endif
