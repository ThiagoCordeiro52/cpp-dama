#include "main.h"
#include "board.h"
#include "move.h"

int main() {
    Piece board[BOARD_SIZE][BOARD_SIZE];
    srand (time(NULL)); //Initialize with random seed every start
    initializeBoard(board);
    printBoard(board);
    is_black_turn = true;
    is_capture_possible = false;
    is_player_black = false; //Hard-coded
	int num_pieces = 4; //Hard-coded
    is_game_on = true;
    while(is_game_on) {
        bool valid_piece = false;
        detectCaptureForTurn(board);
        std::cout << "Insert row for piece: ";
        int row;
        std::cin >> row;
        std::cout << "\nInsert col for piece: ";
        int col;
        std::cin >> col;
        std::cout << std::endl;
        Movements moves = generateMoves(board, row, col);
        if(!moves.empty()) {//Wrong piece, you piece of piece
            valid_piece = true;
        }
        if(valid_piece) {
            executeMove(board, moves.front());
            is_black_turn = !is_black_turn;
        }
        printBoard(board);
    }

    /* For future tests only
    Movement best_move = findBestMove(board, num_pieces);
    printf("Best Move: %c%d to %c%d\n", best_move[0].col + 'A', best_move[0].row + 1,
           best_move[1].col + 'A', best_move[1].row + 1);
    */
    return 0;
}