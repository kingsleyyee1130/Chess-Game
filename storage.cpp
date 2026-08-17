#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <algorithm>
#include "chessBoardList.h"
#include "gameLogic.h"

using namespace std;

vector<gameState> history;

// ===== CONVERT CELL TO STRING =====
string cellToString(const Cell& cell) {
    stringstream ss;
    ss << "{";
    ss << "\"pieceName\":\"" << cell.pieceName << "\",";
    ss << "\"pieceSymbol\":\"" << cell.pieceSymbol << "\",";
    ss << "\"cellName\":\"" << cell.cellName << "\",";
    ss << "\"cellOwner\":\"" << cell.cellOwner << "\",";
    ss << "\"isEmpty\":" << (cell.isEmpty ? "true" : "false") << ",";
    ss << "\"hasMoves\":" << (cell.hasMoves ? "true" : "false");
    ss << "}";
    return ss.str();
}

// ===== CONVERT BOARD TO STRING =====
string boardToString(const boardArray_t& board) {
    stringstream ss;
    ss << "[";
    for (int row = 0; row < 8; row++) {
        ss << "[";
        for (int col = 0; col < 8; col++) {
            ss << cellToString(board[row][col]);
            if (col < 7) ss << ",";
        }
        ss << "]";
        if (row < 7) ss << ",";
    }
    ss << "]";
    return ss.str();
}

// ===== CONVERT GAME STATE TO STRING =====
string gameStateToString(const gameState& state) {
    stringstream ss;
    ss << "{";
    ss << "\"moveCount\":" << state.moveCount << ",";
    ss << "\"isWhiteTurn\":" << (state.isWhiteTurn ? "true" : "false") << ",";
    ss << "\"enPassantTarget\":\"" << state.enPassantTarget << "\",";
    ss << "\"isInCheck\":" << (state.isInCheck ? "true" : "false") << ",";
    ss << "\"whiteCanCastleKS\":" << (state.whiteCanCastleKS ? "true" : "false") << ",";
    ss << "\"whiteCanCastleQS\":" << (state.whiteCanCastleQS ? "true" : "false") << ",";
    ss << "\"blackCanCastleKS\":" << (state.blackCanCastleKS ? "true" : "false") << ",";
    ss << "\"blackCanCastleQS\":" << (state.blackCanCastleQS ? "true" : "false") << ",";
    ss << "\"moveHistory\":\"" << state.moveHistory << "\",";
    ss << "\"board\":" << boardToString(state.board);
    ss << "}";
    return ss.str();
}

// ===== PARSE CELL =====
Cell parseCell(const string& str) {
    Cell cell;

    auto getValue = [&](const string& key) -> string {
        size_t pos = str.find("\"" + key + "\":");
        if (pos == string::npos) return "";
        pos = str.find(":", pos) + 1;
        if (str[pos] == '"') {
            pos++;
            size_t end = str.find("\"", pos);
            return str.substr(pos, end - pos);
        }
        else {
            size_t end = str.find_first_of(",}", pos);
            string val = str.substr(pos, end - pos);
            val.erase(0, val.find_first_not_of(" \t\n\r"));
            val.erase(val.find_last_not_of(" \t\n\r") + 1);
            return val;
        }
        };

    cell.pieceName = getValue("pieceName");
    cell.pieceSymbol = getValue("pieceSymbol");
    cell.cellName = getValue("cellName");
    string owner = getValue("cellOwner");
    cell.cellOwner = owner.empty() ? 'n' : owner[0];
    cell.isEmpty = (getValue("isEmpty") == "true");
    cell.hasMoves = (getValue("hasMoves") == "true");

    return cell;
}

// ===== PARSE GAME STATE =====
gameState parseGameState(const string& json) {
    gameState state;

    // Remove all whitespace for easier parsing
    string cleanJson = json;
    cleanJson.erase(remove_if(cleanJson.begin(), cleanJson.end(), ::isspace), cleanJson.end());

    auto getValue = [&](const string& key) -> string {
        string searchKey = "\"" + key + "\":";
        size_t pos = cleanJson.find(searchKey);
        if (pos == string::npos) return "";

        pos += searchKey.length();

        if (cleanJson[pos] == '"') {
            pos++;
            size_t end = cleanJson.find("\"", pos);
            if (end == string::npos) return "";
            return cleanJson.substr(pos, end - pos);
        }
        else {
            size_t end = cleanJson.find_first_of(",}", pos);
            if (end == string::npos) end = cleanJson.length();
            string val = cleanJson.substr(pos, end - pos);
            return val;
        }
        };

    string moveStr = getValue("moveCount");
    if (!moveStr.empty()) state.moveCount = stoi(moveStr);

    state.isWhiteTurn = (getValue("isWhiteTurn") == "true");
    state.isInCheck = (getValue("isInCheck") == "true");
    state.enPassantTarget = getValue("enPassantTarget");
    state.moveHistory = getValue("movHistory");
    state.whiteCanCastleKS = (getValue("whiteCanCastleKS") == "true");
    state.whiteCanCastleQS = (getValue("whiteCanCastleQS") == "true");
    state.blackCanCastleKS = (getValue("blackCanCastleKS") == "true");
    state.blackCanCastleQS = (getValue("blackCanCastleQS") == "true");

    // Parse board
    size_t boardStart = cleanJson.find("\"board\":[");
    if (boardStart != string::npos) {
        boardStart = cleanJson.find("[", boardStart + 8);
        if (boardStart != string::npos) {
            int bracketCount = 0;
            size_t boardEnd = boardStart;
            for (size_t i = boardStart; i < cleanJson.length(); ++i) {
                if (cleanJson[i] == '[') bracketCount++;
                if (cleanJson[i] == ']') {
                    bracketCount--;
                    if (bracketCount == 0) {
                        boardEnd = i;
                        break;
                    }
                }
            }

            if (boardEnd > boardStart) {
                string boardStr = cleanJson.substr(boardStart, boardEnd - boardStart + 1);

                int row = 0;
                size_t pos = boardStr.find("[");
                while (row < 8 && pos != string::npos) {
                    pos = boardStr.find("[", pos + 1);
                    if (pos == string::npos) break;

                    int rowBracketCount = 0;
                    size_t rowEnd = pos;
                    for (size_t i = pos; i < boardStr.length(); ++i) {
                        if (boardStr[i] == '[') rowBracketCount++;
                        if (boardStr[i] == ']') {
                            rowBracketCount--;
                            if (rowBracketCount == 0) {
                                rowEnd = i;
                                break;
                            }
                        }
                    }

                    if (rowEnd > pos) {
                        string rowStr = boardStr.substr(pos, rowEnd - pos + 1);

                        int col = 0;
                        size_t cellPos = rowStr.find("{");
                        while (col < 8 && cellPos != string::npos) {
                            int braceCount = 0;
                            size_t cellEnd = cellPos;
                            for (size_t i = cellPos; i < rowStr.length(); ++i) {
                                if (rowStr[i] == '{') braceCount++;
                                if (rowStr[i] == '}') {
                                    braceCount--;
                                    if (braceCount == 0) {
                                        cellEnd = i;
                                        break;
                                    }
                                }
                            }

                            if (cellEnd > cellPos) {
                                string cellStr = rowStr.substr(cellPos, cellEnd - cellPos + 1);
                                state.board[row][col] = parseCell(cellStr);
                            }

                            cellPos = rowStr.find("{", cellEnd + 1);
                            col++;
                        }
                    }
                    row++;
                }
            }
        }
    }

    return state;
}

// ===== 1. SAVE GAME STATE (OVERWRITE) =====
void saveGameState(const gameState& state, const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        file << gameStateToString(state);
        file.close();
        cout << "Game saved to " << filename << endl;
    }
    else {
        cout << "Error: Cannot save to " << filename << endl;
    }
}

// ===== 2. SAVE MOVE TO HISTORY (APPEND) =====
void saveMoveToHistory(const gameState& state, const string& filename) {
    ofstream file(filename, ios::app);
    if (file.is_open()) {
        file << gameStateToString(state) << "\n";
        file.close();
        cout << "Move " << state.moveCount << " saved to history" << endl;
    }
    else {
        cout << "Error: Cannot save to " << filename << endl;
    }
}

// ===== 3. LOAD LATEST GAME STATE (FOR UNDO) =====
bool loadLatestGameState(gameState& state, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Cannot open " << filename << endl;
        return false;
    }

    vector<string> lines;
    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }
    file.close();

    if (lines.empty()) {
        cout << "No data found!" << endl;
        return false;
    }

    state = parseGameState(lines.back());
    cout << "Loaded latest move #" << state.moveCount << " from " << filename << endl;
    return true;
}

// ===== 4. LOAD SPECIFIC MOVE FROM HISTORY =====
bool loadMoveFromHistory(gameState& state, const string& filename, int moveNumber) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Cannot open " << filename << endl;
        return false;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        // Extract move number
        size_t numPos = line.find("\"moveCount\"");
        if (numPos == string::npos) continue;

        numPos = line.find(":", numPos);
        if (numPos == string::npos) continue;
        numPos++;

        while (numPos < line.length() && (line[numPos] == ' ' || line[numPos] == '\t')) {
            numPos++;
        }

        string numStr;
        while (numPos < line.length() && (isdigit(line[numPos]) || line[numPos] == '-')) {
            numStr += line[numPos];
            numPos++;
        }

        if (!numStr.empty()) {
            int foundMove = stoi(numStr);
            if (foundMove == moveNumber) {
                state = parseGameState(line);
                cout << "Loaded move " << moveNumber << " from " << filename << endl;
                return true;
            }
        }
    }
    file.close();

    cout << "Move " << moveNumber << " not found!" << endl;
    return false;
}

// ===== 5. DELETE LAST MOVE FROM FILE (FOR UNDO) =====
bool deleteLastMoveFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Cannot open " << filename << endl;
        return false;
    }

    vector<string> lines;
    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }
    file.close();

    if (lines.empty()) {
        cout << "No moves to delete!" << endl;
        return false;
    }

    // Remove the last line
    lines.pop_back();

    // Write back all lines except the last one
    ofstream outFile(filename, ios::trunc);
    if (!outFile.is_open()) {
        cout << "Error: Cannot write to " << filename << endl;
        return false;
    }

    for (const string& l : lines) {
        outFile << l << "\n";
    }
    outFile.close();

    cout << "Last move deleted. " << lines.size() << " moves remaining." << endl;
    return true;
}

// ===== 6. GET TOTAL MOVES IN FILE =====
int getTotalMovesInFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return 0;
    }

    int count = 0;
    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            count++;
        }
    }
    file.close();

    return count;
}

// ===== 7. CLEAR HISTORY FILE =====
void clearHistory(const string& filename) {
    ofstream file(filename, ios::trunc);
    file.close();
    cout << "History cleared!" << endl;
}

// ===== 8. RECORD STATE (BEFORE MOVE) =====
void recordState(gameState& state, vector<gameState>& history) {
    history.push_back(state);
    cout << "State recorded (move " << state.moveCount << ")" << endl;
}

// ===== 9. UNDO MOVE (WITH FILE DELETION) =====
bool undoMove(gameState& state, vector<gameState>& history, const string& filename) {
    if (history.empty()) {
        cout << "No moves to undo!" << endl;
        return false;
    }

    // Get the previous state from history
    state = history.back();
    history.pop_back();

    // Delete the last move from the file
    deleteLastMoveFromFile(filename);

    cout << "Undo successful! Now at move " << state.moveCount << endl;
    return true;
}

// ===== 10. DISPLAY BOARD =====
void displayBoard(const boardArray_t& board) {
    cout << "  a b c d e f g h" << endl;
    for (int row = 0; row < 8; row++) {
        cout << 8 - row << " ";
        for (int col = 0; col < 8; col++) {
            if (board[row][col].isEmpty) {
                cout << ". ";
            }
            else {
                cout << board[row][col].pieceSymbol << " ";
            }
        }
        cout << 8 - row << endl;
    }
    cout << "  a b c d e f g h" << endl;
}

// ===== 11. DISPLAY GAME STATE =====
void displayGameState(const gameState& state) {
    cout << "\n=== GAME STATE ===" << endl;
    cout << "Move: " << state.moveCount << endl;
    cout << "Turn: " << (state.isWhiteTurn ? "White" : "Black") << endl;
    cout << "Move History: " << state.moveHistory << endl;
    cout << "En Passant: " << state.enPassantTarget << endl;
    cout << "Castling: WK=" << (state.whiteCanCastleKS ? "Y" : "N")
        << " WQ=" << (state.whiteCanCastleQS ? "Y" : "N")
        << " BK=" << (state.blackCanCastleKS ? "Y" : "N")
        << " BQ=" << (state.blackCanCastleQS ? "Y" : "N") << endl;
    displayBoard(state.board);
}

// ===== 12. DISPLAY FILE CONTENTS =====
void displayFileContents(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "File not found or empty!" << endl;
        return;
    }

    cout << "\n=== FILE CONTENTS (" << filename << ") ===" << endl;
    string line;
    int lineNum = 1;
    while (getline(file, line)) {
        if (!line.empty()) {
            // Extract move number for display
            size_t numPos = line.find("\"moveCount\"");
            if (numPos != string::npos) {
                numPos = line.find(":", numPos);
                if (numPos != string::npos) {
                    numPos++;
                    while (numPos < line.length() && (line[numPos] == ' ' || line[numPos] == '\t')) {
                        numPos++;
                    }
                    string numStr;
                    while (numPos < line.length() && isdigit(line[numPos])) {
                        numStr += line[numPos];
                        numPos++;
                    }
                    cout << "Line " << lineNum << ": Move #" << numStr << endl;
                }
            }
        }
        lineNum++;
    }
    file.close();
    cout << "Total lines: " << lineNum - 1 << endl;
}

// Load specific move
void viewHistory(const string& filename){
    int historyNum;
    cout << "which move you want to view? ";
    cin >> historyNum;
    gameState move2State;
    loadMoveFromHistory(move2State, filename, historyNum);
}
