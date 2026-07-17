#include <iostream>
#include <string>
#include <array>

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