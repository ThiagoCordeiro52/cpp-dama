#include "move.h"

Movements generateMoves(Piece board[BOARD_SIZE][BOARD_SIZE], int row, int col) {
    Movements moves;
	if(board[row][col].is_void) { //No piece
		std::cout << "There is no piece \n";
        return moves;
	}
    std::cout << "There is Piece \n";
    Piece movingPiece = board[row][col];

    if((movingPiece.is_black && !is_black_turn) || (!movingPiece.is_black && is_black_turn)) {
        std::cout << "This is not your piece \n ";
        return moves;
    }

	if(is_capture_possible && detectCaptureForPiece(movingPiece, board)) { //If there is capture, only try to capture
		std::cout << "Capturing a piece \n";
        return generateCapturesForPiece(movingPiece, board);
	} else if(detectMovementForPiece(movingPiece, board)) { //If there is no capture, no need to endure
         if(movingPiece.is_promoted) {
             //moves for promoted
         } else {
             std::cout << "Just moving a normal piece \n";
             return generateMovesForNormalPiece(movingPiece, board);
         }
	} else { //Select another piece
        std::cout << "Invalid piece selected, try another \n";
        return moves;
    }
    return moves;
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

void executeMove(Piece board[BOARD_SIZE][BOARD_SIZE], Movement move) {
    if(is_capture_possible) {//Execute capture movement
        //For now, let's consider there is only one capturable per move
        int capR = move.front().row - move.back().row;
        int capC = move.front().col - move.back().col;
        Piece transf = board[move.front().row][move.front().col]; //Get piece to move
        board[move.back().row][move.back().col] = transf; //Put piece in its new place
        board[move.front().row][move.front().col].is_void = true; //"Remove" the piece from the place
        board[move.front().row + capR][move.front().col + capC].is_void = true; //"Remove" the captured piece from the place
        is_capture_possible = false;//Guarantee that there is no capture
    } else {//Execute normal movement
        Piece transf = board[move.front().row][move.front().col]; //Get piece to move
        board[move.back().row][move.back().col] = transf; //Put piece in its new place
        board[move.front().row][move.front().col].is_void = true; //"Remove" the piece from the place
    }
    //return board;
    
}