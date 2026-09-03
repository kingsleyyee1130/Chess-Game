#include <iostream>
#include "gameLogic.h"
#include "chessBoardList.h"

using std::string;


/* TO_UPDATE

add > bool gameState.isInCheck
add > inside executeMove, should switch isWhiteTurn

*/


// i want a function that returns a board with possible moves for a piece
boardArray_t universalCheckPieceMoves(gameState, string);


//check if a specific piece is checking a King of color=king_color
bool isCheckingKing(gameState state, Cell target_cell, char king_color) {

    //TODO: maybe need idividual piece moves AND the blockage and stuff
    boardArray_t board = universalCheckPieceMoves(state, target_cell.pieceName);
    for (auto subboard : board) {
        for (auto cell : subboard) {
            if (cell.hasMoves && cell.pieceName=="King" && cell.cellOwner == king_color)
            return true;
        }
    }
    return false;
}


// check if current color is in check after movement, (can set to check for next color after movement)
bool isInCheck(gameState state, int startCell[2], int endCell[2], bool checkForNext = false) {

    char target_color;
    if (checkForNext)
        target_color = (state.isWhiteTurn ? 'b' : 'w');
    else
        target_color = (state.isWhiteTurn ? 'w' : 'b');

    executeMove(state, startCell, endCell);
    for (auto subboard : state.board) {  //loops until found opponent piece and checks if it's attacking king
        for (auto cell : subboard) {
            if (cell.isEmpty)
                continue;
            if (cell.cellOwner==target_color)
                continue;
            if (isCheckingKing(state, cell, target_color))
                return true;
        }
    }
    return false;
}


//search for moves that can not get checked  *only be called at round start (after switching isWhiteTurn)*
bool hasLegalMoves(gameState state) {

    boardArray_t bigBoard = state.board;
    char current_color = (state.isWhiteTurn ? 'w' : 'b');

    for (auto bigSubboard : bigBoard){ //loops until found piece of current color
    for (auto bigCell : bigSubboard) {
        if (bigCell.cellOwner!=current_color)
            continue;

        boardArray_t moveBoard = universalCheckPieceMoves(state, bigCell.pieceName); //generates board with moves of that piece
        for (auto moveSubboard : moveBoard){ //loops until found movable cell
        for (auto moveCell : moveSubboard) {
            if (moveCell.hasMoves){
                if (! isInCheck(state, bigCell.coordinate, moveCell.coordinate))
                    return true;
            }
        }}
    }}
    return false;
}


//checkmate = is being checked + has no moves left
bool isCheckMate(gameState state) {

    bool isBeingChecked = state.isInCheck;
    bool hasNoMovesLeft = !hasLegalMoves(state);

    return (isBeingChecked && hasNoMovesLeft);
}


//stalemate = has no moves left + not being checked
bool isStaleMate(gameState state) {

    bool isBeingChecked = state.isInCheck;
    bool hasNoMovesLeft = !hasLegalMoves(state);

    return (hasNoMovesLeft && !isBeingChecked);
}
