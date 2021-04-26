#include "chessboard.h"

void cb_place_pieces(chessboard *cb)
{
    int y, x;
    char type;
    bool color;
    char file;
    int rank;

    chess_piece *p;

    // White pieces
    for (y = 6; y < 8; y++) {
        for (x = 0; x < 8; x++) {
            if (y == 6) {
                type = pawn;
                rank = 2;
            } else if(x == 0 || x == 7) {
                type = rook;
                rank = 1; 
            } else if (x == 1 || x == 6) {
                type = knight;
                rank = 1;
            } else if (x == 2 || x == 5) {
                type = bishop;
                rank = 1;
            } else if (x == 3) {
                type = queen;
                rank = 1;
            } else if (x == 4) {
                type = king;
                rank = 1;
            }
            color = white;
            file = x + 97;
            p = new chess_piece(color, type);
            cb->piece_map[y][x] = p;
            cb->piece_map[y][x]->coord.file = file;
            cb->piece_map[y][x]->coord.rank = rank;
        }
    }

    //Black pieces
    for (y = 0; y < 2; y++) {
        for (x = 0; x < 8; x++) {
            if (y == 1) {
                type = pawn;
                rank = 7;
            } else if(x == 0 || x == 7) {
                type = rook;
                rank = 8;
            } else if (x == 1 || x == 6) {
                type = knight;
                rank = 8;
            } else if (x == 2 || x == 5) {
                type = bishop;
                rank = 8;
            } else if (x == 3) {
                type = queen;
                rank = 8;
            } else if (x == 4) {
                type = king;
                rank = 8;
            }
            file = x + 97;
            color = black;
            p = new chess_piece(color, type);
            cb->piece_map[y][x] = p;
            cb->piece_map[y][x]->coord.file = file;
            cb->piece_map[y][x]->coord.rank = rank;
        }
    }

    // Assigning empty squares 'empty' pieces
    for (y = 2; y < 6; y++) {
        for (x = 0; x < 8; x++) {
            color = white;
            type = empty;
            
            p = new chess_piece(color, type);
            cb->piece_map[y][x] = p;
            cb->piece_map[y][x]->coord.file = x + 97;
            cb->piece_map[y][x]->coord.rank = 8 - y;
        }
    }
}