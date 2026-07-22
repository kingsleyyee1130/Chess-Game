#include <iostream>
#include <windows.h>
#include <array>
#include <string>

using std::cout, std::string, std::array, std::to_string;


struct Cell {
    string pieceName = "None";
    string pieceSymbol = " ";
    string cellName = "00";
    char cellOwner = 'n'; // 'w'= white ; 'b'= black ; 'n'= none 
    bool isEmpty = true;
    bool hasMoves = false;
    //bool isTargeted = false;
};

string col_alphabet = "abcdefgh";
array<string, 9> chess_names = {"Rook", "Knight", "Bishop", "Queen", "King", "Bishop", "Knight", "Rook", "Pawn"};
array<string, 9> chess_symbol_black = {"♖", "♘", "♗", "♕", "♔", "♗", "♘", "♖", "♙"};
array<string, 9> chess_symbol_white = {"♜", "♞", "♝", "♛", "♚", "♝", "♞", "♜", "♟"};

typedef array<array<Cell, 8>, 8> boardArray_t; //to replace lengthy 8x8 array type-specifier


boardArray_t getNewBoard() {
    boardArray_t board;
    int row = 8;
    int col = 8;

    for (int y=0; y < row; y++) { //setting cell names (eg. a1, e4, h8 ...)
        int rank = y + 1;
        for (int x=0; x < col; x++) {
            board[x][y].cellName = col_alphabet[x] + to_string(rank);
        }
    }
    for (int x=0; x < col; x++) { //setting statuses column by column
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


void printChessArrangement(boardArray_t board) {
    SetConsoleOutputCP(CP_UTF8);
    for (int y=7; y >=0 ; y--) {
        for (int x=0; x < 8; x++) {
            cout << board[x][y].pieceSymbol << "  " ;
        }
        cout << "\n";
    }
}

