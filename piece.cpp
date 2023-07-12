#include "piece.h"

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

std::vector<Piece> getAllTurnMovablePieces(Piece board[BOARD_SIZE][BOARD_SIZE]) {
	std::vector<Piece> allP = getAllTurnPieces(board);
    std::vector<Piece> movP;
    for (Piece p : allP) {
        if(detectCaptureForPiece(p, board) && detectMovementForPiece(p, board)) {
            movP.push_back(p);
        }
    }
	return movP;
}