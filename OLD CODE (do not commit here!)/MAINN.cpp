#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <array>
#include <string>
#include <cstdlib>
#include <cctype>
#include "gameLogic.h"
#include "storage.h"

using namespace std;

// Constants
const int BOARD_SIZE = 8;


// The one and only game state currently in play.
// The board is accessed as currentState.board[col][row].
gameState currentState;

string whitePlayer;
string blackPlayer;

bool pieceSelected = false;

int cursorRow = 7;
int cursorCol = 0;

int selectedRow = -1;
int selectedCol = -1;


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
    cout<<"                       Group 34                               \n";
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

    createFile(whitePlayer, blackPlayer);
}


// Resets game state (board + turn info) for a new game
void resetGame()
{
    currentState = gameState();          // back to default field values
    currentState.board = getNewBoard();  // fresh board goes inside it

    pieceSelected=false;

    cursorRow=7;
    cursorCol=0;

    selectedRow=-1;
    selectedCol=-1;
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

    cout << "\tMove : " << currentState.moveCount << endl;

    cout << "Black : " << blackPlayer;

    if(blackPlayer.length() < 10)
        cout << "\t";

    cout << "\tTurn : ";

    if(currentState.isWhiteTurn)
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
        cout << currentState.board[selectedCol][selectedRow].cellName;
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
    cout << " Q        : Save Game\n";
    cout << " U        : Undo Move\n";
    cout << " H        : View History\n";
    cout << " X        : Resign\n";

    cout << "--------------------------------------------------------\n";
}

// Draws a single board square
void drawCell(int row,int col)
{
    string piece = currentState.board[col][row].pieceSymbol;

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
    if(currentState.board[cursorCol][cursorRow].isEmpty)
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
        currentState.board[selectedCol][selectedRow].pieceName +
        " selected at " +
        currentState.board[selectedCol][selectedRow].cellName
    );

    // TODO: apply the move
}

// Save placeholder
void saveGame()
{
    messageBox(
        "Save Game",
        "The Game is Saved"
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

// Undo placeholder
void undoMove()
{
    messageBox(
        "Undo Move",
        "Waiting for teammate implementation."
    );
}

// View history placeholder
void viewHistory()
{
    messageBox(
        "View History",
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
    cout << "Moves  : " << currentState.moveCount << endl << endl;

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

    if(currentState.isWhiteTurn)
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

            case 'U':
                loadLatestGameState ();
                break;

            case 'H':
                viewHistory();
                break;

            case 'X':
                resignGame();
                return;

            default:
                break;
        }
    }
}

// Continue-from-menu placeholder
void continueGame()
{
    clearScreen();
    gameState state = gameState();
    cout << "==============================\n";
    cout << " Continue Game\n";
    cout << "==============================\n\n";

    int maximum = showAllFile();

    cout << "Choose a previous game to resume./n";
    cout << "(e.g. 1, 2, 3)";

    int choice = getMenuChoice(1, maximum);
    string fileResume = readFileName(choice);
    if (accessLastState(fileResume, currentState))
        gameLoop();
    else {
        cout << "Error: Failed to load saved game";
        pauseScreen();
    }
}

// Sets up and runs a new game
void startNewGame()
{
    inputPlayerNames();

    resetGame();

    gameLoop();
}

int main(int argc, char* argv[])
{
    // Enable UTF-8 mode in Windows Console for chess symbols
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    int choice;

    // If a game state file was passed on the command line, use it
    // instead of the default. e.g.  ./chess mysave.txt
    if(argc > 1)
    {
        existingFileName = argv[1];
        // pending completion
    }

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
