#pragma once
#include <iostream>
#include <stack>
#include "chessBoardList.h"

// define coordinates of a piece
struct coordinate {
	char col; // ‘a’ to ‘h’
	int row; // 1 to 8
};

// this struct is updated after every move to display the current game state
struct gameState {
	boardArray_t board;
	bool isWhiteTurn = true;

	// defaulting to no 'en passant target' at game start
	int enPassantTarget = { ' ', -1 };

	// default castling availabities at game start
	bool whiteCanCastleKS = true;
	bool whiteCanCastleQS = true;
	bool blackCanCastleKS = true;
	bool blackCanCastleQS = true;
};

void executeMove(gameState& state, int startCell[2], int endCell[2]) {

	// to move a piece from one cell to another
	state.board[endCell[0]][endCell[1]].pieceName = state.board[startCell[0]][startCell[1]].pieceName; // Error: arrays could only read numbers!
	state.board[startCell].pieceName = "None";

	// switch turns
	state.isWhiteTurn = !state.isWhiteTurn;
};

