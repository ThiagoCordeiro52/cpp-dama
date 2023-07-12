#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include "capture.h"

bool is_player_black;
bool is_capture_possible;
bool is_black_turn;
bool is_game_on;

void initializeBoard(Piece board[BOARD_SIZE][BOARD_SIZE]);
void printBoard(Piece board[BOARD_SIZE][BOARD_SIZE]);

#endif
