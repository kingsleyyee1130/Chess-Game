
#include <iostream>
#include <string>
#include <limits>
#include <cctype>
#include <cstdlib>

using namespace std;

const int BOARD_SIZE = 8;


// clear screen
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Welcome Page
void showWelcomeScreen() {
    clearScreen();
    cout << "\n";
    cout << "     ####  #   #  #####    #####   #####\n";
    cout << "    ##     #   #  #       #       #\n";
    cout << "    #      #####  #####    #####   #####\n";
    cout << "    ##     #   #  #             #       # \n";
    cout << "     ####  #   #  #####    #####   #####\n";
    cout << "\n";
    cout << "        =============================\n";
    cout << "          CONSOLE-BASED CHESS GAME\n";
    cout << "        =============================\n\n";
    cout << "   Press ENTER to continue...";
    cin.get();
}

// board and chess 
void initializeBoard(char board[BOARD_SIZE][BOARD_SIZE]) {
    const char backRank[BOARD_SIZE] = {'R','N','B','Q','K','B','N','R'};

    for (int col = 0; col < BOARD_SIZE; col++) {
        board[0][col] = tolower(backRank[col]); // Black back rank (rank 8)
        board[1][col] = 'p';                    // Black pawns (rank 7)
        board[6][col] = 'P';                    // White pawns (rank 2)
        board[7][col] = backRank[col];           // White back rank (rank 1)
    }
    for (int col = 0; col < BOARD_SIZE; col++)
        for (int row = 2; row <= 5; row++)
            board[row][col] = '.';               // Empty middle rows
}

//board and coodinates
void drawBoard(char board[BOARD_SIZE][BOARD_SIZE]) {
    clearScreen();

    cout << "\n";
    cout << "     a   b   c   d   e   f   g   h\n";
    cout << "   +---+---+---+---+---+---+---+---+\n";

    for (int row = 0; row < BOARD_SIZE; row++) {
        int rank = BOARD_SIZE - row; // row 0 -> rank 8, row 7 -> rank 1
        cout << " " << rank << " |";
        for (int col = 0; col < BOARD_SIZE; col++) {
            cout << " " << board[row][col] << " |";
        }
        cout << " " << rank << "\n";
        cout << "   +---+---+---+---+---+---+---+---+\n";
    }
    cout << "     a   b   c   d   e   f   g   h\n\n";
}

//Menu
int getMenuChoice(int minOption, int maxOption) {
    int choice;
    while (true) {
        cout << "Enter your choice (" << minOption << "-" << maxOption << "): ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice < minOption || choice > maxOption) {
            cout << "Please choose a number between " << minOption
                 << " and " << maxOption << ".\n";
            continue;
        }
        return choice;
    }
}

//input names
string getPlayerName(int playerNumber, const string& side) {
    string name;
    while (true) {
        cout << "Enter name for Player " << playerNumber << " (" << side << "): ";
        getline(cin, name);
        if (!name.empty()) return name;
        cout << "Name cannot be empty.\n";
    }
}

//main menu
int showMainMenu() {
    cout << "========================================\n";
    cout << "        CONSOLE-BASED CHESS GAME         \n";
    cout << "========================================\n";
    cout << " 1. Start New Game\n";
    cout << " 2. Continue Previous Game\n";
    cout << " 3. Exit Game\n";
    cout << "========================================\n";
    return getMenuChoice(1, 3);
}

//in-game menu
int showGameMenu() {
    cout << "\n---- Game Menu ----\n";
    cout << " 1. Make a Move\n";
    cout << " 2. Undo Last Move\n";
    cout << " 3. Show Move History\n";
    cout << " 4. Save Game\n";
    cout << " 5. Resign\n";
    cout << " 6. Back to Main Menu\n";
    cout << "--------------------\n";
    return getMenuChoice(1, 6);
}

bool parseCoordinate(const string& input, int& row, int& col) {
    if (input.length() != 2) return false;

    char fileChar = tolower(input[0]);
    char rankChar = input[1];

    if (fileChar < 'a' || fileChar > 'h') return false;
    if (rankChar < '1' || rankChar > '8') return false;

    col = fileChar - 'a';
    int rank = rankChar - '0';
    row = BOARD_SIZE - rank;
    return true;
}

bool getCoordinateInput(const string& prompt, int& row, int& col) {
    string input;
    while (true) {
        cout << prompt;
        cin >> input;

        if (input == "menu" || input == "MENU") return false;
        if (parseCoordinate(input, row, col)) return true;

        cout << "Invalid coordinate. Use format like 'e2' (file a-h, rank 1-8).\n";
    }
}

bool getMoveInput(int& fromRow, int& fromCol, int& toRow, int& toCol) {
    if (!getCoordinateInput("Move piece from (e.g. e2), or 'menu': ", fromRow, fromCol))
        return false;
    if (!getCoordinateInput("Move piece to   (e.g. e4), or 'menu': ", toRow, toCol))
        return false;
    return true;
}

int main() {
    char board[BOARD_SIZE][BOARD_SIZE];
    string whiteName, blackName;
    bool whiteTurn = true;
    bool running = true;

    showWelcomeScreen();

    while (running) {
        int mainChoice = showMainMenu();

        switch (mainChoice) {
            case 1: { // Start New Game
                clearScreen();
                cout << "-- New Game Setup --\n";
                whiteName = getPlayerName(1, "White");
                blackName = getPlayerName(2, "Black");
                initializeBoard(board);
                whiteTurn = true;

                bool inGame = true;
                while (inGame) {
                    drawBoard(board);
                    cout << whiteName << " (White)  vs  " << blackName << " (Black)\n";
                    cout << "Turn: " << (whiteTurn ? whiteName + " (White)" : blackName + " (Black)") << "\n";
                    int gameChoice = showGameMenu();

                    switch (gameChoice) {
                        case 1: { // Make a Move
                            int fromRow, fromCol, toRow, toCol;
                            if (getMoveInput(fromRow, fromCol, toRow, toCol)) {
                                // TODO(teammate): validate move legality,
                                // update board array, detect check/checkmate.
                                // Demo only: move the piece with no rule checks.
                                board[toRow][toCol] = board[fromRow][fromCol];
                                board[fromRow][fromCol] = '.';
                                whiteTurn = !whiteTurn;
                            }
                            break;
                        }
                        case 2: // Undo Last Move
                            // TODO(teammate): undo logic
                            cout << "[Undo not yet implemented]\n";
                            break;
                        case 3: // Show Move History
                            // TODO(teammate): move history logic
                            cout << "[Move history not yet implemented]\n";
                            break;
                        case 4: // Save Game
                            // TODO(teammate): save-to-file logic
                            cout << "[Save not yet implemented]\n";
                            break;
                        case 5: { // Resign
                            string loser = whiteTurn ? whiteName : blackName;
                            cout << loser << " resigned. Returning to main menu.\n";
                            inGame = false;
                            break;
                        }
                        case 6: // Back to Main Menu
                            inGame = false;
                            break;
                    }
                }
                break;
            }
            case 2: 
                cout << "[Continue previous game not yet implemented]\n";
                cout << "Press Enter to return to menu...";
                cin.get();
                break;
            case 3: // Exit Game
                clearScreen();
                cout << "Thanks for playing! Goodbye.\n";
                running = false;
                break;
        }
    }
    return 0;
}
