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
    string pieceName[8] =
    { 
        "Rook", "Knight", "Bishop", "Queen", "King", "Bishop", "Knight", "Rook"
    };

    string whitePiece[8] =
    {
        "♜ ", "♞ ", "♝ ", "♛ ", "♚ ", "♝ ", "♞ ", "♜ "
    };

    string blackPiece[8] =
    {
        "♖ ", "♘ ", "♗ ", "♕ ", "♔ ", "♗ ", "♘ ", "♖ "
    };

    // Empty every cell first
    for (int r = 0;r < 8;r++)
    {
        for (int c = 0;c < 8;c++)
        {
            string columnLetter = "abcdefgh";
            board[c][r].cellName =
                columnLetter[c] + to_string(r + 1);

            board[c][r].pieceName = "None";
            board[c][r].pieceSymbol = "  ";
            board[c][r].cellOwner = 'n';
            board[c][r].isEmpty = true;
        }
    }

    // White back rank + pawns
    for (int i = 0;i < 8;i++)
    {
        board[i][0].pieceName = pieceName[i];
        board[i][0].pieceSymbol = whitePiece[i];
        board[i][0].cellOwner = 'w';
        board[i][0].isEmpty = false;

        board[i][1].pieceName = "Pawn";
        board[i][1].pieceSymbol = "♟\xEF\xB8\x8E ";
        board[i][1].cellOwner = 'w';
        board[i][1].isEmpty = false;
    }

    // Black back rank + pawns
    for (int i = 0;i < 8;i++)
    {
        board[i][7].pieceName = pieceName[i];
        board[i][7].pieceSymbol = blackPiece[i];
        board[i][7].cellOwner = 'b';
        board[i][7].isEmpty = false;

        board[i][6].pieceName = "Pawn";
        board[i][6].pieceSymbol = "♙ ";
        board[i][6].cellOwner = 'b';
        board[i][6].isEmpty = false;
    }

    return board;
}


void printChessArrangement(boardArray_t board) {
    
    for (int y=7; y >=0 ; y--) {
        for (int x=0; x < 8; x++) {
            cout << board[x][y].pieceSymbol << "  " ;
        }
        cout << "\n";
    }
}

