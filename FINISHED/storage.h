#pragma once
#include "gameLogic.h"


// ===== HELPER: strip whitespace, but never inside a quoted JSON string ====
string stripWhitespaceOutsideQuotes(const string& input);

// ===== HELPER: parse a 2-element integer array field like "key":[a,b] =====
void parseIntPair(const string& src, const string& key, int (&arr)[2]);

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

// ===== 1. SAVE & RETURNS File Name (Into Master File_of_file) =====
string saveFileToMaster(string whitePlayer, string blackPlayer);

// ===== 2. SAVE STATE TO FILE (APPEND AFTER EVERY MOVE) =====
void saveStateToFile(const gameState& state, const string& filename);

// ===== 3. LOAD LATEST GAME STATE (FOR CONTINUE GAME & UNDO) =====
bool loadLatestGameState(gameState& state, const string& filename);

// ===== 4. DELETE LAST STATE FROM FILE (FOR UNDO) =====
bool deleteLastStateFromFile(const string& filename);

// ===== 5. LOAD SPECIFIC STATE FROM HISTORY =====
bool loadStateFromHistory(gameState& state, const string& filename, int moveNumber);

// ===== 6. GET TOTAL MOVES IN FILE =====
int getTotalMovesInFile(const string& filename);

// Undo to last gameState
bool undoMove(gameState& state);

// Load specific move
void viewHistory(const string& filename, gameState& state);

// Direct read all file and display the choice
int showAllFile();

// connect user choice to filename for game resume
string readFileName(int choice);
