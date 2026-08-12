#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <array>
#include <vector>
#include "chessBoardList.h"
using namespace std;

// need to have 
void displayBoard(const boardArray_t& board) {
    cout << "  a b c d e f g h" << endl;
    for (int row = 0; row < 8; ++row) {
        cout << 8 - row << " ";
        for (int col = 0; col < 8; ++col) {
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
}  // already have by Ej
boardArray_t board; // already have by th
int moveNumber = 0; // already have by vianna
string filename = "game_history.json"; // either alway save in a same file or save in different file (need update) 

// Clear file if it exists
void clearfile(string filename) {
    ofstream clearFile(filename, ios::trunc);
    clearFile.close();
}

// ===== CONVERT CELL TO JSON STRING ===== <<<< boardToJson call this to convert cell to string
string cellToJson(const Cell& cell, int indent = 0) {
    string spaces(indent * 2, ' ');
    stringstream ss;
    ss << "{\n";
    ss << spaces << "  \"pieceName\": \"" << cell.pieceName << "\",\n";
    ss << spaces << "  \"pieceSymbol\": \"" << cell.pieceSymbol << "\",\n";
    ss << spaces << "  \"cellName\": \"" << cell.cellName << "\",\n";
    ss << spaces << "  \"cellOwner\": \"" << cell.cellOwner << "\",\n";
    ss << spaces << "  \"isEmpty\": " << (cell.isEmpty ? "true" : "false") << ",\n";
    ss << spaces << "  \"hasMoves\": " << (cell.hasMoves ? "true" : "false") << "\n";
    ss << spaces << "}";
    return ss.str();
}

// ===== CONVERT BOARD TO JSON STRING ===== <<<< Saving file call this to convert array to string
string boardToJson(const boardArray_t& board, int indent = 0) {
    string spaces(indent * 2, ' ');
    stringstream ss;
    ss << "[\n";
    for (int row = 0; row < 8; ++row) {
        ss << spaces << "  [\n";
        for (int col = 0; col < 8; ++col) {
            ss << spaces << "    " << cellToJson(board[row][col], indent + 2);
            if (col < 7) ss << ",";
            ss << "\n";
        }
        ss << spaces << "  ]";
        if (row < 7) ss << ",";
        ss << "\n";
    }
    ss << spaces << "]";
    return ss.str();
}

// ===== SAVE BOARD STATE WITH MOVE NUMBER (APPEND) ===== <<<This is saving file
void saveBoardState(const boardArray_t& board, int moveNumber, const string& filename) {
    string jsonString = "{\n";
    jsonString += "  \"moveNumber\": " + to_string(moveNumber) + ",\n";
    jsonString += "  \"board\": " + boardToJson(board, 1) + "\n";
    jsonString += "}\n";  // Add newline to separate entries

    ofstream file(filename, ios::app);  // APPEND mode
    if (file.is_open()) {
        file << jsonString;
        file.close();
        cout << "Move " << moveNumber << " saved to " << filename << endl;
    }
    else {
        cerr << "Error: Could not open file " << filename << endl;
    }
}

// ===== PARSE A SINGLE CELL FROM JSON STRING =====
Cell parseCell(const string& cellStr) {
    Cell cell;

    // Helper lambda to extract value
    auto extractValue = [&](const string& key) -> string {
        size_t keyPos = cellStr.find("\"" + key + "\"");
        if (keyPos == string::npos) return "";

        keyPos = cellStr.find(":", keyPos);
        if (keyPos == string::npos) return "";
        keyPos++;

        // Skip whitespace
        while (keyPos < cellStr.length() && (cellStr[keyPos] == ' ' || cellStr[keyPos] == '\n' || cellStr[keyPos] == '\t')) {
            keyPos++;
        }

        if (cellStr[keyPos] == '"') {
            // String value
            keyPos++;
            size_t endPos = cellStr.find("\"", keyPos);
            if (endPos == string::npos) return "";
            return cellStr.substr(keyPos, endPos - keyPos);
        }
        else {
            // Boolean or number value
            size_t endPos = cellStr.find_first_of(",}\n", keyPos);
            if (endPos == string::npos) endPos = cellStr.length();
            string value = cellStr.substr(keyPos, endPos - keyPos);

            // Trim whitespace
            value.erase(0, value.find_first_not_of(" \t\n\r"));
            value.erase(value.find_last_not_of(" \t\n\r") + 1);
            return value;
        }
        };

    cell.pieceName = extractValue("pieceName");
    cell.pieceSymbol = extractValue("pieceSymbol");
    cell.cellName = extractValue("cellName");

    string owner = extractValue("cellOwner");
    cell.cellOwner = owner.empty() ? 'n' : owner[0];

    string isEmpty = extractValue("isEmpty");
    cell.isEmpty = (isEmpty == "true");

    string hasMoves = extractValue("hasMoves");
    cell.hasMoves = (hasMoves == "true");

    return cell;
}

// ===== READ LATEST BOARD STATE (FOR UNDO) =====
boardArray_t readLatestBoard(const string& filename) {
    boardArray_t board;

    // Initialize board with default values
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            board[row][col].cellName = string(1, 'a' + col) + to_string(8 - row);
            board[row][col].isEmpty = true;
            board[row][col].cellOwner = 'n';
        }
    }

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return board;
    }

    // Read entire file
    string content, line;
    while (getline(file, line)) {
        content += line;
        content += "\n";
    }
    file.close();

    cout << "DEBUG: File content length: " << content.length() << endl;

    // Find all JSON objects
    vector<string> jsonObjects;
    size_t pos = 0;
    while (pos < content.length()) {
        size_t start = content.find("{", pos);
        if (start == string::npos) break;

        // Find matching closing brace
        int braceCount = 0;
        size_t end = start;
        for (size_t i = start; i < content.length(); ++i) {
            if (content[i] == '{') braceCount++;
            if (content[i] == '}') {
                braceCount--;
                if (braceCount == 0) {
                    end = i;
                    break;
                }
            }
        }

        if (end > start) {
            jsonObjects.push_back(content.substr(start, end - start + 1));
        }
        pos = end + 1;
    }

    cout << "DEBUG: Found " << jsonObjects.size() << " JSON objects" << endl;

    if (jsonObjects.empty()) {
        cerr << "No board data found!" << endl;
        return board;
    }

    // Get the last JSON object
    string jsonStr = jsonObjects.back();
    cout << "DEBUG: Last JSON object length: " << jsonStr.length() << endl;

    // Extract move number
    int moveNumber = 0;
    size_t numPos = jsonStr.find("\"moveNumber\"");
    if (numPos != string::npos) {
        numPos = jsonStr.find(":", numPos) + 1;
        while (numPos < jsonStr.length() && (jsonStr[numPos] == ' ' || jsonStr[numPos] == '\n' || jsonStr[numPos] == '\t')) {
            numPos++;
        }
        string numStr;
        while (numPos < jsonStr.length() && (isdigit(jsonStr[numPos]) || jsonStr[numPos] == '-')) {
            numStr += jsonStr[numPos];
            numPos++;
        }
        if (!numStr.empty()) {
            moveNumber = stoi(numStr);
            cout << "Loading move #" << moveNumber << " for undo..." << endl;
        }
    }

    // Parse board data
    size_t boardPos = jsonStr.find("\"board\"");
    if (boardPos != string::npos) {
        boardPos = jsonStr.find("[", boardPos);
        if (boardPos != string::npos) {
            // Find the matching closing bracket
            int bracketCount = 0;
            size_t boardEnd = boardPos;
            for (size_t i = boardPos; i < jsonStr.length(); ++i) {
                if (jsonStr[i] == '[') bracketCount++;
                if (jsonStr[i] == ']') {
                    bracketCount--;
                    if (bracketCount == 0) {
                        boardEnd = i;
                        break;
                    }
                }
            }

            if (boardEnd > boardPos) {
                string boardStr = jsonStr.substr(boardPos, boardEnd - boardPos + 1);

                // Parse each row
                int row = 0;
                size_t rowPos = boardStr.find("[");
                while (row < 8 && rowPos != string::npos) {
                    rowPos = boardStr.find("[", rowPos + 1);
                    if (rowPos == string::npos) break;

                    // Find the end of this row
                    int rowBracketCount = 0;
                    size_t rowEnd = rowPos;
                    for (size_t i = rowPos; i < boardStr.length(); ++i) {
                        if (boardStr[i] == '[') rowBracketCount++;
                        if (boardStr[i] == ']') {
                            rowBracketCount--;
                            if (rowBracketCount == 0) {
                                rowEnd = i;
                                break;
                            }
                        }
                    }

                    if (rowEnd > rowPos) {
                        string rowStr = boardStr.substr(rowPos, rowEnd - rowPos + 1);

                        // Parse each cell in the row
                        int col = 0;
                        size_t cellPos = rowStr.find("{");
                        while (col < 8 && cellPos != string::npos) {
                            // Find the matching closing brace for this cell
                            int cellBraceCount = 0;
                            size_t cellEnd = cellPos;
                            for (size_t i = cellPos; i < rowStr.length(); ++i) {
                                if (rowStr[i] == '{') cellBraceCount++;
                                if (rowStr[i] == '}') {
                                    cellBraceCount--;
                                    if (cellBraceCount == 0) {
                                        cellEnd = i;
                                        break;
                                    }
                                }
                            }

                            if (cellEnd > cellPos) {
                                string cellStr = rowStr.substr(cellPos, cellEnd - cellPos + 1);
                                board[row][col] = parseCell(cellStr);
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

    return board;
}

// ===== READ ALL BOARD STATES (FOR HISTORY) =====
struct BoardHistory {
    int moveNumber;
    boardArray_t board;
};

vector<BoardHistory> readAllBoards(const string& filename) {
    vector<BoardHistory> history;

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return history;
    }

    // Read entire file
    string content, line;
    while (getline(file, line)) {
        content += line;
        content += "\n";
    }
    file.close();

    cout << "DEBUG: File content length: " << content.length() << endl;

    // Find all JSON objects
    vector<string> jsonObjects;
    size_t pos = 0;
    while (pos < content.length()) {
        size_t start = content.find("{", pos);
        if (start == string::npos) break;

        // Find matching closing brace
        int braceCount = 0;
        size_t end = start;
        for (size_t i = start; i < content.length(); ++i) {
            if (content[i] == '{') braceCount++;
            if (content[i] == '}') {
                braceCount--;
                if (braceCount == 0) {
                    end = i;
                    break;
                }
            }
        }

        if (end > start) {
            jsonObjects.push_back(content.substr(start, end - start + 1));
        }
        pos = end + 1;
    }

    cout << "DEBUG: Found " << jsonObjects.size() << " JSON objects" << endl;

    for (const string& jsonStr : jsonObjects) {
        BoardHistory entry;

        // Extract move number
        size_t numPos = jsonStr.find("\"moveNumber\"");
        if (numPos != string::npos) {
            numPos = jsonStr.find(":", numPos) + 1;
            while (numPos < jsonStr.length() && (jsonStr[numPos] == ' ' || jsonStr[numPos] == '\n' || jsonStr[numPos] == '\t')) {
                numPos++;
            }
            string numStr;
            while (numPos < jsonStr.length() && (isdigit(jsonStr[numPos]) || jsonStr[numPos] == '-')) {
                numStr += jsonStr[numPos];
                numPos++;
            }
            if (!numStr.empty()) {
                entry.moveNumber = stoi(numStr);
            }
        }

        // Parse board (simplified - just store empty board for now)
        // You can expand this if needed
        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                entry.board[row][col].cellName = string(1, 'a' + col) + to_string(8 - row);
                entry.board[row][col].isEmpty = true;
                entry.board[row][col].cellOwner = 'n';
            }
        }

        // Actually parse the board data
        size_t boardPos = jsonStr.find("\"board\"");
        if (boardPos != string::npos) {
            boardPos = jsonStr.find("[", boardPos);
            if (boardPos != string::npos) {
                // Find the matching closing bracket
                int bracketCount = 0;
                size_t boardEnd = boardPos;
                for (size_t i = boardPos; i < jsonStr.length(); ++i) {
                    if (jsonStr[i] == '[') bracketCount++;
                    if (jsonStr[i] == ']') {
                        bracketCount--;
                        if (bracketCount == 0) {
                            boardEnd = i;
                            break;
                        }
                    }
                }

                if (boardEnd > boardPos) {
                    string boardStr = jsonStr.substr(boardPos, boardEnd - boardPos + 1);

                    // Parse each row
                    int row = 0;
                    size_t rowPos = boardStr.find("[");
                    while (row < 8 && rowPos != string::npos) {
                        rowPos = boardStr.find("[", rowPos + 1);
                        if (rowPos == string::npos) break;

                        // Find the end of this row
                        int rowBracketCount = 0;
                        size_t rowEnd = rowPos;
                        for (size_t i = rowPos; i < boardStr.length(); ++i) {
                            if (boardStr[i] == '[') rowBracketCount++;
                            if (boardStr[i] == ']') {
                                rowBracketCount--;
                                if (rowBracketCount == 0) {
                                    rowEnd = i;
                                    break;
                                }
                            }
                        }

                        if (rowEnd > rowPos) {
                            string rowStr = boardStr.substr(rowPos, rowEnd - rowPos + 1);

                            // Parse each cell in the row
                            int col = 0;
                            size_t cellPos = rowStr.find("{");
                            while (col < 8 && cellPos != string::npos) {
                                // Find the matching closing brace for this cell
                                int cellBraceCount = 0;
                                size_t cellEnd = cellPos;
                                for (size_t i = cellPos; i < rowStr.length(); ++i) {
                                    if (rowStr[i] == '{') cellBraceCount++;
                                    if (rowStr[i] == '}') {
                                        cellBraceCount--;
                                        if (cellBraceCount == 0) {
                                            cellEnd = i;
                                            break;
                                        }
                                    }
                                }

                                if (cellEnd > cellPos) {
                                    string cellStr = rowStr.substr(cellPos, cellEnd - cellPos + 1);
                                    entry.board[row][col] = parseCell(cellStr);
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

        history.push_back(entry);
    }

    return history;
}


// ===== DISPLAY HISTORY ===== <<<< This is view history
void displayHistory(const string& filename) {
    vector<BoardHistory> history = readAllBoards(filename);

    if (history.empty()) {
        cout << "No history found!" << endl;
        return;
    }

    cout << "\n=== MOVE HISTORY (" << history.size() << " moves) ===" << endl;
    for (const auto& entry : history) {
        cout << "Move #" << entry.moveNumber << endl;
    }

    // Option to view specific move
    cout << "\nEnter move number to view (0 to skip): ";
    int viewMove;
    cin >> viewMove;

    for (const auto& entry : history) {
        if (entry.moveNumber == viewMove) {
            cout << "\n--- Move " << viewMove << " ---" << endl;
            displayBoard(entry.board);
            break;
        }
    }
}

// Undo ===================== <<<< This is undo
void undo(string filename) {
    cout << "\n--- UNDO: Loading latest board state ---" << endl;
    boardArray_t latestBoard = readLatestBoard(filename);
    displayBoard(latestBoard);
}
