/*
1. validatePawn: added validation for en passant
2. validateKing: added validation for castling:: added gameState argument\
3. isPawnPromotion: changed
4. isMoveLegal: comments on each if statement
5. isMoveLegal: added special if statement for castling
*/


#include "gameLogic.h"
#include "moveValidation.h"
#include <cmath>
#include <array>

#include <iostream>
string dummy;

// to check if a coordinate is within chess board
bool isWithinBounds(int startCell[2], int endCell[2]) {
	// initializing
	bool startXTrue = false;
	bool startYTrue = false;
	bool endXTrue = false;
	bool endYTrue = false;

	return (startCell[0] >= 0 && startCell[0] < 8
		&& startCell[1] >= 0 && startCell[1] < 8
		&& endCell[0] >= 0 && endCell[0] < 8
		&& endCell[1] >= 0 && endCell[1] < 8);
}

// to check if all squares between startCell and endCell are empty
// used for pieces that move linearly such as rooks, bishops and queens
bool isPathClear(const boardArray_t &board, int startCell[2], int endCell[2]) {
	int stepX, stepY;

	// safety net: only straight (horizontal/vertical) or diagonal lines are
	// well-defined here. Anything else would make the loop below overshoot
	// the board and read out of bounds.
	int dx0 = abs(endCell[0] - startCell[0]);
	int dy0 = abs(endCell[1] - startCell[1]);
	if (!(dx0 == 0 || dy0 == 0 || dx0 == dy0))
		return false;

	if (startCell[0] > endCell[0])
		stepX = -1;
	else if (startCell[0] < endCell[0])
		stepX = 1;
	else if (startCell[0] == endCell[0])
		stepX = 0;

	if (startCell[1] > endCell[1])
		stepY = -1;
	else if (startCell[1] < endCell[1])
		stepY = 1;
	else if (startCell[1] == endCell[1])
		stepY = 0;

	int currentX = startCell[0] + stepX;
	int currentY = startCell[1] + stepY;

	for (currentX, currentY; currentX != endCell[0] || currentY != endCell[1]; currentX += stepX, currentY += stepY) {
		if (!board[currentX][currentY].isEmpty)
			return false;
	}

	return true;
}

// ==== piece specific validation functions start here ====
bool validateKnight(int startCell[2], int endCell[2]) {

	int dx = abs(endCell[0] - startCell[0]);
	int dy = abs(endCell[1] - startCell[1]);

	if ((dx == 1 && dy == 2) || (dx == 2 && dy == 1)){
		return true;}
	else {
		return false;}
}

bool validateRook(const boardArray_t &board, int startCell[2], int endCell[2]) {
	int dx = abs(endCell[0] - startCell[0]);
	int dy = abs(endCell[1] - startCell[1]);

	if ((dx == 0 && dy != 0) || (dx != 0 && dy == 0))
		return isPathClear(board, startCell, endCell);
	else
		return false;
}

bool validateBishop(const boardArray_t &board, int startCell[2], int endCell[2]){
	int dx = abs(endCell[0] - startCell[0]);
	int dy = abs(endCell[1] - startCell[1]);

	if (dy == dx)
		return isPathClear(board, startCell, endCell);
	else
		return false;
}

bool validateKing(const gameState &state, int startCell[2], int endCell[2]) {
	char owner = state.board[startCell[0]][startCell[1]].cellOwner; // use the King's OWN color, not the global turn flag
	int dx = abs(endCell[0] - startCell[0]);
	int dy = abs(endCell[1] - startCell[1]);

	if (dx <= 1 && dy <= 1 && (dx > 0 || dy > 0))
		return true;

	// castling always stays on the King's home rank; bail out early otherwise
	// so isPathClear is never called on a non-straight (e.g. diagonal) path,
	// which is what was walking off the board and reading out of bounds.
	if (startCell[1] != endCell[1])
		return false;

	const Cell& endSquare = state.board[endCell[0]][endCell[1]];
	// castling is only ever valid onto a square that actually still has
	// this player's own Rook on it -- without this, an empty (or
	// enemy-occupied) corner square with a stale canCastle flag looks
	// like a legal King move, which hides real stalemate/checkmate.
	bool destIsOwnRook = (endSquare.pieceName == "Rook" && endSquare.cellOwner == owner);

	if (owner == 'w' && startCell[1] == 0)  {
		if (endCell[1]==0 && (endCell[0]==0 || endCell[0]==7)) //search for rook of same row 0
			if (state.whiteCanCastleKS || state.whiteCanCastleQS)
				if (destIsOwnRook && isPathClear(state.board, startCell, endCell)) 
					return true;
	}
	if (owner == 'b' && startCell[1] == 7) {
		if (endCell[1]==7 && (endCell[0]==0 || endCell[0]==7)) //search for rook of same row 7
			if (state.blackCanCastleKS || state.blackCanCastleQS)
				if (destIsOwnRook && isPathClear(state.board, startCell, endCell))
					return true;
	}
	return false;
}

bool validatePawn(const boardArray_t &board, const gameState &state, int startCell[2], int endCell[2]) {
	int dx = abs(endCell[0] - startCell[0]);
	int dy = endCell[1] - startCell[1];
	char owner = board[startCell[0]][startCell[1]].cellOwner; // use the pawn's OWN color, not the global turn flag

	if (owner == 'w') {
		// White pawn logic (dy > 0)
		if (startCell[1] == 1) {
			if (dx == 0 && dy == 1 && board[endCell[0]][endCell[1]].isEmpty)
				return true;
			if (dx == 0 && dy == 2 && (board[endCell[0]][endCell[1]].isEmpty) && (board[endCell[0]][endCell[1] - 1].isEmpty))
				return true;
			else if ((dx == 1 && dy == 1) && board[endCell[0]][endCell[1]].cellOwner == 'b')
				return true;
		}
		else {
			if (dx == 0 && dy == 1 && board[endCell[0]][endCell[1]].isEmpty)
				return true;
			else if ((dx == 1 && dy == 1) && board[endCell[0]][endCell[1]].cellOwner == 'b')
				return true;
			else if ((dx == 1 && dy == 1) && board[endCell[0]][endCell[1]].cellName == state.enPassantTarget)
				return true;
		}
	}
	else if (owner == 'b') {
		// Black pawn logic (dy < 0)
		if (startCell[1] == 6) {
			if (dx == 0 && dy == -1 && board[endCell[0]][endCell[1]].isEmpty)
				return true;
			if (dx == 0 && dy == -2 && board[endCell[0]][endCell[1]].isEmpty && board[endCell[0]][endCell[1] + 1].isEmpty)
				return true;
			else if ((dx == 1 && dy == -1) && board[endCell[0]][endCell[1]].cellOwner == 'w')
				return true;
		}
		else {
			if (dx == 0 && dy == -1 && board[endCell[0]][endCell[1]].isEmpty)
				return true;
			else if ((dx == 1 && dy == -1) && board[endCell[0]][endCell[1]].cellOwner == 'w')
				return true;
			else if ((dx == 1 && dy == -1) && board[endCell[0]][endCell[1]].cellName == state.enPassantTarget)
				return true;
		}
	}

	return false;
}

bool validateQueen(const boardArray_t &board, int startCell[2], int endCell[2]){
	bool rook = validateRook(board, startCell, endCell);
	bool bishop = validateBishop(board, startCell, endCell);

	return (rook == true || bishop == true);
}

// generic validation function that recognises piece type in cell and calls validation function accordingly
bool doesCellHaveMoves(const gameState &state, int startCell[2], int endCell[2]) {
	const boardArray_t board = state.board;
	string piece = state.board[startCell[0]][startCell[1]].pieceName;

	if (piece == "Knight")
		return validateKnight(startCell, endCell);
	if (piece == "Rook")
		return validateRook(board, startCell, endCell);
	if (piece == "Bishop")
		return validateBishop(board, startCell, endCell);
	if (piece == "King")
		return validateKing(state, startCell, endCell);
	if (piece == "Pawn")
		return validatePawn(board, state, startCell, endCell);
	if (piece == "Queen")
		return validateQueen(board, startCell, endCell);

	return false;
}

// the main validation function
// this function is called whenever user attempts to execute a move
bool isMoveLegal(const gameState &state, int startCell[2], int endCell[2]) {
	const boardArray_t board = state.board;
	const Cell& startSquare = state.board[startCell[0]][startCell[1]];
	const Cell& endSquare = state.board[endCell[0]][endCell[1]];

	if (startSquare.isEmpty) //selected nothing
		return false;
	if (!isWithinBounds(startCell, endCell)) //outside board
		return false;
	if (startCell[0] == endCell[0] && startCell[1] == endCell[1]) //start == end position
		return false;
	if (startSquare.pieceName == "King" && endSquare.pieceName == "Rook"
		&& startSquare.cellOwner==endSquare.cellOwner && abs(endCell[0] - startCell[0]) > 1) //special bypass for castling (must be put on top of "eating self")
		return doesCellHaveMoves(state, startCell, endCell);
	if (startSquare.cellOwner == endSquare.cellOwner) //eating self--
		return false;

	return doesCellHaveMoves(state, startCell, endCell);
}

