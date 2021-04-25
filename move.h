#ifndef MOVE_H
#define MOVE_H

#include "chessboard.h"

#define nr       next.rank
#define pr       prev.rank
#define nf       next.file
#define pf       prev.file
#define nr_pr    next.rank - prev.rank
#define pr_nr    prev.rank - next.rank
#define nf_pf    next.file - prev.file
#define pf_nf    prev.file - next.file

void move_turn (chessboard *cb);
void move_piece(chessboard *cb, coordinates next, bool is_taking);
void move_check_piece(chessboard *cb);
int move_check_pawn (chessboard *cb, coordinates prev, coordinates next, bool color, bool is_taking);
int move_check_knight (chessboard *cb, coordinates prev, coordinates next, bool is_taking);
int move_check_bishop (chessboard *cb, coordinates prev, coordinates next, bool is_taking);
int move_check_rook (chessboard *cb, coordinates prev, coordinates next, bool is_taking);
int move_check_queen (chessboard *cb, coordinates prev, coordinates next, bool is_taking);
int move_check_king (chessboard *cb, coordinates prev, coordinates next, bool color, bool is_taking);

#endif