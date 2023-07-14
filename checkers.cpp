#include <stdio.h>
#include <stdbool.h>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <algorithm>

#define BOARD_SIZE 8
bool is_player_black;
bool is_capture_possible;
bool is_black_turn;
bool is_game_on;
int num_pieces;
bool is_ai_on;

//Struct to represent a Coord
struct Coord{
	int row;
	int col;
    bool operator==(const Coord& other) const;
};

bool Coord::operator==(const Coord& other) const {
    return row == other.row && col == other.col;
}

Coord init_coord(int r, int c) {
    Coord co;
    co.row = r;
    co.col = c;
    return co;
}

// Struct to represent a game piece
struct Piece{
    bool is_black;   // true if the piece is black, false otherwise
    bool is_promoted;  // true if the piece is promoted, false otherwise
    bool is_void;  // true if the space is void, false otherwise
    Coord place; // Coord where the piece is on
    bool operator==(const Piece& other) const;
};

bool Piece::operator==(const Piece& other) const {
    return is_black == other.is_black && is_promoted == other.is_promoted && is_void == other.is_void && place == other.place;
}

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
                if (board[i][j].is_black && board[i][j].is_promoted) {
                    printf("B ");
                } else if (board[i][j].is_black && !board[i][j].is_promoted) {
                    printf("b ");
                } else if (!board[i][j].is_black && board[i][j].is_promoted) {
                    printf("W ");
                } else if (!board[i][j].is_black && !board[i][j].is_promoted) {
                    printf("w ");
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

bool detectCaptureForPromoted(Piece tarPie, Piece board[BOARD_SIZE][BOARD_SIZE]) {
    bool down = tarPie.place.row != 7;
	bool up = tarPie.place.row != 0;
	bool right = tarPie.place.col != 7;
	bool left = tarPie.place.col != 0;
	int tpr = tarPie.place.row;
	int tpc = tarPie.place.col;

    if(down && right) { //can we try down-right?
        int i = 1;
        while(tpr + i + 2 != 7 && tpc + i + 2 != 7) {
            if(!board[tpr + i][tpc + i].is_void && board[tpr + i][tpc + i].is_black != tarPie.is_black && board[tpr + i + 1][tpc + i + 1].is_void) {
                return true;
            }
            i++;
        }
    }

    if(down && left) { //can we try down-left?
        int i = 1;
        while(tpr + i + 2 != 7 && tpc - i - 2 != 0) {
            if(!board[tpr + i][tpc - i].is_void && board[tpr + i][tpc - i].is_black != tarPie.is_black && board[tpr + i + 1][tpc - i - 1].is_void) {
                return true;
            }
            i++;
        }
    }

    if(up && right) { //can we try up-right?
        int i = 1;
        while(tpr - i - 2 != 0 && tpc + i + 2 != 7) {
            if(!board[tpr - i][tpc + i].is_void && board[tpr - i][tpc + i].is_black != tarPie.is_black && board[tpr - i - 1][tpc + i + 1].is_void) {
                return true;
            }
            i++;
        }
    }
    
    if(up && left) { //can we try up-left?
        int i = 1;
        while(tpr - i - 2 != 0 && tpc - i - 2 != 0) {
            if(!board[tpr - i][tpc - i].is_void && board[tpr - i][tpc - i].is_black != tarPie.is_black && board[tpr - i - 1][tpc - i - 1].is_void) {
                return true;
            }
            i++;
        }
        
    }
	
    return false;
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
        if(is_capture_possible){
            if(p.is_promoted) {
                if(detectCaptureForPromoted(p, board)) {
                    movP.push_back(p);
                }
            } else {
                if(detectCaptureForPiece(p, board)) {
                    movP.push_back(p);
                }
            }
            
        } else if(detectMovementForPiece(p, board)) {
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
	std::vector<Piece> pieces = getAllTurnPieces(board);
	for(Piece p : pieces) {
        if(p.is_promoted) {
            if(detectCaptureForPromoted(p, board)) {
                is_capture_possible = true;
                break;
            }
        } else {
            if(detectCaptureForPiece(p, board)) {
                is_capture_possible = true;
                break;
            }
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

Movements generateCapturesForPromotedPiece(Piece tarPie, Piece board[BOARD_SIZE][BOARD_SIZE]) {
    Movements moves;
    bool down = tarPie.place.row != 7;
	bool up = tarPie.place.row != 0;
	bool right = tarPie.place.col != 7;
	bool left = tarPie.place.col != 0;
	int tpr = tarPie.place.row;
	int tpc = tarPie.place.col;

    if(down && right) { //can we try down-right?
        int i = 1;
        while(tpr + i + 1 != 7 && tpc + i + 1 != 7) {
            if(!board[tpr + i][tpc + i].is_void && board[tpr][tpc].is_black != board[tpr + i][tpc + i].is_black) {
                Movement m;
                m.push_back(init_coord(tpr, tpc));
                m.push_back(init_coord(tpr+i,tpc+i));
                moves.push_back(m);
                break;
            }
            i++;
        }
    }

    if(down && left) { //can we try down-left?
        int i = 1;
        while(tpr + i + 1 != 7 && tpc - i - 1 != 0) {
            if(!board[tpr + i][tpc - i].is_void && board[tpr][tpc].is_black != board[tpr + i][tpc - i].is_black) {
                Movement m;
                m.push_back(init_coord(tpr, tpc));
                m.push_back(init_coord(tpr+i,tpc-i));
                moves.push_back(m);
                break;
            }
            i++;
        }
    }

    if(up && right) { //can we try up-right?
        int i = 1;
        while(tpr - i - 1 != 0 && tpc + i + 1 != 7) {
            if(!board[tpr - i][tpc + i].is_void && board[tpr][tpc].is_black != board[tpr - i][tpc + i].is_black) {
                Movement m;
                m.push_back(init_coord(tpr, tpc));
                m.push_back(init_coord(tpr-i,tpc+i));
                moves.push_back(m);
                break;
            }
            i++;
        }
    }
    
    if(up && left) { //can we try up-left?
        int i = 1;
        while(tpr - i != 0 && tpc - i != 0) {
            if(!board[tpr - i][tpc - i].is_void && board[tpr][tpc].is_black != board[tpr - i][tpc - i].is_black) {
                Movement m;
                m.push_back(init_coord(tpr, tpc));
                m.push_back(init_coord(tpr-i,tpc-i));
                moves.push_back(m);
                break;
            }
            i++;
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

Movements generateMovesForPromotedPiece(Piece tarPie, Piece board[BOARD_SIZE][BOARD_SIZE]) {
    Movements moves;
    bool down = tarPie.place.row != 7;
	bool up = tarPie.place.row != 0;
	bool right = tarPie.place.col != 7;
	bool left = tarPie.place.col != 0;
	int tpr = tarPie.place.row;
	int tpc = tarPie.place.col;

    if(down && right) { //can we try down-right?
        int i = 1;
        while(tpr + i != 7 && tpc + i != 7) {
            if(board[tpr + i][tpc + i].is_void) {
                Movement m;
                m.push_back(init_coord(tpr, tpc));
                m.push_back(init_coord(tpr+i,tpc+i));
                moves.push_back(m);
            }
            i++;
        }
    }

    if(down && left) { //can we try down-left?
        int i = 1;
        while(tpr + i != 7 && tpc - i != 0) {
            if(board[tpr + i][tpc - i].is_void) {
                Movement m;
                m.push_back(init_coord(tpr, tpc));
                m.push_back(init_coord(tpr+i,tpc-i));
                moves.push_back(m);
            }
            i++;
        }
    }

    if(up && right) { //can we try up-right?
        int i = 1;
        while(tpr - i != 0 && tpc + i != 7) {
            if(board[tpr - i][tpc + i].is_void) {
                Movement m;
                m.push_back(init_coord(tpr, tpc));
                m.push_back(init_coord(tpr-i,tpc+i));
                moves.push_back(m);
            }
            i++;
        }
    }
    
    if(up && left) { //can we try up-left?
        int i = 1;
        while(tpr - i != 0 && tpc - i != 0) {
            if(board[tpr - i][tpc - i].is_void) {
                Movement m;
                m.push_back(init_coord(tpr, tpc));
                m.push_back(init_coord(tpr-i,tpc-i));
                moves.push_back(m);
            }
            i++;
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
        printf("This piece isn't yours\n");
        return moves;
    }

	if(is_capture_possible && detectCaptureForPiece(movingPiece, board)) { //If there is capture, only try to capture
		printf("Capturing a piece\n");
        if(movingPiece.is_promoted) {
            return generateCapturesForPromotedPiece(movingPiece, board);
        }
        return generateCapturesForPiece(movingPiece, board);
	} else if(!is_capture_possible && detectMovementForPiece(movingPiece, board)) { //If there is no capture, no need to endure
        if(movingPiece.is_promoted) {
            return generateMovesForPromotedPiece(movingPiece, board);
        }
        printf("Just getting a normal piece moves\n");
        return generateMovesForNormalPiece(movingPiece, board);
         
	} else { //Select another piece
        printf("Invalid piece selected, try another\n");
        return moves;
    }
    return moves;
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
            Movements moves = generateMoves(board,move.back().row,move.back().col);
            if(!is_ai_on) {//It's player
                Movement mTry;
                mTry.push_back(move.back());
                int row, col;
                printf("Insert row-col for movement:");
                scanf("%d",&row); //Hard-coded
                scanf("%d",&col); //Hard-coded
                printf("\n");
                mTry.push_back(init_coord(row, col));
                if(std::count(moves.begin(), moves.end(), mTry)) { //There is such moviment in valids?
                    executeMove(board, mTry);
                } else {
                    printf("Wrong move, try to capture again"); //How do i go back to the cap again point?
                }
            } else { //AI time to shine
                Movement m = moves.front();
                executeMove(board, m);
            }
        } else { //There is no more capture, try to verify if it can be promoted
            if((board[move.back().row][move.back().col].is_black && move.back().row == 7) || (!board[move.back().row][move.back().col].is_black && move.back().row == 0) ) {
                board[move.back().row][move.back().col].is_promoted = true;
            }
        }
        is_capture_possible = false;//Guarantee that there is no capture
    } else {//Execute normal movement
        Piece transf = board[move.front().row][move.front().col]; //Get piece to move
        board[move.back().row][move.back().col].is_void = false;//Register piece presence
        board[move.back().row][move.back().col].is_black = transf.is_black; //Put piece color
        board[move.back().row][move.back().col].is_promoted = transf.is_promoted; //Put piece promotion status
        board[move.front().row][move.front().col].is_void = true; //"Remove" the piece from the place
        //Now see if there is promotion
        if((board[move.back().row][move.back().col].is_black && move.back().row == 7) || (!board[move.back().row][move.back().col].is_black && move.back().row == 0) ) {
            board[move.back().row][move.back().col].is_promoted = true;
        }
    }
    
}

int evaluateMove(Movement move,Piece board[BOARD_SIZE][BOARD_SIZE]){
    // Make a copy of the board
    Piece copy[BOARD_SIZE][BOARD_SIZE];
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
        copy[i][j] = board[i][j]; 
        }
    }

    // Execute the move on the copy
    bool temp = is_capture_possible;
    executeMove(copy, move);
    is_capture_possible = temp;

    // Evaluate board and return score
    return evaluateBoard(copy);
}

// Function to find the best move using the Minimax algorithm with alpha-beta pruning
Movement findBestMove(Piece board[BOARD_SIZE][BOARD_SIZE], int dept, Movements moves) {
    Movement best_move = moves.front();
    int i = -13;
    for(Movement m : moves) {
        int t = evaluateMove(m, board);
        if(t > i) {
            i = t;
            best_move = m;
        }
    }
    return best_move; //For now, get the first for testing porpuses
}

int pW = 0;
int mW = 0;

void player_routine(Piece board[BOARD_SIZE][BOARD_SIZE]) {
    printf("Insert row-col for piece:");
    int row, col;
    scanf("%d",&row); //Hard-coded
    scanf("%d",&col); //Hard-coded
    printf("\n");
    Movements moves = generateMoves(board, row, col);
    if(moves.empty()) {
        printf("Invalid piece, select another\n");
    } else {
        Movement mTry;
        mTry.push_back(init_coord(row, col));
        printf("Insert row-col for movement:");
        scanf("%d",&row); //Hard-coded
        scanf("%d",&col); //Hard-coded
        printf("\n");
        mTry.push_back(init_coord(row, col));
        if(std::count(moves.begin(), moves.end(), mTry)) { //There is such moviment in valids?
            executeMove(board, mTry);
            is_black_turn = !is_black_turn;
            is_ai_on = true;
        } else {
            printf("Invalid movement, try again\n");
        }
    }
}

void ai_routine(Piece board[BOARD_SIZE][BOARD_SIZE]) {
    Movements moves;
    //Get machine movable pieces
    std::vector<Piece> pieces = getXRandomTurnMovablePieces(num_pieces, board);
    //For each one, generate thi moves
    for (Piece p : pieces) {
        Movements newM = generateMoves(board, p.place.row, p.place.col);
        moves.insert(moves.end(), newM.begin(), newM.end());
    }
    //Now chose da best move, mwahahaha
    Movement best = findBestMove(board, num_pieces, moves);
    executeMove(board, best);
    is_black_turn = !is_black_turn;
    is_ai_on = false;
}

int main() {
    Piece board[BOARD_SIZE][BOARD_SIZE];
    //1689366332 -- Seed with segmentation fault- it gives invalid Piece selected, try another
    //The problem may be related to the white king, since it can capture, but maybe failed
    //To recognize this capture, or better yet, to execute it
    srand (time(NULL)); //Initialize with random seed every start
    initializeBoard(board);
    printBoard(board);
    is_black_turn = true;
    is_capture_possible = false;
    is_player_black = false; //Hard-coded
	num_pieces = 4; //Hard-coded
    is_game_on = true;
    is_ai_on = is_black_turn =! is_player_black;
    while(is_game_on) {
        if(getAllTurnMovablePieces(board).empty()) { //No movable turn this time, someone won
            is_game_on = false;
            if(!is_ai_on) {
                mW++;
            } else {
                pW++;
            }
            break;
        }
        if(!is_ai_on) {
            player_routine(board);
        } else { //It is machine time
            ai_routine(board);
        }
        detectCaptureForTurn(board);
        printBoard(board);
    }

    return 0;
}
