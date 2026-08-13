#include <iostream>
#include <iomanip>
#include <array>
#include <string>
#include <cstdlib>
#include <cctype>

using namespace std;

// Constants
const int BOARD_SIZE = 8;

// One board square
struct Cell
{
    string pieceName = "None";
    string pieceSymbol = "  ";
    string cellName = "";
    char cellOwner = 'n';      // w = White, b = Black, n = None
    bool isEmpty = true;
    bool hasMoves = false;
};

// 8x8 board type
typedef array<array<Cell, BOARD_SIZE>, BOARD_SIZE> boardArray_t;

// Game state
boardArray_t board;

string whitePlayer;
string blackPlayer;

bool whiteTurn = true;
bool pieceSelected = false;

int cursorRow = 7;
int cursorCol = 0;

int selectedRow = -1;
int selectedCol = -1;

int moveCounter = 1;

string columnLetter = "abcdefgh";

// Clears the console
void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Waits for ENTER
void pauseScreen()
{
    cin.ignore(1000,'\n');

    cout << "\nPress ENTER to continue...";
    cin.get();
}

// Reads a validated int in [minimum, maximum]
int getMenuChoice(int minimum,int maximum)
{
    int choice;

    while(true)
    {
        cout << "\nEnter Choice : ";
        cin >> choice;

        if(cin.fail())
        {
            cin.clear();
            cin.ignore(1000,'\n');

            cout << "\nInvalid Input!\n";
            continue;
        }

        if(choice>=minimum && choice<=maximum)
            return choice;

        cout << "\nPlease enter between "
             << minimum
             << " and "
             << maximum
             << ".\n";
    }
}

// Title screen
void showWelcomeScreen()
{
    clearScreen();

    cout<<"==============================================================\n";
    cout<<"                                                              \n";
    cout<<"                  CONSOLE CHESS GAME                          \n";
    cout<<"                                                              \n";
    cout<<"             Universiti Tunku Abdul Rahman                    \n";
    cout<<"                                                              \n";
    cout<<"==============================================================\n\n";

    cout<<"                 Press ENTER to Continue";

    pauseScreen();
}

// Main menu, returns chosen option
int showMainMenu()
{
    clearScreen();

    cout<<"==============================================================\n";
    cout<<"                  CONSOLE CHESS GAME\n";
    cout<<"==============================================================\n\n";

    cout<<"                 1. Start New Game\n\n";
    cout<<"                 2. Continue Game\n\n";
    cout<<"                 3. Exit\n\n";

    cout<<"==============================================================\n";

    return getMenuChoice(1,3);
}

// Asks for both player names
void inputPlayerNames()
{
    clearScreen();

    cin.ignore(1000,'\n');

    cout<<"White Player Name : ";
    getline(cin,whitePlayer);

    cout<<"\nBlack Player Name : ";
    getline(cin,blackPlayer);
}

// Builds a fresh starting board
boardArray_t getNewBoard()
{
    boardArray_t temp;

    string pieceName[8] =
    {
        "Rook","Knight","Bishop","Queen",
        "King","Bishop","Knight","Rook"
    };

    string whitePiece[8] =
    {
        "\u2656 ","\u2658 ","\u2657 ","\u2655 ",
        "\u2654 ","\u2657 ","\u2658 ","\u2656 "
    };

    string blackPiece[8] =
    {
        "\u265C ","\u265E ","\u265D ","\u265B ",
        "\u265A ","\u265D ","\u265E ","\u265C "
    };

    // Empty every cell first
    for(int r=0;r<8;r++)
    {
        for(int c=0;c<8;c++)
        {
            temp[c][r].cellName=
            columnLetter[c]+to_string(r+1);

            temp[c][r].pieceName="None";
            temp[c][r].pieceSymbol="  ";
            temp[c][r].cellOwner='n';
            temp[c][r].isEmpty=true;
        }
    }

    // White back rank + pawns
    for(int i=0;i<8;i++)
    {
        temp[i][0].pieceName=pieceName[i];
        temp[i][0].pieceSymbol=whitePiece[i];
        temp[i][0].cellOwner='w';
        temp[i][0].isEmpty=false;

        temp[i][1].pieceName="Pawn";
        temp[i][1].pieceSymbol="\u2659 ";
        temp[i][1].cellOwner='w';
        temp[i][1].isEmpty=false;
    }

    // Black back rank + pawns
    for(int i=0;i<8;i++)
    {
        temp[i][7].pieceName=pieceName[i];
        temp[i][7].pieceSymbol=blackPiece[i];
        temp[i][7].cellOwner='b';
        temp[i][7].isEmpty=false;

        temp[i][6].pieceName="Pawn";
        temp[i][6].pieceSymbol="\u265F ";
        temp[i][6].cellOwner='b';
        temp[i][6].isEmpty=false;
    }

    return temp;
}

// Resets board and turn state for a new game
void resetGame()
{
    board=getNewBoard();

    whiteTurn=true;

    pieceSelected=false;

    cursorRow=7;
    cursorCol=0;

    selectedRow=-1;
    selectedCol=-1;

    moveCounter=1;
}

// Continue-from-menu placeholder
void continueGame()
{
    clearScreen();

    cout<<"==============================\n";
    cout<<" Continue Game\n";
    cout<<"==============================\n\n";

    cout<<"Waiting for teammate.\n";

    pauseScreen();
}

// Exit screen
void exitScreen()
{
    clearScreen();

    cout<<"==============================\n";
    cout<<" Thanks for Playing\n";
    cout<<"==============================\n\n";

    cout<<"See You Again!\n";

    pauseScreen();
}

// Generic popup message
void messageBox(string title,string msg)
{
    clearScreen();

    cout<<"==============================\n";
    cout<<" "<<title<<endl;
    cout<<"==============================\n\n";

    cout<<msg<<endl;

    pauseScreen();
}

// Player names, move count, whose turn
void showGameInfo()
{
    cout << "White : " << whitePlayer;

    if(whitePlayer.length() < 10)
        cout << "\t";

    cout << "\tMove : " << moveCounter << endl;

    cout << "Black : " << blackPlayer;

    if(blackPlayer.length() < 10)
        cout << "\t";

    cout << "\tTurn : ";

    if(whiteTurn)
        cout << "White";
    else
        cout << "Black";

    cout << "\n\n";
}

// a-h column labels
void drawColumnHeader()
{
    cout << "      ";

    for(char c='a'; c<='h'; c++)
        cout << " " << c << "   ";

    cout << endl;
}

// Shows currently selected cell
void showSelectedPiece()
{
    cout << "\nSelected : ";

    if(pieceSelected)
        cout << board[selectedCol][selectedRow].cellName;
    else
        cout << "None";

    cout << endl;
}

// Key legend
void showControls()
{
    cout << "\n--------------------------------------------------------\n";

    cout << " W A S D  : Move Cursor\n";
    cout << " E        : Select Piece\n";
    cout << " R        : Continue Game\n";
    cout << " Q        : Save Game\n";
    cout << " X        : Resign\n";

    cout << "--------------------------------------------------------\n";
}

// Draws a single board square
void drawCell(int row,int col)
{
    string piece = board[col][row].pieceSymbol;

    if(piece=="")
        piece="  ";

    cout << "|";

    if(cursorRow==row && cursorCol==col)
    {
        cout << "[" << setw(2) << left << piece << "]";
    }
    else
    {
        cout << " " << setw(2) << left << piece << " ";
    }
}

// Draws all 8 rows
void drawBoardGrid()
{
    for(int row=7; row>=0; row--)
    {
        cout << "   -----------------------------------------\n";

        cout << row+1 << " ";

        for(int col=0; col<8; col++)
        {
            drawCell(row,col);
        }

        cout << "| " << row+1 << endl;
    }

    cout << "   -----------------------------------------\n";
}

// Full screen redraw
void drawBoard()
{
    clearScreen();

    cout << "========================================================\n";
    cout << "                 CONSOLE CHESS GAME\n";
    cout << "========================================================\n\n";

    showGameInfo();

    drawColumnHeader();

    drawBoardGrid();

    drawColumnHeader();

    showSelectedPiece();

    showControls();
}

// WASD cursor movement
void moveCursor(char key)
{
    key = toupper(key);

    switch(key)
    {
        case 'W':
            if(cursorRow < 7)
                cursorRow++;
            break;

        case 'S':
            if(cursorRow > 0)
                cursorRow--;
            break;

        case 'A':
            if(cursorCol > 0)
                cursorCol--;
            break;

        case 'D':
            if(cursorCol < 7)
                cursorCol++;
            break;
    }
}

// Selects the piece under the cursor (no move logic yet)
void selectPiece()
{
    if(board[cursorCol][cursorRow].isEmpty)
    {
        messageBox(
            "Selection",
            "No chess piece on this square."
        );
        return;
    }

    pieceSelected = true;

    selectedRow = cursorRow;
    selectedCol = cursorCol;

    messageBox(
        "Piece Selected",
        board[selectedCol][selectedRow].pieceName +
        " selected at " +
        board[selectedCol][selectedRow].cellName
    );

    // TODO: apply the move
}

// Save placeholder
void saveGame()
{
    messageBox(
        "Save Game",
        "Waiting for teammate implementation."
    );
}

// Load placeholder
void continueSavedGame()
{
    messageBox(
        "Continue Game",
        "Waiting for teammate implementation."
    );
}

// End-of-game winner screen
void winnerScreen(string winner)
{
    clearScreen();

    cout << "########################################################\n";
    cout << "#                                                      #\n";
    cout << "#                 CONGRATULATIONS!                     #\n";
    cout << "#                                                      #\n";
    cout << "########################################################\n\n";

    cout << "Winner : " << winner << endl;
    cout << "Moves  : " << moveCounter << endl << endl;

    pauseScreen();
}

// Current player resigns, opponent wins
void resignGame()
{
    clearScreen();

    cout << "========================================================\n";
    cout << "                    PLAYER RESIGNED\n";
    cout << "========================================================\n\n";

    string resigningPlayer;
    string winningPlayer;

    if(whiteTurn)
    {
        resigningPlayer = whitePlayer;
        winningPlayer = blackPlayer;
    }
    else
    {
        resigningPlayer = blackPlayer;
        winningPlayer = whitePlayer;
    }

    cout << resigningPlayer << " has resigned.\n\n";

    pauseScreen();

    winnerScreen(winningPlayer);
}

// Main input loop
void gameLoop()
{
    char command;

    while(true)
    {
        drawBoard();

        cout << "\nCommand : ";
        cin >> command;

        command = toupper(command);

        switch(command)
        {
            case 'W':
            case 'A':
            case 'S':
            case 'D':
                moveCursor(command);
                break;

            case 'E':
                selectPiece();
                break;

            case 'Q':
                saveGame();
                break;

            case 'R':
                continueSavedGame();
                break;

            case 'X':
                resignGame();
                return;

            default:
                break;
        }
    }
}

// Sets up and runs a new game
void startNewGame()
{
    inputPlayerNames();

    resetGame();

    gameLoop();
}

int main()
{
    int choice;

    showWelcomeScreen();

    while(true)
    {
        choice = showMainMenu();

        switch(choice)
        {
            case 1:
                startNewGame();
                break;

            case 2:
                continueGame();
                break;

            case 3:
                exitScreen();
                return 0;
        }
    }

    return 0;
}
