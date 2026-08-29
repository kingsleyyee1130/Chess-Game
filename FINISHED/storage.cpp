/*
1. gameStateToString: added new gameState attributes
2. cellToString : added new Cell attributes
3. parseGameState:
4. parseCell
*/
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <array>


#include "storage.h"

using namespace std;

const string MASTER_FILE_NAME = "File_of_file.txt";

// ===== HELPER: strip whitespace, but never inside a quoted JSON string =====
// (the previous version stripped ALL whitespace including inside quotes,
// which silently corrupted any string field containing a space --
// e.g. moveHistory ("White moved Pawn to e4") or player names with spaces)
string stripWhitespaceOutsideQuotes(const string& input) {
    string result;
    bool inQuotes = false;
    for (char c : input) {
        if (c == '"') inQuotes = !inQuotes;
        if (!inQuotes && isspace(static_cast<unsigned char>(c))) continue;
        result += c;
    }
    return result;
}

// ===== HELPER: parse a 2-element integer array field like "key":[a,b] =====
// used for both Cell::coordinate and gameState::curserPos
void parseIntPair(const string& src, const string& key, int (&arr)[2]) {
    size_t pos = src.find("\"" + key + "\":[");
    if (pos == string::npos) return; // leave existing/default values if the field is absent
    pos = src.find('[', pos) + 1;
    size_t end = src.find(']', pos);
    if (end == string::npos) return;
    string inner = src.substr(pos, end - pos);
    size_t comma = inner.find(',');
    if (comma == string::npos) return;
    try {
        arr[0] = stoi(inner.substr(0, comma));
        arr[1] = stoi(inner.substr(comma + 1));
    } catch (...) {
        // malformed value -- leave arr untouched rather than throwing
    }
}

// ===== HELPER: find the closing bracket/brace matching the one at openPos =====
// Scans forward from an opening bracket/brace, tracking nesting depth, but
// -- critically -- ignoring any bracket/brace characters that appear INSIDE
// a quoted string. Without this, a pieceSymbol value containing a raw
// bracket character (e.g. an ANSI color escape like "\033[38;5;230m", which
// contains an unmatched '[' with no matching ']') desyncs a naive counter
// permanently, since it never returns to depth 0. That's exactly what was
// making the whole board silently fail to parse once piece symbols started
// carrying color codes.
size_t findMatchingDelim(const string& src, size_t openPos, char openChar, char closeChar) {
    int depth = 0;
    bool inQuotes = false;
    for (size_t i = openPos; i < src.length(); ++i) {
        char c = src[i];
        if (c == '"') { inQuotes = !inQuotes; continue; }
        if (inQuotes) continue; // brackets/braces inside a quoted value are just data
        if (c == openChar) depth++;
        else if (c == closeChar) {
            depth--;
            if (depth == 0) return i;
        }
    }
    return string::npos;
}

// ===== CONVERT CELL TO STRING =====
string cellToString(const Cell& cell) {
    stringstream ss;
    ss << "{";
    ss << "\"pieceName\":\"" << cell.pieceName << "\",";
    ss << "\"pieceSymbol\":\"" << cell.pieceSymbol << "\",";
    ss << "\"cellName\":\"" << cell.cellName << "\",";
    ss << "\"cellOwner\":\"" << cell.cellOwner << "\",";
    ss << "\"isEmpty\":" << (cell.isEmpty ? "true" : "false") << ",";
    ss << "\"hasMoves\":" << (cell.hasMoves ? "true" : "false") << ",";
    ss << "\"coordinate\":[" << cell.coordinate[0] << "," << cell.coordinate[1] << "]";
    ss << "}";
    return ss.str();
}

// ===== CONVERT BOARD TO STRING =====
string boardToString(const boardArray_t& board) {
    stringstream ss;
    ss << "[";
    for (int x = 0; x < 8; x++) {
        ss << "[";
        for (int y = 0; y < 8; y++) {
            ss << cellToString(board[x][y]);
            if (y < 7) ss << ",";
        }
        ss << "]";
        if (x < 7) ss << ",";
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
    ss << "\"isInCheck\":" << (state.isInCheck ? "true" : "false") << ",";
    ss << "\"enPassantTarget\":\"" << state.enPassantTarget << "\",";
    ss << "\"whiteCanCastleKS\":" << (state.whiteCanCastleKS ? "true" : "false") << ",";
    ss << "\"whiteCanCastleQS\":" << (state.whiteCanCastleQS ? "true" : "false") << ",";
    ss << "\"blackCanCastleKS\":" << (state.blackCanCastleKS ? "true" : "false") << ",";
    ss << "\"blackCanCastleQS\":" << (state.blackCanCastleQS ? "true" : "false") << ",";
    ss << "\"curserPos\":[" << state.curserPos[0] << "," << state.curserPos[1] << "],";
    ss << "\"moveHistory\":\"" << state.moveHistory << "\",";
    ss << "\"fileName\":\"" << state.fileName << "\",";
    ss << "\"whitePlayer\":\"" << state.whitePlayer << "\",";
    ss << "\"blackPlayer\":\"" << state.blackPlayer << "\",";
    ss << "\"winner\":\"" << state.winner << "\",";
    ss << "\"gameReason\":\"" << state.gameReason << "\",";
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
    parseIntPair(str, "coordinate", cell.coordinate);

    return cell;
}

// ===== PARSE GAME STATE =====
gameState parseGameState(const string& json) {
    gameState state;

    // Strip whitespace OUTSIDE quoted strings only -- stripping it
    // everywhere (as before) corrupted any string value containing a
    // space, such as moveHistory or player names.
    string cleanJson = stripWhitespaceOutsideQuotes(json);

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
    state.moveHistory = getValue("moveHistory");
    state.whiteCanCastleKS = (getValue("whiteCanCastleKS") == "true");
    state.whiteCanCastleQS = (getValue("whiteCanCastleQS") == "true");
    state.blackCanCastleKS = (getValue("blackCanCastleKS") == "true");
    state.blackCanCastleQS = (getValue("blackCanCastleQS") == "true");
    parseIntPair(cleanJson, "curserPos", state.curserPos);

    // string fields added to gameState -- fall back to the
    // default-constructed value if a field is missing (e.g. loading a
    // save written before these fields existed)
    string fileNameVal = getValue("fileName");
    if (!fileNameVal.empty()) state.fileName = fileNameVal;
    string whitePlayerVal = getValue("whitePlayer");
    if (!whitePlayerVal.empty()) state.whitePlayer = whitePlayerVal;
    string blackPlayerVal = getValue("blackPlayer");
    if (!blackPlayerVal.empty()) state.blackPlayer = blackPlayerVal;
    string winnerVal = getValue("winner");
    if (!winnerVal.empty()) state.winner = winnerVal;
    state.gameReason = getValue("gameReason");

    // Parse board
    size_t boardStart = cleanJson.find("\"board\":[");
    if (boardStart != string::npos) {
        boardStart = cleanJson.find("[", boardStart + 8);
        if (boardStart != string::npos) {
            size_t boardEnd = findMatchingDelim(cleanJson, boardStart, '[', ']');

            if (boardEnd != string::npos && boardEnd > boardStart) {
                string boardStr = cleanJson.substr(boardStart, boardEnd - boardStart + 1);

                int x = 0;
                size_t pos = boardStr.find("[");
                while (x < 8 && pos != string::npos) {
                    pos = boardStr.find("[", pos + 1);
                    if (pos == string::npos) break;

                    size_t rowEnd = findMatchingDelim(boardStr, pos, '[', ']');
                    if (rowEnd == string::npos) break;

                    if (rowEnd > pos) {
                        string rowStr = boardStr.substr(pos, rowEnd - pos + 1);

                        int y = 0;
                        size_t cellPos = rowStr.find("{");
                        while (y < 8 && cellPos != string::npos) {
                            size_t cellEnd = findMatchingDelim(rowStr, cellPos, '{', '}');
                            if (cellEnd == string::npos) break;

                            if (cellEnd > cellPos) {
                                string cellStr = rowStr.substr(cellPos, cellEnd - cellPos + 1);
                                state.board[x][y] = parseCell(cellStr);
                            }

                            cellPos = rowStr.find("{", cellEnd + 1);
                            y++;
                        }
                    }
                    x++;
                    // Advance the search position to AFTER this row's own
                    // closing bracket before hunting for the next "[".
                    // Without this, the search would find the FIRST "["
                    // anywhere after the current row's *opening* bracket --
                    // which, now that each Cell carries a nested
                    // "coordinate":[x,y] array, is almost always one of
                    // those nested brackets rather than the next row's
                    // bracket. That desynced parsing for every row after
                    // the first as soon as "coordinate" was added.
                    pos = rowEnd;
                }
            }
        }
    }

    return state;
}

// ===== 1. SAVE & RETURNS File Name (Into Master File_of_file) =====
string saveFileToMaster(string whitePlayer, string blackPlayer) {
    string filename = whitePlayer + "_VS_" + blackPlayer + ".txt";
    string checkname = filename;
    string saved_name;
    int same_name = 0;

    // scan if name repeats
    ifstream ifile(MASTER_FILE_NAME);
    while (getline(ifile, saved_name)) {
        if (!saved_name.empty()) 
            if (saved_name == checkname) { // if name repeats, add (no.) at back of filename into checkname and check for that name in the rest names
                same_name++;
                checkname = filename.substr(0, filename.length()-4) + '(' + to_string(same_name) + ')' + ".txt";
            }
    }
    ifile.close();

    // if exist multiple same filenames, lastest filename = filename(no.+1)
    if (checkname != filename) {
        filename = filename.substr(0, filename.length()-4) + '(' + to_string(same_name) + ')' + ".txt";
    }

    // append new filename
    ofstream ofile(MASTER_FILE_NAME, ios::app);
    if (ofile.is_open()) {
        ofile << filename << "\n";
        ofile.close();
    }
    
    return filename;
}

// ===== 2. SAVE STATE TO FILE (APPEND AFTER EVERY MOVE) =====
void saveStateToFile(const gameState& state, const string& filename) {
    ofstream file(filename, ios::app);
    if (file.is_open()) {
        file << gameStateToString(state) << "\n";
        file.close();
        cout << "Move " << state.moveCount << " saved to history" << endl;
    }
}

// ===== 3. LOAD LATEST GAME STATE (FOR CONTINUE GAME & UNDO) =====
bool loadLatestGameState(gameState& state, const string& filename) {
    ifstream file(filename);

    vector<string> lines;
    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }

    if (!lines.empty()) {
        state = parseGameState(lines.back());
        return true;
    }

    return false;
}

// ===== 4. DELETE LAST STATE FROM FILE (FOR UNDO) =====
bool deleteLastStateFromFile(const string& filename) {
    ifstream file(filename);

    vector<string> lines;
    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }
    file.close();

    if (lines.empty()) {
        return false;
    }

    // Remove the last line
    lines.pop_back();

    // Write back all lines except the last one
    ofstream outFile(filename, ios::trunc);

    for (const string& l : lines) {
        outFile << l << "\n";
    }
    outFile.close();

    cout << "Undo successfull. " << lines.size() << " moves now." << endl;
    return true;
}

// ===== 5. LOAD SPECIFIC STATE FROM HISTORY =====
bool loadStateFromHistory(gameState& state, const string& filename, int moveNumber) {
    ifstream file(filename);

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
    return false; // move number not found in file
}

// ===== 6. GET TOTAL MOVES IN FILE =====
int getTotalMovesInFile(const string& filename) {
    ifstream file(filename);

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

// Undo to last gameState
bool undoMove(gameState& state) {
    
    if(deleteLastStateFromFile(state.fileName))
        if (loadLatestGameState(state, state.fileName))
            return true;

    return false;
}

// Load specific move
void viewHistory(const string& filename, gameState& state) {
    gameState move2State;
    for (int i = 1; i < state.moveCount; i++) {
        loadStateFromHistory(move2State, filename, i);
    }
}

// Direct read all file and display the choice
int showAllFile() {
    ifstream file(MASTER_FILE_NAME);
    string line;
    int i = 1;

    cout << '\n' << string(18, '=') << " GAME HISTORY " << string(18, '=') << "\n\n";
    while (getline(file, line)) {
        if (!line.empty()) {
            cout << std::setw(3) << i << ".  " << line.substr(0, line.length()-4) << endl;
            i++;
        }
    }
    file.close();
    cout << "\n\n";
    i--;
    return i;
}

// connect user choice to filename for game resume
// NOTE: choice is 1-indexed (matches the numbering printed by
// showAllFile()). The original loop used "i <= choice", which reads one
// line too many and returns the NEXT entry instead of the selected one
// (choice=1 returned the 2nd file, not the 1st). Changed to "i < choice"
// so exactly `choice` lines are read.
string readFileName(int choice) {
    ifstream file(MASTER_FILE_NAME);
    string line;
    string filename;

    for (int i = 0; i < choice; i++) {
        getline(file, line);
            if (!line.empty())
                filename = line;
    }
    file.close();

    return filename;
}
