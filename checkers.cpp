#include <stdio.h>
#include <stdbool.h>
#include <vector>
#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE 8
bool is_player_black;
bool is_capture_possible;
bool is_black_turn;
bool is_game_on;

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

using Movement = std::vector<Coord>;
using Movements = std::vector<Movement>;

// Function to initialize the board
void initializeBoard(Piece board[BOARD_SIZE][BOARD_SIZE]) {
    int i, j;
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
        	board[i][j].place = init_coord(i,j);
            board[i][j].is_void = true;
            if (i < 3 && (i + j) % 2 == 0) {
                board[i][j].is_black = true;  // Black piece
                board[i][j].is_promoted = false;
                board[i][j].is_void = false;
            } else if (i > 4 && (i + j) % 2 == 0) {
                board[i][j].is_black = false;  // White piece
                board[i][j].is_promoted = false;
                board[i][j].is_void = false;
            }
        }
    }
}

// Function to print the board
void printBoard(Piece board[BOARD_SIZE][BOARD_SIZE]) {
    int i, j;
    printf("  0 1 2 3 4 5 6 7\n");
    for (i = 0; i < BOARD_SIZE; i++) {
        printf("%d ", i);
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

bool detectCaptureForPiece(Piece tarPie, Piece board[BOARD_SIZE][BOARD_SIZE]) {
	bool down = tarPie.place.row < 6;
	bool up = tarPie.place.row > 1;
	bool right = tarPie.place.col < 6;
	bool left = tarPie.place.col > 1;
	int tpr = tarPie.place.row;
	int tpc = tarPie.place.col;

	if(down && right) { //can we try down-right?
		if(!board[tpr + 1][tpc + 1].is_void && (board[tpr + 1][tpc + 1].is_black != tarPie.is_black) && board[tpr + 2][tpc + 2].is_void) {
			return true;
		}
	}
	
	if(down && left) { //can we try down-left?
		if(!board[tpr + 1][tpc - 1].is_void && (board[tpr + 1][tpc - 1].is_black != tarPie.is_black) && board[tpr + 2][tpc - 2].is_void) {
			return true;
		}
	}
	
	if(up && right) { //can we try up-right?
		if(!board[tpr - 1][tpc + 1].is_void && (board[tpr - 1][tpc + 1].is_black != tarPie.is_black) && board[tpr - 2][tpc + 2].is_void) {
			return true;
		}
	}
	
	if(up && left) { //can we try up-left?
		if(!board[tpr - 1][tpc - 1].is_void && (board[tpr - 1][tpc - 1].is_black != tarPie.is_black) && board[tpr - 2][tpc - 2].is_void) {
			return true;
		}
	}
	
	return false;//No capture found for this piece
}


std::vector<Piece> getAllTurnPieces(Piece board[BOARD_SIZE][BOARD_SIZE]) {
	std::vector<Piece> pieces;
	for(int i = 0; i < BOARD_SIZE; i++) {
    	for(int j = 0; j < BOARD_SIZE; j++) {
    		if(!board[i][j].is_void && ((board[i][j].is_black && is_black_turn) || (!board[i][j].is_black && !is_black_turn))) {//If there is a piece and this piece is in turn
                pieces.push_back(board[i][j]);
    		}
        }
    }
    return pieces;
}

bool detectMovementForPiece(Piece tarPie, Piece board[BOARD_SIZE][BOARD_SIZE]) {
    bool down = tarPie.place.row != 7;
	bool up = tarPie.place.row != 0;
	bool right = tarPie.place.col != 7;
	bool left = tarPie.place.col != 0;
	int tpr = tarPie.place.row;
	int tpc = tarPie.place.col;

    if(tarPie.is_promoted) {
        if(down && right) { //can we try down-right?
            if(board[tpr + 1][tpc + 1].is_void) {
                return true;
            }
        
	    }
	
        if(down && left) { //can we try down-left?
            if(board[tpr + 1][tpc - 1].is_void) {
                return true;
            }
        }

        if(up && right) { //can we try up-right?
            if(board[tpr - 1][tpc + 1].is_void) {
                return true;
            }
        }
        
        if(up && left) { //can we try up-left?
            if(board[tpr - 1][tpc - 1].is_void) {
                return true;
            }
        }
    } else {
        if(tarPie.is_black) {
            if(down && right) { //can we try down-right?
                if(board[tpr + 1][tpc + 1].is_void) {
                    return true;
                }
            
            }
        
            if(down && left) { //can we try down-left?
                if(board[tpr + 1][tpc - 1].is_void) {
                    return true;
                }
            }
        } else {

            if(up && right) { //can we try up-right?
                if(board[tpr - 1][tpc + 1].is_void) {
                    return true;
                }
            }
            
            if(up && left) { //can we try up-left?
                if(board[tpr - 1][tpc - 1].is_void) {
                    return true;
                }
            }

        }
    }
	
    return false;
}

std::vector<Piece> getAllTurnMovablePieces(Piece board[BOARD_SIZE][BOARD_SIZE]) {
	std::vector<Piece> allP = getAllTurnPieces(board);
    std::vector<Piece> movP;
    for (Piece p : allP) {
        if(detectCaptureForPiece(p, board) || detectMovementForPiece(p, board)) {
            movP.push_back(p);
        }
    }
	return movP;
}

std::vector<Piece> getXRandomTurnMovablePieces(int x, Piece board[BOARD_SIZE][BOARD_SIZE]) {
	std::vector<Piece> allP = getAllTurnMovablePieces(board);
	while(allP.size() > x) {
		allP.erase(allP.begin() + (rand() % allP.size()));
	}
	return allP;
}

void detectCaptureForTurn(Piece board[BOARD_SIZE][BOARD_SIZE]) {
	std::vector<Piece> pieces = getAllTurnMovablePieces(board);
	for(Piece p : pieces) {
		if(detectCaptureForPiece(p, board)) {
			is_capture_possible = true;
			break;
		}
	}
}

Movements generateCapturesForPiece(Piece tarPie, Piece board[BOARD_SIZE][BOARD_SIZE]) {
    Movements moves;
    bool down = tarPie.place.row != 7;
	bool up = tarPie.place.row != 0;
	bool right = tarPie.place.col != 7;
	bool left = tarPie.place.col != 0;
	int tpr = tarPie.place.row;
	int tpc = tarPie.place.col;

	if(down && right) { //can we try down-right?
		if(!board[tpr + 1][tpc + 1].is_void && (board[tpr + 1][tpc + 1].is_black != tarPie.is_black) && tpr+2 != 8 && tpc+2 != 8) {
			if(board[tpr + 2][tpc + 2].is_void) {
                Movement m;
                m.push_back(init_coord(tpr, tpc));
                m.push_back(init_coord(tpr+2,tpc+2));
				moves.push_back(m);
			}
		}
	}
	
	if(down && left) { //can we try down-left?
		if(!board[tpr + 1][tpc - 1].is_void && (board[tpr + 1][tpc - 1].is_black != tarPie.is_black) && tpr+2 != 8 && tpc-2 != -1) {
			if(board[tpr + 2][tpc - 2].is_void) {
				Movement m;
                m.push_back(init_coord(tpr, tpc));
                m.push_back(init_coord(tpr+2,tpc-2));
				moves.push_back(m);
			}
		}
	}
	
	if(up && right) { //can we try up-right?
		if(!board[tpr - 1][tpc + 1].is_void && (board[tpr - 1][tpc + 1].is_black != tarPie.is_black) && tpr-2 != -1 && tpc+2 != 8) {
			if(board[tpr - 2][tpc + 2].is_void) {
				Movement m;
                m.push_back(init_coord(tpr, tpc));
                m.push_back(init_coord(tpr-2,tpc+2));
				moves.push_back(m);
			}
		}
	}
	
	if(up && left) { //can we try up-left?
		if(!board[tpr - 1][tpc - 1].is_void && (board[tpr - 1][tpc - 1].is_black != tarPie.is_black) && tpr-2 != -1 && tpc-2 != -1) {
			if(board[tpr - 2][tpc - 2].is_void) {
				Movement m;
                m.push_back(init_coord(tpr, tpc));
                m.push_back(init_coord(tpr-2,tpc-2));
				moves.push_back(m);
			}
		}
	}
	
	return moves;
}

Movements generateMovesForNormalPiece(Piece tarPie, Piece board[BOARD_SIZE][BOARD_SIZE]) {
    Movements moves;
    bool down = tarPie.place.row != 7;
	bool up = tarPie.place.row != 0;
	bool right = tarPie.place.col != 7;
	bool left = tarPie.place.col != 0;
	int tpr = tarPie.place.row;
	int tpc = tarPie.place.col;

    if(tarPie.is_black) {
        if(down && right) { //can we try down-right?
            if(board[tpr + 1][tpc + 1].is_void) {
                Movement m;
                m.push_back(init_coord(tpr, tpc));
                m.push_back(init_coord(tpr+1,tpc+1));
				moves.push_back(m);
            }
        
	    }
	
        if(down && left) { //can we try down-left?
            if(board[tpr + 1][tpc - 1].is_void) {
                Movement m;
                m.push_back(init_coord(tpr, tpc));
                m.push_back(init_coord(tpr+1,tpc-1));
				moves.push_back(m);
            }
        }
    } else {

        if(up && right) { //can we try up-right?
            if(board[tpr - 1][tpc + 1].is_void) {
                Movement m;
                m.push_back(init_coord(tpr, tpc));
                m.push_back(init_coord(tpr-1,tpc+1));
				moves.push_back(m);
            }
        }
        
        if(up && left) { //can we try up-left?
            if(board[tpr - 1][tpc - 1].is_void) {
                Movement m;
                m.push_back(init_coord(tpr, tpc));
                m.push_back(init_coord(tpr-1,tpc-1));
				moves.push_back(m);
            }
        }

    }
	
    return moves;
}

// Function to generate all possible moves for a given piece
Movements generateMoves(Piece board[BOARD_SIZE][BOARD_SIZE], int row, int col) {
    Movements moves;
	if(board[row][col].is_void) { //No piece
		printf("There is no piece\n");
        return moves;
	}
    Piece movingPiece = board[row][col];

    if((movingPiece.is_black && !is_black_turn) || (!movingPiece.is_black && is_black_turn)) {
        return moves;
    }

	if(is_capture_possible && detectCaptureForPiece(movingPiece, board)) { //If there is capture, only try to capture
		printf("Capturing a piece\n");
        return generateCapturesForPiece(movingPiece, board);
	} else if(detectMovementForPiece(movingPiece, board)) { //If there is no capture, no need to endure
         if(movingPiece.is_promoted) {
             //moves for promoted
         } else {
             printf("Just moving a normal piece\n");
             return generateMovesForNormalPiece(movingPiece, board);
         }
	} else { //Select another piece
        printf("Invalid piece selected, try another\n");
        return moves;
    }
    return moves;
}


//TODO: Implement this after generateMoves is finished
// Function to perform the Minimax algorithm
//Will return evaluation for the move inserted
int minimax(Piece board[BOARD_SIZE][BOARD_SIZE], int depth, bool is_maximizing_player, int alpha, int beta, std::vector<Coord> move) {
   
   return -9999; //Placeholder return
}

// Function to find the best move using the Minimax algorithm with alpha-beta pruning
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

void movePiece1To2(Piece p1, Piece p2){

}

void executeMove(Piece board[BOARD_SIZE][BOARD_SIZE], Movement move) {
    if(is_capture_possible) {//Execute capture movement
        //For now, let's consider there is only one capturable per move
        int capR = (move.front().row - move.back().row)/2;
        int capC = (move.front().col - move.back().col)/2;
        Piece transf = board[move.front().row][move.front().col]; //Get piece to move
        board[move.back().row][move.back().col].is_void = false;//Register piece presence
        board[move.back().row][move.back().col].is_black = transf.is_black; //Put piece color
        board[move.back().row][move.back().col].is_promoted = transf.is_promoted; //Put piece promotion status
        board[move.front().row][move.front().col].is_void = true; //"Remove" the piece from the place
        board[move.back().row + capR][move.back().col + capC].is_void = true; //"Remove" the captured piece from the place
        if(detectCaptureForPiece(board[move.back().row][move.back().col], board)){
            //For multiple captures, there will be a recursion around here, be it for player or AI
        }
        is_capture_possible = false;//Guarantee that there is no capture
    } else {//Execute normal movement
        Piece transf = board[move.front().row][move.front().col]; //Get piece to move
        board[move.back().row][move.back().col].is_void = false;//Register piece presence
        board[move.back().row][move.back().col].is_black = transf.is_black; //Put piece color
        board[move.back().row][move.back().col].is_promoted = transf.is_promoted; //Put piece promotion status
        board[move.front().row][move.front().col].is_void = true; //"Remove" the piece from the place
    }
    //return board;
    
}

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
        printf("Insert row-col for piece:");
        int row, col;
        scanf("%d",&row);
        scanf("%d",&col);
        printf("\n");
        Movements moves = generateMoves(board, row, col);
        if(!moves.empty()) {//Wrong piece, you piece of piece
            valid_piece = true;
        }
        if(valid_piece) {
            executeMove(board, moves.front());
            is_black_turn = !is_black_turn;
        }
        detectCaptureForTurn(board);
        printBoard(board);
    }

    /* For future tests only
    Movement best_move = findBestMove(board, num_pieces);
    printf("Best Move: %c%d to %c%d\n", best_move[0].col + 'A', best_move[0].row + 1,
           best_move[1].col + 'A', best_move[1].row + 1);
    */
    return 0;
}
