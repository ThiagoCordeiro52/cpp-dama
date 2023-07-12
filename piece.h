#ifndef PIECE_H
#define PIECE_H

#include "coord.h"
#include "constants.h"
#include <vector>

// Struct to represent a game piece
typedef struct {
    bool is_black;   // true if the piece is black, false otherwise
    bool is_promoted;  // true if the piece is promoted, false otherwise
    bool is_void;  // true if the space is void, false otherwise
    Coord place; // Coord where the piece is on
} Piece;

bool detectMovementForPiece(Piece tarPie, Piece board[BOARD_SIZE][BOARD_SIZE]);
std::vector<Piece> getAllTurnMovablePieces(Piece board[BOARD_SIZE][BOARD_SIZE]);
std::vector<Piece> getAllTurnPieces(Piece board[BOARD_SIZE][BOARD_SIZE]);

#endif