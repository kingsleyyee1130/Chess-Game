#include <iostream>
#include <iomanip>
#include <windows.h>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <array>
#include <string>
#include <conio.h>

#include "gameFlow.h"
#include "gameLogic.h"
#include "storage.h"

using std::cout;
using std::cin;
using std::setw;
using std::string;
using std::array;
using std::toupper;
using std::getline;
using std::to_string;


const int VIEW_WIDTH = 50;
string dummy_string; // use in getline to accept anything including "enter"
string ERROR_TEXT_FOR_2 = "-- Please Enter only the Number 1 and 2 --";


//============ HELPER FUNCTIONS ===========//

// Check string for whitespaces
bool hasSpace(string word) {
    for (char c : word) {
        if (std::isspace(c))
            return true;
    }
    return false;
}

// Check string for punctuations
bool hasPunct(string word) {
    for (char c : word) {
        if (std::ispunct(c))
            return true;
    }
    return false;
}

// Clears the console
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

//NEEDDO// Stops console for next enter input
void waitForEnter() {
    getline(cin, dummy_string);
}

//NEEDDO// Generate text centered in a specified length, surrounded by character of choice defaulted to space
string centeredString(string text, int length, char space = ' ') {

    int left  = length/2 - floor(text.length()/2.0);
    int right = length/2 - ceil(text.length()/2.0);

    return string(left, space) + text + string(right, space);
}

//NEEDDO// Returns intended num between max and min, returns 999 if not
int getChoice(int minimum, int maximum, const string error_text = "") {
    int choice;
    
    if (!error_text.empty())
        cout << "\n\n" << error_text << "\n";
    cout << "\n>>";
    cin >> choice;

    bool input_err = cin.fail();

    if (input_err) cin.clear();

    cin.ignore(1000, '\n');
    
    if (input_err || choice < minimum || choice > maximum) 
        return 999;  
    
    return choice; 
}



//============= DISPLAY FUNCTIONS ===========//

// Title screen
void showWelcomeScreen() {
    clearScreen();

    cout<<"==============================================================\n";
    cout<<"                                                              \n";
    cout<<"                     CONSOLE CHESS GAME                          \n";
    cout<<"                                                              \n";
    cout<<"                Universiti Tunku Abdul Rahman                    \n";
    cout<<"                                                              \n";
    cout<<"                          Group 34                               \n";
    cout<<"==============================================================\n\n";

    cout<<"                   Press ENTER to Continue\n";

    waitForEnter();
} 

//NEEDDO// Main menu, returns chosen option
int showMainMenu() {
    int input;
    string error;
    while(true) {
        clearScreen();

        cout<<"==============================================================\n";
        cout<<"                    CONSOLE CHESS GAME\n";
        cout<<"==============================================================\n\n";

        cout<<"                   1. Start New Game\n\n";
        cout<<"                   2. Continue Game\n\n";
        cout<<"                   3. Exit\n\n";

        cout<<"==============================================================\n";

        input = getChoice(1,3,error);
        if(input==999) 
            error = "-- Please Enter only 1~3 --";
        else
            break;
    }
    return input;
}

// Exit screen
void exitScreen() {
    clearScreen();

    cout<< string(VIEW_WIDTH, '=') << "\n\n";
    cout<< centeredString("~~~~~~~  Thank You for Playing  ~~~~~~~", VIEW_WIDTH) << "\n\n";
    cout<< string(VIEW_WIDTH, '=') << "\n\n";

    cout<< centeredString("See You Again!", VIEW_WIDTH);

    waitForEnter();
}

// Generic popup message
void messageBox(string title,string msg) {
    clearScreen();

    cout<< string(VIEW_WIDTH, '=')<<"\n";
    cout<<centeredString(title, VIEW_WIDTH)<<std::endl;
    cout<< string(VIEW_WIDTH, '=')<<"\n\n";

    cout<<msg<<std::endl;

    waitForEnter();
}

//NEEDDO// Print board with coordinate alphanumbers
void printPlayBoard(const gameState& state, int indent=8) {
    SetConsoleOutputCP(CP_UTF8);
    auto board = state.board;
    char lefts  = ' ';
    char rights = ' ';

    for (int y=7; y>=0; y--){
        cout << string(indent, ' ') << string(33, '-') << "\n";
        cout << setw(indent-1) << y+1 << ' ';
        for (int x=0; x<8; x++) {
            if (board[x][y].coordinate[0]==state.curserPos[0] && board[x][y].coordinate[1]==state.curserPos[1]){
                lefts  ='{';
                rights ='}';}
            cout << '|' << lefts << board[x][y].pieceSymbol << rights;
            lefts  = ' ';
            rights = ' ';
        }
        cout << "|\n"; //notice there is a "|" at front
    }
        cout << string(indent, ' ') << string(33, '-') << "\n";
        cout << string(indent, ' ');
        for (int i=0; i<8; i++){
            char alpha = 'a'+i;
            cout << setw(3) << alpha << ' ';
        }
        cout << "\n\n";
}

// Display including board and commands 
void displayPlayView (const gameState& state, bool moving = false) {
    Cell chosenCell = state.board[state.curserPos[0]][state.curserPos[1]];
    string current_color = (state.isWhiteTurn ? "White" : "Black");
    string history = state.moveHistory;
    string game_name = state.fileName.substr(0, state.fileName.size()-4);
    string movesobig = "";

    if (state.moveCount == 64)
        movesobig = "  wth why so many moves??";
    else if (state.moveCount == 66)
        movesobig = "";
    else if (state.moveCount == 72)
        movesobig = "  mygod still going ?!!";
    else if (state.moveCount == 74)
        movesobig = "";

    cout << "Game : " << setw(VIEW_WIDTH-21) << std::left << "game_name" << std::right << "Half-Move: " << state.moveCount << movesobig << "\n";
    cout << "White: " << state.whitePlayer << "\n";
    cout << "Black: " << state.blackPlayer << "\n";
    cout << "Turn : " << current_color << " | Move History: " << (history.length()>21 ? history.substr(history.length()-21) : history) << "\n";
    cout << string(VIEW_WIDTH, '-') << '\n';

    printPlayBoard(state);

    if (state.gameReason=="") {
        cout << string(VIEW_WIDTH, '-') << "\n";
        if (state.isInCheck) {
            cout << centeredString("CHECK!!", VIEW_WIDTH) << "\n";
            cout << string(VIEW_WIDTH, '-') << "\n";
        }
        cout << " WASD / Arrows  : Move Cursor\n";
        if (!moving){
        cout << " E    / Enter   : Select Piece\n";
        cout << " Q              : Save and Quit\n";
        cout << " U              : Undo Move\n";
        cout << " X              : Resign\n";
        cout << " P              : Draw\n";
        }
        else {
        cout << " E    / Enter   : Select Move\n";
        cout << " C              : Cancel Selection\n";
        }
    }
    else if (state.gameReason=="Checkmated") {
        cout << string(VIEW_WIDTH, '-') << "\n";
        cout << centeredString("CHECKMATE!!", VIEW_WIDTH) << "\n";
    }
    else if (state.gameReason=="Stalemated") {
        cout << string(VIEW_WIDTH, '-') << "\n";
        cout << centeredString("STALEMATE!!", VIEW_WIDTH) << "\n"; 
    }
    cout << string(VIEW_WIDTH, '-') << std::endl;

    if (state.gameReason=="Checkmated" || state.gameReason=="Stalemated")
        cout << "Press Enter to Continue...";
}

// Result screen when someone win
void winnerScreen(const gameState& state) {   
    string loser = (state.winner==state.whitePlayer ? state.blackPlayer : state.whitePlayer);
    string reason = (state.gameReason=="Resigned" ? loser+" Resigned" : "Checkmated "+loser);

    char boarder = '#';
    clearScreen();
    cout << string(VIEW_WIDTH, boarder) << "\n";
    cout << boarder << string(VIEW_WIDTH-2, ' ') << boarder << "\n";
    cout << boarder << centeredString("CONGRATULATIONS!", VIEW_WIDTH-2) << boarder << "\n";
    cout << boarder << string(VIEW_WIDTH-2, ' ') << boarder << "\n";
    cout << string(VIEW_WIDTH, boarder) << "\n\n";
    cout << string(VIEW_WIDTH, '-') << "\n";

    cout << "Winner       : " << state.winner << "\n";
    cout << "Total Moves  : " << state.moveCount << "\n";
    cout << "Reason       : " << reason <<"\n\n";
    cout << "Press Enter to Go Back to Menu...";
    waitForEnter();
}

//NEEDDO// Result screen when someone drawd (stalemate screen included)
void drawScreen(const gameState& state, bool stalemate=false) {
    string display_text = (stalemate ? "STALEMATE" : "DRAW");

    char boarder = '=';
    clearScreen();
    cout << string(VIEW_WIDTH, boarder) << "\n";
    cout << boarder << string(VIEW_WIDTH-2, ' ') << boarder << "\n";
    cout << boarder << centeredString(display_text, VIEW_WIDTH-2) << boarder << "\n";
    cout << boarder << string(VIEW_WIDTH-2, ' ') << boarder << "\n";
    cout << string(VIEW_WIDTH, boarder) << "\n\n";
    cout << string(VIEW_WIDTH, '-') << "\n";

    cout << "Total Moves  : " << state.moveCount << "\n\n";
    cout << "Press Enter to Go Back to Main Menu...";
    waitForEnter();
}

//NEEDDO// Result screen someone resign
void resignScreen(const gameState& state) {
    string display_text;
    if (state.winner == state.whitePlayer)
        display_text = state.blackPlayer + " RESIGNED";
    else 
        display_text = state.whitePlayer + " RESIGNED";

    char boarder = '=';
    clearScreen();
    displayPlayView(state);
    cout << string(VIEW_WIDTH, boarder) << "\n";
    cout << boarder << string(VIEW_WIDTH-2, ' ') << boarder << "\n";
    cout << boarder << centeredString(display_text, VIEW_WIDTH-2) << boarder << "\n";
    cout << boarder << string(VIEW_WIDTH-2, ' ') << boarder << "\n";
    cout << string(VIEW_WIDTH, boarder) << "\n\n";
    waitForEnter();

    winnerScreen(state);
    
}



//============ GAME FLOW FUNCTIONS ============//
 
// Input validation for Quit Game (inside game loop)
void quitConfirmation(gameState& state) {
    string error_text = "";
    int quit_rspd;
    while (true){ 
        clearScreen();
        displayPlayView(state);
        cout << "Sure to Save and Quit? \n1.Yes\n2.No";
        quit_rspd = getChoice(1,2, error_text);
        if (quit_rspd == 999)
            error_text = ERROR_TEXT_FOR_2;
        else if (quit_rspd == 1) { 
            error_text = "";
            state.gameReason = "Quit";
            break;}
        else if (quit_rspd == 2){
            error_text = "";
            cout << "Game Continues...";
            waitForEnter();
            break;}
    }
}

// Input validation for Resign Game (inside game loop)
void resignConfirmation(gameState& state) {
    string error_text = "";
    int resign_rspd;
    while (true){ 
        clearScreen();
        displayPlayView(state);
        cout << "Sure to Resign? \n1.Yes\n2.No";
        resign_rspd = getChoice(1,2, error_text);
        if (resign_rspd == 999)
            error_text = ERROR_TEXT_FOR_2;
        else if (resign_rspd == 1) { 
            error_text = "";
            state.winner = (state.isWhiteTurn ? state.blackPlayer : state.whitePlayer);
            state.gameReason = "Resigned";
            break;}
        else if (resign_rspd == 2){
            error_text = "";
            cout << "Resign Canceled...";
            waitForEnter();
            break;}
    }
}

// Input validation for Undo Game (inside game loop)
void undoConfirmation(gameState& state) {
    string error_text = "";
    int undo_rspd;
    while (true){ 
        clearScreen();
        displayPlayView(state);
        cout << "Sure to Undo? \n1.Yes\n2.No";
        undo_rspd = getChoice(1,2, error_text);
        if (undo_rspd == 999)
            error_text = ERROR_TEXT_FOR_2;
        else if (undo_rspd == 1) {
            error_text = "";
            undoMove(state);
            break;}
        else if (undo_rspd == 2) {
            error_text = "";
            cout << "Undo Canceled...";
            waitForEnter();
            break;}
    }
}

// Input validation for Draw Game (inside game loop)
void drawConfirmation(gameState& state) {
    string error_text = "";
    int draw_rspd;
    int step = 0;
    string player1 = (state.isWhiteTurn ? state.whitePlayer : state.blackPlayer);
    string player2 = (state.isWhiteTurn ? state.blackPlayer : state.whitePlayer);
    while (true){ // loop for draw confirmation
        if (step%2 == 0){
            clearScreen();
            displayPlayView(state);
            step ++;
        }
        if (step==1){
            cout << player1 << ": Sure to Draw? \n1.Yes\n2.No";
            draw_rspd = getChoice(1,2, error_text);
            if (draw_rspd == 999){
                error_text = ERROR_TEXT_FOR_2;
                step -= 1;}
            else if (draw_rspd == 1) {
                error_text = "";
                step ++;}
            else if (draw_rspd == 2){
                error_text = "";
                cout << "Draw Canceled...";
                waitForEnter();
                break;}
        }
        if (step==3){
            cout << player2 << ": Sure to Draw? \n1.Yes\n2.No";
            int draw_rspd = getChoice(1,2, error_text);
            if (draw_rspd == 999){
                error_text = ERROR_TEXT_FOR_2;
                step -= 1;}
            else if (draw_rspd == 1) {
                error_text = "";
                state.winner = "None";
                state.gameReason = "Drawed";
                break;}
            else if (draw_rspd == 2){
                error_text = "";
                cout << player2 << " refused to draw:\n"<< "Draw Canceled...";
                waitForEnter();
                break;}
        }
    }
}

//NEEDDO// Move cursor printed on board
void moveCursor(gameState& state, char key){
    key = toupper(key);
    int& cursorCol = state.curserPos[0];
    int& cursorRow = state.curserPos[1];

    switch(key)
    {
        case 72 : // numbers are ASCII codes for arrow keys that follows after lead byte
        case 'W':
            if(cursorRow < 7)
                cursorRow++;
            break;

        case 80 :
        case 'S':
            if(cursorRow > 0)
                cursorRow--;
            break;

        case 75 :
        case 'A':
            if(cursorCol > 0)
                cursorCol--;
            break;

        case 77 :
        case 'D':
            if(cursorCol < 7)
                cursorCol++;
            break;
    }
}

//NEEDDO// Loop when moving a selected piece
void makeMove(gameState& state) {

    char current_color = (state.isWhiteTurn ? 'w' : 'b');
    char next_color = (state.isWhiteTurn ? 'b' : 'w');
    //record selected cell (which is lastest cursor position), and show all legal moves
    Cell selected_cell = state.board[state.curserPos[0]][state.curserPos[1]];
    auto move_state = showAvailableMoves(state, selected_cell); 

    //checks if there are any moves for selected piece
    bool pieceHasMoves;
    for (auto subboard : move_state.board) {
        for (auto cell : subboard) {
            if (pieceHasMoves = cell.hasMoves) break;
        }
        if (pieceHasMoves) break;
    }

    unsigned char move_rspd; // inputs in loop
    bool chosen = false; // loop bools
    bool moving = true; 
    bool promoting = false;
    while (moving) { // choosing cell loop

        clearScreen();
        displayPlayView(move_state, true);
        cout << "Selected >> " << selected_cell.cellName << " : " << selected_cell.pieceName;
        if (!pieceHasMoves) cout << " [NO MOVES]";
        cout << '\n';
        cout << string(VIEW_WIDTH, '-') << '\n';
        move_rspd = _getch();
        move_rspd = toupper(move_rspd);

        switch (move_rspd) { //input accept loop
            case 'C':
                moving=false;
                break;

            case 224: // detecting lead byte for arrow keys
                move_rspd = _getch(); // striping arrow keys input after the lead byte
            case 'W':
            case 'A':
            case 'S':
            case 'D':
                moveCursor(move_state, move_rspd);
                break;

            case '\r': // this is the input from _getch() when 'enter' is pressed (ASCII code 13)
            case 'E': 
                if (move_state.board[move_state.curserPos[0]][move_state.curserPos[1]].hasMoves){
                    moving = false;
                    chosen = true;}
                break;
        }
    }
    if (chosen) { // move chosen piece (promote pawn if available)
        Cell move_cell = move_state.board[move_state.curserPos[0]][move_state.curserPos[1]];

        executeMove(state, selected_cell.coordinate, move_state.curserPos);

        string promo_error = "";
        promoting = isPawnPromotion(state, move_state.curserPos);
        while (promoting) {
            clearScreen();
            displayPlayView(state);
            cout << "WOW! Promotion!:\n"
                << "1.Queen\n"
                << "2.Bishop\n"
                << "3.Knight\n"
                << "4.Rook\n"
                << "5.No Promotion\n"
                << "Please choose a piece to promote to (1~5)";
            int promo_code = getChoice(1, 5, promo_error);
            if (promo_code == 999) {
                promo_error = "-- Please Enter Number Between 1~5 --";
                continue;}
            promotePawn(state, move_state.curserPos, promo_code);
            promo_error = "";
            promoting = false;
            break;
        }
        //record history move
        char pieceName = (selected_cell.pieceName=="Knight" ? 'N' : selected_cell.pieceName[0]);
        state.moveHistory += to_string(state.moveCount) + pieceName + selected_cell.cellName + move_cell.cellName + ' ';
        
        //checking next player's states
        state.isInCheck = isInCheck(state, next_color);
        if (!hasLegalMoves(state, next_color)){
            if (state.isInCheck) { // checkmate checking
                state.winner = (state.isWhiteTurn ?  state.whitePlayer : state.blackPlayer);
                state.gameReason = "Checkmated";
            }
            else { // stalemate checking
                state.winner = "None";
                state.gameReason = "Stalemated";
            }
        }
        if (state.gameReason==""){
            state.moveCount ++;
            state.isWhiteTurn = !state.isWhiteTurn;
        }
        //save gameState
        saveStateToFile(state, state.fileName);
    }
    state.curserPos[0] = move_state.curserPos[0];
    state.curserPos[1] = move_state.curserPos[1];
}

//NEEDDO// Loop when playing game (seleceting piece)
void gameLoop(gameState& state) {

    unsigned char game_rspd;
    while (state.gameReason == "")
    {
        clearScreen();
        displayPlayView(state);
        game_rspd = _getch();
        game_rspd = toupper(game_rspd);

        switch (game_rspd) {

            case 224: // detecting lead byte for arrow keys
                game_rspd = _getch(); // striping arrow keys input after the lead byte
            case 'W': // movement inputs
            case 'A':
            case 'S':
            case 'D':
                moveCursor(state, game_rspd);
                break;

            case '\r': // this is the input from _getch() when 'enter' is pressed (ASCII code 13)
            case 'E':{ // select cell
                Cell cursur_cell = state.board[state.curserPos[0]][state.curserPos[1]];
                if (state.isWhiteTurn&&cursur_cell.cellOwner=='w' || !state.isWhiteTurn&&cursur_cell.cellOwner=='b')
                    makeMove(state); 
                
                break;}

            case 'Q':  // Quit game
                quitConfirmation(state);
                break;

            case 'X':  // resign
                resignConfirmation(state);
                break;

            case 'U': // undo
                undoConfirmation(state);
                break;

            case 'P': // draw
                drawConfirmation(state);
                break;
        }
    }
    // game Finished, printing last move
    if (state.gameReason=="Checkmated" || state.gameReason=="Stalemated") {
        clearScreen();
        displayPlayView(state);
        waitForEnter();
        if (state.gameReason=="Checkmated")
            winnerScreen(state);
        else if (state.gameReason=="Stalemated")
            drawScreen(state, true);
    }
    else if (state.gameReason=="Drawed") {
        saveStateToFile(state, state.fileName);
        drawScreen(state);
    }
    else if (state.gameReason=="Resigned") {
        saveStateToFile(state, state.fileName);
        resignScreen(state);
    }
    else if (state.gameReason=="Quit")
        messageBox("Game Saved", "Returning to Main Menu...");
    
  

    //go back to main menu
}

// Continue Previous Games
void continueGame() {
    gameState state;
    int choice;
    int maximum;

    string error_text;
    int step = 0;
    while (true) { // loop for game selection

        if (step%2 == 0) {
            clearScreen();
            maximum = showAllFile();
            cout << string(50, '-')<<'\n';
            step++;
        }

        if (step == 1) {
            cout << "Choose a previous game to resume (eg. 1, 2, 3)\n";
            cout << "Enter 0 to Exit";
            choice = getChoice(0, maximum, error_text);
            if (choice == 999){
                error_text = "-- Please Only Enter Number from 0 to " + to_string(maximum) + " --";
                step--;
                continue;
            }
            else if (choice == 0)
                return;

            error_text.clear();
            //extract gameState from file, returns to main menu if error wrong
            string fileResume = readFileName(choice);
            if (!loadLatestGameState(state, fileResume)){
                cout << "Error: Failed to load saved game\n";
                cout << "Returning to Main Menu...";
                waitForEnter();
                return;
            }
            step++;
        }     

        if (step == 3) {
            //see if game was finished because checkmated/stalemated, ask if want to undo and continue game
            if (state.gameReason!="") {
                if (state.gameReason=="Checkmated" || state.gameReason=="Stalemated") {
                    cout << "\nThis game was finished: " << state.gameReason << "\n";
                    cout << "Undo to right before getting " << state.gameReason << " and continue game?\n1.Yes\n2.No";
                }
                else if (state.gameReason == "Resigned") {
                    string resignee = (state.isWhiteTurn ? "White" : "Black");
                    cout << "\nThis game was finished: " << resignee + " Resigned \n";
                    cout << "Resume Game?\n1.Yes\n2.No";
                }
                else {
                    cout << "\nThis game was finished: Drawed\n";
                    cout << "Resume Game?\n1.Yes\n2.No";
                }

                choice = getChoice(1, 2, error_text);
                if (choice == 999) {
                    error_text = ERROR_TEXT_FOR_2;
                    step--;
                    continue;
                }
                else if (choice == 1) {
                    undoMove(state);
                    error_text.clear();
                    step += 2;}
                else if (choice == 2) {
                    error_text.clear();
                    step = 0;
                    continue;}
            }
            else 
                step += 2;
        }

        if (step == 5){
            cout << "\nLoading Game...\n";
            cout << "Press Enter to Continue Game...";
            waitForEnter();
            break;
        }
    }

    gameLoop(state);
}

// Sets up and runs a new game
void startNewGame() {
    string whitePlayer;
    string blackPlayer;
    gameState new_state = getNewGameState();

    int step = 0;
    while (true) {
        clearScreen();
        if (step%2==0){
            cout << string(VIEW_WIDTH, '-') << '\n';
            cout << centeredString("START NEW GAME", VIEW_WIDTH) << '\n';
            cout << string(VIEW_WIDTH, '-') << '\n';
            cout << centeredString("(Enter \"\\QUIT\" to quit)", VIEW_WIDTH) << "\n\n";
            step++;
        }
        if (step == 1){
            cout<<"White Player Name : ";
            getline(cin,whitePlayer);
            if (whitePlayer == "\\QUIT")
                return;
            if (hasSpace(whitePlayer)){
                cout << "-- No Spaces are allowed in name --\n";
                cout << "Please reenter your name...";
                waitForEnter();
                step--;}
            else if (hasPunct(whitePlayer)){
                cout << "-- No Punctuations are allowed in name --\n";
                cout << "Please reenter your name...";
                waitForEnter();
                step--;}
            else 
                step++;
        }
        if (step == 3){
            cout<<"Black Player Name : ";
            getline(cin,blackPlayer);
            if (blackPlayer == "\\QUIT")
                return;
            if (hasSpace(blackPlayer)){
                cout << "-- No Spaces are allowed in name --\n";
                cout << "Please reenter your name...";
                waitForEnter();
                step--;}
            else if (hasPunct(blackPlayer)){
                cout << "-- No Punctuations are allowed in name --\n";
                cout << "Please reenter your name...";
                waitForEnter();
                step--;}
            else 
                step++;
        }
        if (step == 4)
                break;
    }

    new_state.whitePlayer = whitePlayer;
    new_state.blackPlayer = blackPlayer;
    string file_name = saveFileToMaster(whitePlayer, blackPlayer);
    new_state.fileName = file_name;
    saveStateToFile(new_state, file_name);
    new_state.moveCount ++;
    
    gameLoop(new_state);
}


