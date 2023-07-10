#include <stdio.h>
#include <stdbool.h>
#include <vector>
#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE 8
bool is_player_black;
bool is_capture_possible;
bool is_black_turn;

//Struct to represent a Coord
typedef struct {
	int row;
	int col;
} Coord;

Coord init_coord(int r, int c) {
    Coord co;
    co.row = r;
    co.col = c;
    return co;
}

// Struct to represent a game piece
typedef struct {
    bool is_black;   // true if the piece is black, false otherwise
    bool is_promoted;  // true if the piece is promoted, false otherwise
    bool is_void;  // true if the space is void, false otherwise
    Coord place; // Coord where the piece is on
} Piece;

// Function to initialize the board
void initializeBoard(Piece board[BOARD_SIZE][BOARD_SIZE]) {
    int i, j;
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
        	board[i][j].place = init_coord(i,j);
            if (i < 3 && (i + j) % 2 == 0) {
                board[i][j].is_black = true;  // Black piece
                board[i][j].is_promoted = false;
                board[i][j].is_void = false;
            } else if (i > 4 && (i + j) % 2 == 0) {
                board[i][j].is_black = false;  // White piece
                board[i][j].is_promoted = false;
                board[i][j].is_void = false;
            } else {
		        board[i][j].is_void = true;
            }
        }
    }
}

// Function to print the board
void printBoard(Piece board[BOARD_SIZE][BOARD_SIZE]) {
    int i, j;
    printf("  A B C D E F G H\n");
    for (i = 0; i < BOARD_SIZE; i++) {
        printf("%d ", i + 1);
        for (j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j].is_void) {
                printf("  ");
            } else {
                if (board[i][j].is_black) {
                    printf("B ");
                } else {
                    printf("W ");
                } 
            }
        }
        printf("\n");
    }
}

// Function to evaluate the board state, sending a number
int evaluateBoard(Piece board[BOARD_SIZE][BOARD_SIZE]) {
    int i, j;
    int white_pieces = 0;
    int black_pieces = 0;
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j].is_black) {
                black_pieces++;
            } else {
                white_pieces++;
            }
        }
    }
    if(is_player_black) {
        return white_pieces - black_pieces;
    } else {
        return black_pieces - white_pieces;
    }
}

std::vector<Piece> getAllTurnPieces(Piece board[BOARD_SIZE][BOARD_SIZE]) {
	std::vector<Piece> pieces;
	for(int i = BOARD_SIZE; i < BOARD_SIZE; i++) {
    	for(int j = BOARD_SIZE; j < BOARD_SIZE; j++) {
    		if(!board[i][j].is_void && (board[i][j].is_black && is_black_turn) || (!board[i][j].is_black && !is_black_turn)) {//If there is a piece and this piece is in turn
    			pieces.push_back(board[i][j]);
    		}
        }
    }
    return pieces;
}

std::vector<Piece> getXRandomTurnPieces(int x, Piece board[BOARD_SIZE][BOARD_SIZE]) {
	std::vector<Piece> allP = getAllTurnPieces(board);
	while(allP.size() > x) {
		allP.erase(allP.begin() + (rand() % allP.size()));
	}
	return allP;
}

bool detectCaptureForPiece(Piece tarPie, Piece board[BOARD_SIZE][BOARD_SIZE]) {
	bool down = tarPie.place.row != 7;
	bool up = tarPie.place.row != 0;
	bool right = tarPie.place.col != 7;
	bool left = tarPie.place.col != 0;
	int tpr = tarPie.place.row;
	int tpc = tarPie.place.col;

	if(down && right) { //can we try down-right?
		if(!board[tpr + 1][tpc + 1].is_void && (board[tpr + 1][tpc + 1].is_black != tarPie.is_black) && tpr+2 != 8 && tpc+2 != 8) {
			if(board[tpr + 2][tpc + 2].is_void) {
				return true;
			}
		}
	}
	
	if(down && right) { //can we try down-left?
		if(!board[tpr + 1][tpc - 1].is_void && (board[tpr + 1][tpc - 1].is_black != tarPie.is_black) && tpr+2 != 8 && tpc-2 != -1) {
			if(board[tpr + 2][tpc - 2].is_void) {
				return true;
			}
		}
	}
	
	if(down && right) { //can we try up-right?
		if(!board[tpr - 1][tpc + 1].is_void && (board[tpr - 1][tpc + 1].is_black != tarPie.is_black) && tpr-2 != -1 && tpc+2 != 8) {
			if(board[tpr - 2][tpc + 2].is_void) {
				return true;
			}
		}
	}
	
	if(down && right) { //can we try up-left?
		if(!board[tpr - 1][tpc - 1].is_void && (board[tpr - 1][tpc - 1].is_black != tarPie.is_black) && tpr-2 != -1 && tpc-2 != -1) {
			if(board[tpr - 2][tpc - 2].is_void) {
				return true;
			}
		}
	}
	
	return false;//No capture found for this piece
}

void detectCaptureForTurn(Piece board[BOARD_SIZE][BOARD_SIZE]) {
	std::vector<Piece> pieces = getAllTurnPieces(board);
	for(Piece p : pieces) {
		if(detectCaptureForPiece(p, board)) {
			is_capture_possible = true;
			break;
		}
	}
}

// Function to generate all possible moves for a given piece
std::vector<std::vector<Coord>> generateMoves(Piece board[BOARD_SIZE][BOARD_SIZE], int row, int col) {
    std::vector<std::vector<Coord>> moves;
	if(board[row][col].is_void) { //Invalid movement
		printf("Invalid piece selected");
        return moves;
	}
    Piece movingPiece = board[row][col];

	if(is_capture_possible) { //If there is capture, only try to capture
		
	} else { //If there is no capture, no need to endure
	
	}
	//Old code, not working as there has been some changes, can be useful
	/*
    if(movingPiece.is_promoted) {
        for(int i = row; i < BOARD_SIZE; i++) {
            for(int j = col; j < BOARD_SIZE; j++) {
                if((i + j % 2) == 0 && (i != row && j != col)) { //If it is a place to move
                    if(board[i][j].is_void) { //If it doesn't have someone on the way
                    	std::vector<Coord> coords;
                    	coords.push_back(init_coord(row,col));
                    	coords.push_back(init_coord(i,j));
                        moves.push_back(coords);
                    } else { //There is something
                        if(board[i][j].is_black == movingPiece.is_black) {//If there is a friend ahead
                            //Can't move in this direction anymore
                            i = BOARD_SIZE;
                            j = BOARD_SIZE;
                        } else { //There is a enemy piece to be captured
                            if(i+1 < BOARD_SIZE && j + 1 < BOARD_SIZE) {//Can move
                                if(board[i+1][j+1].is_void) {
                                    
                                }
                            }
                            
                        }
                    }
                } 
            }
        }
    }*/
    return moves;
}

//TODO: Implement this after generateMoves is finished
// Function to perform the Minimax algorithm
//Will return evaluation for the move inserted
int minimax(Piece board[BOARD_SIZE][BOARD_SIZE], int depth, bool is_maximizing_player, int alpha, int beta, std::vector<Coord> move) {
   
   return -9999; //Placeholder return
}

// Function to find the best move using the Minimax algorithm with alpha-beta pruning
std::vector<Coord> findBestMove(Piece board[BOARD_SIZE][BOARD_SIZE], int num_pieces) {
    std::vector<Coord> best_move;
    int i;
    int max_eval = -9999;
    int alpha = -9999;
    int beta = 9999;
    // TODO: Select 'num_pieces' random computer pieces
    // TODO: Generate moves for each selected piece and evaluate them using the minimax algorithm
    // Keep track of the best move with the highest evaluation score
    return best_move;
}

int main() {
    Piece board[BOARD_SIZE][BOARD_SIZE];
    srand (time(NULL)); //Initialize with random seed every start
    initializeBoard(board);
    printBoard(board);
    is_player_black = false; //Hard-coded
	int num_pieces = 4; //Hard-coded

    std::vector<Coord> best_move = findBestMove(board, num_pieces);
    printf("Best Move: %c%d to %c%d\n", best_move[0].col + 'A', best_move[0].row + 1,
           best_move[1].col + 'A', best_move[1].row + 1);
    return 0;
}
