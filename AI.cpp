#include "AI.h"

Movement findBestMove(Piece board[BOARD_SIZE][BOARD_SIZE], int num_pieces) {
    Movement best_move;
    int i;
    int max_eval = -9999;
    int alpha = -9999;
    int beta = 9999;
    // TODO: Select 'num_pieces' random computer pieces
    // TODO: Generate moves for each selected piece and evaluate them using the minimax algorithm
    // Keep track of the best move with the highest evaluation score
    return best_move;
}

int minimax(Piece board[BOARD_SIZE][BOARD_SIZE], int depth, bool is_maximizing_player, int alpha, int beta, std::vector<Coord> move) {
   
   return -9999; //Placeholder return
}