#ifndef AI_H
#define AI_H

#include "move.h"

Movement findBestMove(Piece board[BOARD_SIZE][BOARD_SIZE], int num_pieces);
int minimax(Piece board[BOARD_SIZE][BOARD_SIZE], int depth, bool is_maximizing_player, int alpha, int beta, std::vector<Coord> move);

#endif