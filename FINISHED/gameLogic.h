/*
1. gameState: added cursur coordinate[2]
2. gameState: symbol for pawn promotion
3. gameState: fileName, winner, playernames
4. gameState: gameFinish -> gameReason
*/
#pragma once
#include <vector>
#include <string>
#include <array>

/* To use gameState: "#include <gameLogic.h>"

 Include file for game state logic such as:
  - castling, 
  - en passant,
  - checkmate/stalemate detection,
 and etc. */ 

using std::string, std::array;


// struct for each element in board list
struct Cell {
    string pieceName = "None";
    string pieceSymbol = " ";
    string cellName = "00";
    char cellOwner = 'n'; // w:white ; b:black ; n:none ;
    bool isEmpty = true;
    bool hasMoves = false;
    int coordinate[2] = {9,9};
};

// typedef for board list
typedef array<array<Cell, 8>, 8> boardArray_t; //to replace lengthy 8x8 array type-specifier

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

	int curserPos[2] {3,3};

	// description of the last move made
	string moveHistory = ""; // or "White moved Pawn to e4"
	int moveCount = 0;

	string fileName = "";
	string whitePlayer = "Albino";
	string blackPlayer = "Obama";
	string winner = "No One Yet";
	string gameReason = "";
};

// --generate a new board--
boardArray_t getNewBoard();

// --generate a new gameState
gameState getNewGameState();

// --move piece--
void executeMove(gameState& state, int startCell[2], int endCell[2]);

// --check if pawn is promotable
bool isPawnPromotion(const gameState& state, int endCell[2]);

// --promotes a pawn to piece of choice
void pawnPromotion(gameState& state, int targetCell[2], int choice);

// --returns a new gameState with every possible move for a piece (even if making the move self checks king)--
gameState generatePieceMoves(gameState state, Cell moving_cell);

// --check if a specific piece is checking a King of color=king_color--
bool isCheckingKing(const gameState& state, Cell attacking_cell, char king_color);

// --check if color=checkColor is in check--
bool isInCheck(const gameState& state, char checkColor);

// --check if color = checkColor is in check after movement--
bool isInCheckAfterMove(gameState state, int startCell[2], int endCell[2], char checkColor);

// --returns a new gameState with selected cell's moves that cannot lead to self check--
gameState showAvailableMoves(gameState state, Cell selected_cell);

// --search for moves that will not end up being checked (for color == checkColor)--
bool hasLegalMoves(const gameState& state, char checkColor);
