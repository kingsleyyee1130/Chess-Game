#include <iostream>
#include <windows.h>
#include <array>
#include <string>
#include <iomanip>

using std::cout, std::string, std::array, std::to_string;


struct Cell {
    string pieceName = "None";
    string pieceSymbol;
    string cellName = "00";
    char cellOwner = 'n'; // w:white ; b:black ; n:none ;
    bool isEmpty = true;
    bool hasMoves = false;
    //bool isTargeted = false;
};

string col_alpha = "abcdefgh";
array<string, 9> chess_names = {"Rook", "Knight", "Bishop", "Queen", "King", "Bishop", "Knight", "Rook", "Pawn"};
array<string, 9> chess_symbol_black = {"♖", "♘", "♗", "♕", "♔", "♗", "♘", "♖", "♙"};
array<string, 9> chess_symbol_white = {"♜", "♞", "♝", "♛", "♚", "♝", "♞", "♜", "♟"};

typedef array<array<Cell, 8>, 8> boardArray_t;


boardArray_t getNewBoard() {
    boardArray_t board;
    int row = 8;
    int col = 8;

    for (int y=0; y < row; y++) {
        int rank = row - y; 
        for (int x=0; x < col; x++) {
            board[x][y].cellName = col_alpha[x] + to_string(rank);
        }
    }
    for (int x=0; x < col; x++) {
        for (int i : {0,1}) {
            board[x][i].pieceName   = (i==0 ? chess_names[x] : chess_names[8]);
            board[x][i].pieceSymbol = (i==0 ? chess_symbol_white[x] : chess_symbol_white[8]);
            board[x][i].cellOwner   = 'w';
            board[x][i].isEmpty     = false;
        }
        for (int i : {6,7}) {
            board[x][i].pieceName   = (i==7 ? chess_names[x] : chess_names[8]);
            board[x][i].pieceSymbol = (i==7 ? chess_symbol_black[x] : chess_symbol_black[8]);
            board[x][i].cellOwner   = 'b';
            board[x][i].isEmpty     = false;
        }
    }

    return board;
}


int main() {
    SetConsoleOutputCP(CP_UTF8);
    auto board = getNewBoard();
    cout << board[0][7].cellName << "\n" << board[0][7].pieceName << "\n" << board[0][5].cellOwner << "\n";

    for (int y=7; y >=0 ; y--) {
        for (int x=0; x < 8; x++) {
            cout << board[x][y].pieceSymbol << "  " ;
        }
        cout << "\n";
    }
    return 0;
}