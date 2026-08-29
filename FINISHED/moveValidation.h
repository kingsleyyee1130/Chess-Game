#pragma once
#include "gameLogic.h"
#include <vector>
#include <array>

// to use the functions in moveValidation, include "moveValidation.h"

bool isWithinBounds(int startCell[2], int endCell[2]);
bool isPathClear(const boardArray_t& board, int startCell[2], int endCell[2]);
bool validateKnight(int startCell[2], int endCell[2]);
bool validateRook(const boardArray_t& board, int startCell[2], int endCell[2]);
bool validateBishop(const boardArray_t& board, int startCell[2], int endCell[2]);
bool validateKing(const gameState& state, int startCell[2], int endCell[2]);
bool validatePawn(const boardArray_t& board, const gameState& state, int startCell[2], int endCell[2]);
bool validateQueen(const boardArray_t& board, int startCell[2], int endCell[2]);
bool doesCellHaveMoves(const gameState& state, int startCell[2], int endCell[2]);
bool isMoveLegal(const gameState& state, int startCell[2], int endCell[2]);