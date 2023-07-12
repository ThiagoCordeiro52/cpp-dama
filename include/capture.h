#ifndef CAPTURE_H
#define CAPTURE_H

#include "coord.h"
#include "constants.h"
#include <vector>
#include "structures.h"


bool detectCaptureForPiece(Piece tarPie, Piece board[BOARD_SIZE][BOARD_SIZE]);
void detectCaptureForTurn(Piece board[BOARD_SIZE][BOARD_SIZE]);

#endif
