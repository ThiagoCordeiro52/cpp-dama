#ifndef MOVE_H
#define MOVE_H

#include <vector>
#include "board.h"
#include "main.h"
#include "capture.h"

using Movement = std::vector<Coord>;
using Movements = std::vector<Movement>;

Movements generateMoves(Piece board[BOARD_SIZE][BOARD_SIZE], int row, int col);
Movements generateCapturesForPiece(Piece tarPie, Piece board[BOARD_SIZE][BOARD_SIZE]);
Movements generateMovesForNormalPiece(Piece tarPie, Piece board[BOARD_SIZE][BOARD_SIZE]);
void executeMove(Piece board[BOARD_SIZE][BOARD_SIZE], Movement move);

#endif