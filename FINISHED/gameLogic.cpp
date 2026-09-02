/*
1. executeMove: deleted switch turn
2. executeMove: special eating for en passant first (order of code also changed)
3. executeMove: special moving for castling
*/
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "gameLogic.h"
#include "moveValidation.h"

#define MOVE_SYMBOL "-"
using std::cout, std::string, std::array, std::to_string;


string col_alphabet = "abcdefgh";
array<string, 9> chess_names = {"Rook", "Knight", "Bishop", "Queen", "King", "Bishop", "Knight", "Rook", "Pawn"};
array<string, 9> chess_symbol_black = {"\033[38;5;172m♜\033[0m", "\033[38;5;172m♞\033[0m", "\033[38;5;172m♝\033[0m", "\033[38;5;172m♛\033[0m", "\033[38;5;172m♚\033[0m", "\033[38;5;172m♝\033[0m", "\033[38;5;172m♞\033[0m", "\033[38;5;172m♜\033[0m", "\033[38;5;172m♙\033[0m"};
array<string, 9> chess_symbol_white = {"\033[38;5;230m♜\033[0m", "\033[38;5;230m♞\033[0m", "\033[38;5;230m♝\033[0m", "\033[38;5;230m♛\033[0m", "\033[38;5;230m♚\033[0m", "\033[38;5;230m♝\033[0m", "\033[38;5;230m♞\033[0m", "\033[38;5;230m♜\033[0m", "\033[38;5;230m♙\033[0m"};


//returns a new board
boardArray_t getNewBoard() {
    boardArray_t board;
    int row = 8;
    int col = 8;

    for (int y=0; y < row; y++) { //setting cell names (eg. a1, e4, h8 ...) & coordinate
        int rank = y + 1;
        for (int x=0; x < col; x++) {
            board[x][y].cellName = col_alphabet[x] + to_string(rank);
            board[x][y].coordinate[0] = x;
            board[x][y].coordinate[1] = y;
        }
    }
    for (int x=0; x < col; x++) {
        for (int i : {0,1}) { //setting white chess cell status
            board[x][i].pieceName   = (i==0 ? chess_names[x] : chess_names[8]);
            board[x][i].pieceSymbol = (i==0 ? chess_symbol_white[x] : chess_symbol_white[8]);
            board[x][i].cellOwner   = 'w';
            board[x][i].isEmpty     = false;
        }
        for (int i : {6,7}) { //setting black chess cell status
            board[x][i].pieceName   = (i==7 ? chess_names[x] : chess_names[8]);
            board[x][i].pieceSymbol = (i==7 ? chess_symbol_black[x] : chess_symbol_black[8]);
            board[x][i].cellOwner   = 'b';
            board[x][i].isEmpty     = false;
        }
    }
    return board;
}


//returns a new gameState
gameState getNewGameState() {
    gameState new_state;
    new_state.board = getNewBoard();

    return new_state;
}


//move piece 
void executeMove(gameState& state, int startCell[2], int endCell[2]) {

	// Square and Cell have the same meaning essentially,
	// the name difference is just to differentiate variables
	Cell& startSquare = state.board[startCell[0]][startCell[1]];
	Cell& endSquare = state.board[endCell[0]][endCell[1]];
		

	// if en passant is performed
	if (endSquare.cellName == state.enPassantTarget) {
		int eatenCell[2];
		if (endCell[1] == 5) {      //position of black pawn being eaten
			eatenCell[0] = endCell[0];
			eatenCell[1] = endCell[1] - 1;}
		else if (endCell[1] == 2) { //position of white pawn being eaten
			eatenCell[0] = endCell[0];
			eatenCell[1] = endCell[1] + 1;}
		else {
			std::cout << "\n[WARNING] En Passant Target Problem!! [WARNING]\n\n";
			return;}
			
		Cell& eatenSquare = state.board[eatenCell[0]][eatenCell[1]];
		eatenSquare.pieceName = "None";
		eatenSquare.pieceSymbol = " ";
		eatenSquare.cellOwner = 'n';
		eatenSquare.isEmpty = true;
	}

	//check for castling
	if (startSquare.pieceName=="King" && endSquare.pieceName=="Rook" && startSquare.cellOwner == endSquare.cellOwner) //validation were done in selection time
		{
		int kingxPos, rookxPos;
		if (endSquare.coordinate[0] > startSquare.coordinate[0]) { //setting king&rook x positions since same for both colors
			kingxPos = 6;
			rookxPos = 5;

		} else {
			kingxPos = 2;
			rookxPos = 3;
		}
		
		//determining y position based on turn color; assign king&rook to respective cell coordinate for data transfer
		int yPos = (state.isWhiteTurn ? 0 : 7);
		char color = (state.isWhiteTurn ? 'w' : 'b');
		Cell& kingCell = state.board[kingxPos][yPos];
		Cell& rookCell = state.board[rookxPos][yPos];

		//transfer piece to castled positions
		kingCell.pieceName = "King";
		rookCell.pieceName = "Rook";
		kingCell.pieceSymbol = startSquare.pieceSymbol;
		rookCell.pieceSymbol =   endSquare.pieceSymbol;
		kingCell.cellOwner = color;
		rookCell.cellOwner = color;
		kingCell.isEmpty = false;
		rookCell.isEmpty = false;
		
		//emptying original King&Rook cell
		  endSquare.pieceName = "None";
		startSquare.pieceName = "None";
		  endSquare.pieceSymbol = " ";
		startSquare.pieceSymbol = " ";
		  endSquare.cellOwner = 'n';
		startSquare.cellOwner = 'n';
		  endSquare.isEmpty = true;
		startSquare.isEmpty = true;
		
		//falsing all CanCastle boolean
		//because "update castle rights" section below uses endSquare to verify, which here it's already empty
		state.whiteCanCastleKS = false;
		state.whiteCanCastleQS = false;
		state.blackCanCastleKS = false;
		state.blackCanCastleQS = false;
		}

	else
		{
		// NORMAL move a piece from startSquare to endSquare
		  endSquare.pieceName = startSquare.pieceName;
		startSquare.pieceName = "None";
		  endSquare.pieceSymbol = startSquare.pieceSymbol;
		startSquare.pieceSymbol = " ";
		  endSquare.cellOwner = startSquare.cellOwner;
		startSquare.cellOwner = 'n';
		  endSquare.isEmpty = false;
		startSquare.isEmpty = true;
		}


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

	/* also revoke rights if a rook's home square gets captured by the
	opponent -- the block above only catches a rook moving on its OWN
	turn, so a captured rook otherwise leaves the flag stuck at true,
	which lets validateKing's castling check misfire later */
	if (endSquare.cellName == "a1") state.whiteCanCastleQS = false;
	if (endSquare.cellName == "h1") state.whiteCanCastleKS = false;
	if (endSquare.cellName == "a8") state.blackCanCastleQS = false;
	if (endSquare.cellName == "h8") state.blackCanCastleKS = false;
}


// checks if a pawn is valid for promotion
bool isPawnPromotion(const gameState &state, int endCell[2]) {
	const boardArray_t board = state.board;
	Cell endSquare = board[endCell[0]][endCell[1]];

	if (endSquare.pieceName != "Pawn")
		return false;
	if (endCell[1] == 7 && endSquare.cellOwner == 'w')
		return true;
	if (endCell[1] == 0 && endSquare.cellOwner == 'b')
		return true;

	return false;
}


// promotes a pawn
void promotePawn(gameState& state, int targetCell[2], int choice) {
    Cell& promoCell = state.board[targetCell[0]][targetCell[1]];
    array<string, 9> pieceSymbols = (promoCell.cellOwner=='w' ? chess_symbol_white : chess_symbol_black);
    string pieceChoice[6] = {" ", "Queen", "Bishop", "Knight", "Rook", "Pawn"};
    string piece = pieceChoice[choice];

    int pieceSymbolIndex;
    if (piece == "Queen")
        pieceSymbolIndex = 3;
    else if (piece == "Bishop")
        pieceSymbolIndex = 2;
    else if (piece == "Knight")
        pieceSymbolIndex = 1;
    else if (piece == "Rook")
        pieceSymbolIndex = 0;
    else if (piece == "Pawn")
        pieceSymbolIndex = 8;
    
    promoCell.pieceName = piece;
    promoCell.pieceSymbol = pieceSymbols[pieceSymbolIndex];
}


//returns a new gameState with every possible move for a piece (even if making the move self checks king)g)
gameState generatePieceMoves(gameState state, Cell moving_cell) {

    boardArray_t& board = state.board;

    for (auto& subboard : board){
    for (auto& cell : subboard) {
        if (isMoveLegal(state, moving_cell.coordinate, cell.coordinate)){ //can be used on any piece (in moveValidation.cpp)
            cell.hasMoves = true;
            if (cell.pieceSymbol == " ") cell.pieceSymbol = MOVE_SYMBOL;
        }
    }}
    return state;
}


//check if a specific piece is checking a King of color=king_color
bool isCheckingKing(const gameState& state, Cell attacking_cell, char king_color) {

    gameState state_show = generatePieceMoves(state, attacking_cell);
    for (auto subboard : state_show.board) {
        for (auto cell : subboard) {
            if (cell.hasMoves && cell.pieceName == "King" && cell.cellOwner == king_color)
                return true;
        }
    }
    return false;
}


//check if color=checkColor is in check
bool isInCheck(const gameState& state, char checkColor) {
    for (auto subboard : state.board){
    for (auto cell : subboard)       {
        if (cell.isEmpty)
            continue;
        if (cell.cellOwner==checkColor)
            continue;
        if (isCheckingKing(state, cell, checkColor))
            return true;
    }}
    return false;
}


//check if color = checkColor is in check after movement
bool isInCheckAfterMove(gameState state, int startCell[2], int endCell[2], char checkColor) {

    executeMove(state, startCell, endCell);
    return isInCheck(state, checkColor);
}


//returns a new gameState with selected cell's moves that cannot lead to self check
gameState showAvailableMoves(gameState state, Cell selected_cell) {
    gameState moveState = generatePieceMoves(state, selected_cell); //generates board with moves of that piece
    for (auto& moveSubboard : moveState.board){ //loops until found moves that causes self check and remove piece moving availability(cell.hasMoves=false)
    for (auto& moveCell : moveSubboard)       {
        if (moveCell.hasMoves){
            if (isInCheckAfterMove(state, selected_cell.coordinate, moveCell.coordinate, selected_cell.cellOwner)){ //check if selected's color's king is checked after move
                moveCell.hasMoves = false;
                if (moveCell.pieceSymbol==MOVE_SYMBOL)
                    moveCell.pieceSymbol = " ";
            }}
    }}
    return moveState;
}   


//search for moves that will not end up being checked (for color == checkColor)
bool hasLegalMoves(const gameState& state, char checkColor) {

    boardArray_t bigBoard = state.board;
    for (auto bigSubboard : bigBoard){ //search each cell until found piece of color = checkColor
    for (auto bigCell : bigSubboard) {
        if (bigCell.cellOwner==checkColor){

            gameState moveState = generatePieceMoves(state, bigCell); //generates board with moves of that piece (color=checkColor)
            for (auto moveSubboard : moveState.board){ //search for cell to move to
            for (auto moveCell : moveSubboard)       {
                if (moveCell.hasMoves){
                    if (!isInCheckAfterMove(state, bigCell.coordinate, moveCell.coordinate, checkColor)) //see if moving the piece will not check its king (color=checkColor)
                        return true;}
            }}
        }   
    }}
    return false;
}
