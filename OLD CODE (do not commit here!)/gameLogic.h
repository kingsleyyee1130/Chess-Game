#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "chessBoardList.h"

/* To use gameState: "#include <gameLogic.h>"

 Include file for game state logic such as:
  - castling, 
  - en passant,
  - checkmate/stalemate detection,
 and etc. */ 

// this struct is updated after every move to display the current game state
struct gameState {
	boardArray_t board;
	bool isWhiteTurn = true;
	bool isInCheck = false;

	// defaulting to no 'en passant target' at game start
	// using cellName format from chessBoardList.h
	string enPassantTarget = "00";

	// default castling availabities at game start
	bool whiteCanCastleKS = true;
	bool whiteCanCastleQS = true;
	bool blackCanCastleKS = true;
	bool blackCanCastleQS = true;

	// description of the last move made
	int moveCount = 1;
	string moveHistory = "";
};

struct matchHistory;

void executeMove(gameState& state, int startCell[2], int endCell[2]);

bool isKingInCheck(gameState& state);
bool isCheckmate(gameState& state);
bool isStalemate(gameState& state);

// records a single gameState before every move
void recordState(gameState& state, std::vector<gameState>& history);

// restores previous gameState from history
bool undoMove(gameState& state, std::vector<gameState>& history);
