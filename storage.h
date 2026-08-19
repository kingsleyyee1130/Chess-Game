#pragma once
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


// ===== CONVERT CELL TO STRING =====
string cellToString(const Cell& cell);

// ===== CONVERT BOARD TO STRING =====
string boardToString(const boardArray_t& board);

// ===== CONVERT GAME STATE TO STRING =====
string gameStateToString(const gameState& state);

// ===== PARSE CELL =====
Cell parseCell(const string& str);

// ===== PARSE GAME STATE =====
gameState parseGameState(const string& json);

// ===== 1. SAVE File Name =====
void saveFileName(const string& filename);

// create a file name
void createFile(string whitePlayer, string blackPlayer);

// ===== 2. SAVE MOVE TO HISTORY (APPEND) =====
void saveMoveToHistory(const gameState& state, const string& filename);

// ===== 3. LOAD LATEST GAME STATE (FOR UNDO) =====
bool loadLatestGameState(gameState& state, const string& filename);

// ===== 4. LOAD SPECIFIC MOVE FROM HISTORY =====
bool loadMoveFromHistory(gameState& state, const string& filename, int moveNumber);

// ===== 5. DELETE LAST MOVE FROM FILE (FOR UNDO) =====
bool deleteLastMoveFromFile(const string& filename);

// ===== 6. GET TOTAL MOVES IN FILE =====
int getTotalMovesInFile(const string& filename);

// ===== 7. CLEAR HISTORY FILE =====
void clearHistory(const string& filename);

// ===== 8. RECORD STATE (BEFORE MOVE) =====
void recordState(gameState& state, vector<gameState>& history);

// ===== 9. UNDO MOVE (WITH FILE DELETION) =====
bool undoMove(gameState& state, vector<gameState>& history, const string& filename);

// Load specific move
void viewHistory(const string& filename, gameState& state);

// Direct read all file and display the choice
int showAllFile();

// connect user choice to filename for game resume
string readFileName(int choice);

// to access the last state of a saved game
bool accessLastState(const string& filename, gameState& state);
