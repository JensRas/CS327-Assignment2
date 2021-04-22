#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <vector>

#define pawn        'p'
#define knight      'N'
#define bishop      'B'
#define rook        'R'
#define queen       'Q'
#define king        'K'

#define black       0
#define white       1

class coordinates
{
    public:
        char file;
        int rank;
        bool takes;
        bool promotes;
};

class chess_piece
{
    public:
    chess_piece(bool color, char type) {this->color = color, this->type = type;}
    ~chess_piece();
        bool color;
        char type;
        coordinates coord;
};

class chessboard 
{
    public:
        chess_piece *piece_map[8][8] = {0};
        std::vector <coordinates> whites_notation;
        std::vector <coordinates> blacks_notation;
};

void cb_place_pieces(chessboard *cb);

#endif