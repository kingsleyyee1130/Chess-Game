#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "chessBoardList.h"
#include "gameLogic.h"

void executeMove(gameState& state, int startCell[2], int endCell[2]) {

	// Square and Cell have the same meaning essentially,
	// the name difference is just to differentiate variables
	Cell& startSquare = state.board[startCell[0]][startCell[1]];
	Cell& endSquare = state.board[endCell[0]][endCell[1]];
		
	// to move a piece from startSquare to endSquare
	endSquare.pieceName = startSquare.pieceName;
	startSquare.pieceName = "None";
	endSquare.pieceSymbol = startSquare.pieceSymbol;
	startSquare.pieceSymbol = " ";
	endSquare.cellOwner = startSquare.cellOwner;
	startSquare.cellOwner = 'n';
	startSquare.isEmpty = true;
	endSquare.isEmpty = false;


	/* updates en passant target
	if a pawn moves 2 cells
	set en passant target for the cell that was skipped
	otherwise set it to default */

	if (endSquare.pieceName == "Pawn" && abs(endCell[1] - startCell[1]) == 2) {
		if (state.isWhiteTurn == true)
			state.enPassantTarget = state.board[endCell[0]][endCell[1] - 1].cellName;
		else state.enPassantTarget = state.board[endCell[0]][endCell[1] + 1].cellName;
	}
	else state.enPassantTarget = "00";


	/* updates castling rights accordingly
	if king moves --> disable both KS and QS for castling
	if rook moves --> disable that side of castling */
	
	if (state.isWhiteTurn == true) {
		if (endSquare.pieceName == "King") {
			state.whiteCanCastleKS = false;
			state.whiteCanCastleQS = false;
		}
		else if (endSquare.pieceName == "Rook" && startSquare.cellName == "a1")
			state.whiteCanCastleQS = false;
		else if (endSquare.pieceName == "Rook" && startSquare.cellName == "h1")
			state.whiteCanCastleKS = false;
	}
	else {
		if (endSquare.pieceName == "King") {
			state.blackCanCastleKS = false;
			state.blackCanCastleQS = false;
		}
		else if (endSquare.pieceName == "Rook" && startSquare.cellName == "a8")
			state.blackCanCastleQS = false;
		else if (endSquare.pieceName == "Rook" && startSquare.cellName == "h8")
			state.blackCanCastleKS = false;
	}

	// switch turns
	state.isWhiteTurn = !state.isWhiteTurn;
}


// waiting for coder 2's framework to continue this part
bool isKingInCheck(gameState& state) {



	return true;
}

bool isCheckmate(gameState& state) {



};


bool isStalemate(gameState& state);


void recordState(gameState& state, gameState& history) {





}


bool undoMove(gameState& state, gameState& history);