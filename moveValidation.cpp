#include "chessBoardList.h"
#include "gameLogic.h"
#include "moveValidation.h"
#include <cmath>
#include <array>

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

// piece specific validation functions start here
bool validateKnight(int startCell[2], int endCell[2]) {
	
	int dx = abs(endCell[0] - startCell[0]);
	int dy = abs(endCell[1] - startCell[1]);
	
	if ((dx == 1 && dy == 2) || (dx == 2 && dy == 1))
		return true;
	else
		return false;
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

bool validateKing(int startCell[2], int endCell[2]) {
	int dx = abs(endCell[0] - startCell[0]);
	int dy = abs(endCell[1] - startCell[1]);

	if (dx <= 1 && dy <= 1 && (dx > 0 || dy > 0))
		return true;
	else
		return false;
}

bool validatePawn(const boardArray_t &board, const gameState &state, int startCell[2], int endCell[2]) {
	int dx = abs(endCell[0] - startCell[0]);
	int dy = endCell[1] - startCell[1];

	if (state.isWhiteTurn) {
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
		}
	}
	else {
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
		}
	}

	return false;
}

// this function is called every time when a pawn is selected by a user. 
// another section of code should be written in main() (or in executeMove() ?) such that, 
// if isPawnPromotion, then *executes the actual promotion*
bool isPawnPromotion(const gameState &state, int startCell[2], int endCell[2]) {
	const boardArray_t board = state.board;
	bool pawnCanMove = validatePawn(board, state, startCell, endCell);
	if (state.isWhiteTurn) {
		if (pawnCanMove == true && endCell[1] == 7 && board[startCell[0]][startCell[1]].cellOwner == 'w') {
			return true;
		}
	}
	else if (pawnCanMove == true && endCell[1] == 0 && board[startCell[0]][startCell[1]].cellOwner == 'b') {
		return true;
	}

	return false;
}

bool validateQueen(const boardArray_t &board, int startCell[2], int endCell[2]){
	bool rook = validateRook(board, startCell, endCell);
	bool bishop = validateBishop(board, startCell, endCell);

	return (rook == true || bishop == true);
}

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
		return validateKing(startCell, endCell);
	if (piece == "Pawn")
		return validatePawn(board, state, startCell, endCell);
	if (piece == "Queen")
		return validateQueen(board, startCell, endCell);

	return false;
}

// the main validation function
// this function is called whenever user attempts to execute a move
bool isMoveLegal(const gameState &state, int startCell[2], int endCell[2]) {
	if (state.board[startCell[0]][startCell[1]].isEmpty)
		return false;
	if (!isWithinBounds(startCell, endCell))
		return false;
	if (startCell[0] == endCell[0] && startCell[1] == endCell[1])
		return false;
	if (state.isWhiteTurn && state.board[startCell[0]][startCell[1]].cellOwner != 'w')
		return false;
	else if (!state.isWhiteTurn && state.board[startCell[0]][startCell[1]].cellOwner != 'b')
		return false;
	if (state.board[startCell[0]][startCell[1]].cellOwner == state.board[endCell[0]][endCell[1]].cellOwner)
		return false;

	return doesCellHaveMoves(state, startCell, endCell);
}

// this function is called by every cell on the board every time user selects a piece
// when getPossibleMoves == true for a cell, a dot appears on the cell
// returns validDestinations
std::vector <std::array<int, 2>>
 getPossibleMoves(const gameState &state, int startCell[2]) {
	const boardArray_t board = state.board;
	std::vector <std::array <int, 2>> validDestinations;
	
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			int candidateCell[2] = {x, y};

			if (isMoveLegal(state, startCell, candidateCell))
				validDestinations.push_back({x, y});
		}
	}

	return validDestinations;
}
