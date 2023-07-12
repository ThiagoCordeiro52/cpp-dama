#ifndef STRUCTURES_H
#define STRUCTURES_H

//Struct to represent a Coord
typedef struct {
	int row;
	int col;
} Coord;

typedef struct {
    bool is_black;   // true if the piece is black, false otherwise
    bool is_promoted;  // true if the piece is promoted, false otherwise
    bool is_void;  // true if the space is void, false otherwise
    Coord place; // Coord where the piece is on
} Piece;


#endif