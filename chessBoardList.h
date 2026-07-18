#include <iostream>
#include <string>
#include <array>    

/*  "include file for using board functions"
 *  How to get board list: 1. #include "chessBoardList.h"
 *                         2. use auto type-identifier for getNewBoard() to accept board list
 *
 *  printChessArrangement() will only print chess symbol in order
 */ 

using std::string, std::array;

struct Cell {
    string pieceName = "None";
    string pieceSymbol;
    string cellName = "00";
    char cellOwner = 'n'; // w:white ; b:black ; n:none ;
    bool isEmpty = true;
    bool hasMoves = false;
    //bool isTargeted = false;
};

typedef array<array<Cell, 8>, 8> boardArray_t;


boardArray_t getNewBoard();

void printChessArrangement(boardArray_t);
