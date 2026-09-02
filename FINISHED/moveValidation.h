#pragma once
#include "gameLogic.h"
#include <vector>
#include <array>

// to check if a coordinate is within chess board
bool isWithinBounds(int startCell[2], int endCell[2]);

// to check if all squares between startCell and endCell are empty
bool isPathClear(const boardArray_t& board, int startCell[2], int endCell[2]);

// ==== piece specific validation functions ====
bool validateKnight(int startCell[2], int endCell[2]);
bool validateRook(const boardArray_t& board, int startCell[2], int endCell[2]);
bool validateBishop(const boardArray_t& board, int startCell[2], int endCell[2]);
bool validateKing(const gameState& state, int startCell[2], int endCell[2]);
bool validatePawn(const boardArray_t& board, const gameState& state, int startCell[2], int endCell[2]);
bool validateQueen(const boardArray_t& board, int startCell[2], int endCell[2]);

// generic validation function that recognises piece type in cell and calls validation function accordingly
bool doesCellHaveMoves(const gameState& state, int startCell[2], int endCell[2]);

// the main validation function
bool isMoveLegal(const gameState& state, int startCell[2], int endCell[2]);