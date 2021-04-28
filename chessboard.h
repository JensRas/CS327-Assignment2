#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <vector>

#define pawn        'p'
#define knight      'N'
#define bishop      'B'
#define rook        'R'
#define queen       'Q'
#define king        'K'
#define empty       ' '

#define black       0
#define white       1

class coordinates
{
    public:
        char file; // col
        int rank; // row
};

class chess_piece
{
    public:
        chess_piece(bool color, char type) {this->color = color, this->type = type;}
        //~chess_piece();
        bool color;
        char type;
        coordinates coord;
};

class chessboard 
{
    public:
        chess_piece *piece_map[8][8] = {0};
        chess_piece *piece_graveyard[32] = {0};
        std::vector <coordinates> whites_notation;
        std::vector <coordinates> blacks_notation;
        bool can_castle_white_long = 1;
        bool can_castle_white_short = 1;
        bool can_castle_black_long = 1;
        bool can_castle_black_short = 1;
        int white_pieces[6] = {0}; // p = 0, R = 1, N = 2, B = 3, Q = 4, K = 5.
        int black_pieces[6] = {0}; // p = 0, R = 1, N = 2, B = 3, Q = 4, K = 5.
        int end_game_flag = 0;
        chess_piece *selected_piece = 0;
        int num_taken = 0;
        bool placing = false;
        bool whose_turn = white; // true == white, false == black
};

void cb_place_pieces(chessboard *cb);

#endif