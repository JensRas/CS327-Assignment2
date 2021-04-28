#include <curses.h>
#include <vector>
#include <cstdlib>
#include <math.h>

#include "move.h"
#include "chessboard.h"
#include "io.h"
#include "printer.h"

void move_turn (chessboard *cb)
{
    do {
        //white's turn
        print_board(cb);
        mvprintw(0, 0, "         WHITE'S TURN TO MOVE            ");
        refresh();

        cb->whose_turn = white;
        io_move_cursor(cb, 0, 8, 16);
        if (cb->end_game_flag == 3)
            return;
        if(cb->end_game_flag == 1 || isCheckmate(cb, black) == 8) {
            cb->end_game_flag = 1; // white wins
            return;
        }
        mvprintw(0, 0, "                                         ");
        refresh();
        if (cb->selected_piece->coord.rank == 1 && cb->selected_piece->coord.file == 'e') {
            cb->can_castle_white_long = 0;
            cb->can_castle_white_short = 0;
        } else if (cb->selected_piece->coord.rank == 1 && cb->selected_piece->coord.file == 'a') {
            cb->can_castle_white_long = 0;
        } else if (cb->selected_piece->coord.rank == 1 && cb->selected_piece->coord.file == 'h') {
            cb->can_castle_white_short = 0;
        }
        cb->placing = false;
        cb->selected_piece = 0;
        //black's turn
        print_board(cb);
        mvprintw(0, 0, "         BLACK'S TURN TO MOVE            ");
        refresh();
        cb->whose_turn = black;
        io_move_cursor(cb, 0, 8, 16);
        if (cb->end_game_flag == 3)
            return;
        if(cb->end_game_flag == 2 || isCheckmate(cb, white) == 8){
            cb->end_game_flag = 2; // Black wins
            return;
        }
        refresh();
        if (cb->selected_piece->coord.rank == 8 && cb->selected_piece->coord.file == 'e') {
            cb->can_castle_black_long = 0;
            cb->can_castle_black_short = 0;
        } else if (cb->selected_piece->coord.rank == 8 && cb->selected_piece->coord.file == 'a') {
            cb->can_castle_black_long = 0;
        } else if (cb->selected_piece->coord.rank == 8 && cb->selected_piece->coord.file == 'h') {
            cb->can_castle_black_short = 0;
        }
        cb->placing = false;
        cb->selected_piece = 0;

    } while (getch() != 27);
        cb->end_game_flag = 3; // nobody wins
}

// Swapping and "deleting" old
void move_piece(chessboard *cb, coordinates next, bool is_taking)
{   
    chess_piece tmp(empty, 1);
    coordinates old_coords, new_coords;

    if(is_taking) {
        cb->piece_graveyard[cb->num_taken] = cb->piece_map[8 - nr][nf - 97];
        cb->piece_map[8 - nr][nf - 97]->type = empty;
    }
    
    tmp = *cb->piece_map[8 - nr][nf - 97];
    // new coords
    old_coords = cb->selected_piece->coord;
    new_coords = cb->piece_map[8 - nr][nf - 97]->coord;
    // new location -- selected piece
    *cb->piece_map[8 - nr][nf - 97] = *cb->selected_piece;
    // new location -- new coords
    cb->piece_map[8 - nr][nf - 97]->coord = new_coords;
    // old location -- swapped piece (hopefully empty)
    *cb->piece_map[8 - old_coords.rank][old_coords.file - 97] = tmp;
    // old location -- old coords
    cb->piece_map[8 - old_coords.rank][old_coords.file - 97]->coord = old_coords;
}

// Swapping and deleting old (for castling - rook and king)
void move_piece(chessboard *cb, coordinates prev, coordinates next)
{
    chess_piece tmp(empty, 1);
    coordinates old_coords, new_coords;
    
    tmp = *cb->piece_map[8 - nr][nf - 97];
    // new coords
    old_coords = prev;
    new_coords = cb->piece_map[8 - nr][nf - 97]->coord;
    // new location -- selected piece
    *cb->piece_map[8 - nr][nf - 97] = *cb->piece_map[8 - prev.rank][prev.file - 97];
    // new location -- new coords
    cb->piece_map[8 - nr][nf - 97]->coord = new_coords;
    // old location -- swapped piece (hopefully empty)
    *cb->piece_map[8 - old_coords.rank][old_coords.file - 97] = tmp;
    // old location -- old coords
    cb->piece_map[8 - old_coords.rank][old_coords.file - 97]->coord = old_coords;
}

int move_check_piece(chessboard *cb)
{
    int y, x;
    char type;
    bool color, is_taking = false;

    y = getcury(stdscr);
    x = getcurx(stdscr);
    // Need to change move_check functions to take y, x, instead of coords.
    color = cb->selected_piece->color;

    if(cb->piece_map[(y - 2) / 2][(x - 4) / 4]->type != empty && (cb->selected_piece->color != cb->piece_map[(y - 2) / 2][(x - 4) / 4]->color))
        is_taking = true;

    switch (type = cb->selected_piece->type) {
        case pawn:
            if (move_check_pawn(cb, cb->selected_piece->coord, cb->piece_map[(y - 2) / 2][(x - 4) / 4]->coord, color, is_taking)) {
                if (is_taking)
                    cb->num_taken++;
                move_piece(cb, cb->piece_map[(y - 2) / 2][(x - 4) / 4]->coord, is_taking);
                return 1;
            }
            break;
        case knight:
            if (move_check_knight(cb, cb->selected_piece->coord, cb->piece_map[(y - 2) / 2][(x - 4) / 4]->coord, is_taking)) {
                if (is_taking)
                    cb->num_taken++;
                move_piece(cb, cb->piece_map[(y - 2) / 2][(x - 4) / 4]->coord, is_taking);
                return 1;
            }
            break;
        case bishop:
            if (move_check_bishop(cb, cb->selected_piece->coord, cb->piece_map[(y - 2) / 2][(x - 4) / 4]->coord, is_taking)) {
                if (is_taking)
                    cb->num_taken++;
                move_piece(cb, cb->piece_map[(y - 2) / 2][(x - 4) / 4]->coord, is_taking);
                return 1;
            }
            break;
        case rook:
            if (move_check_rook(cb, cb->selected_piece->coord, cb->piece_map[(y - 2) / 2][(x - 4) / 4]->coord, is_taking)) {
                if (is_taking)
                    cb->num_taken++;
                move_piece(cb, cb->piece_map[(y - 2) / 2][(x - 4) / 4]->coord, is_taking);
                return 1;
            }
            break;
        case queen:
            if (move_check_queen(cb, cb->selected_piece->coord, cb->piece_map[(y - 2) / 2][(x - 4) / 4]->coord, is_taking)) {
                if (is_taking)
                    cb->num_taken++;
                move_piece(cb, cb->piece_map[(y - 2) / 2][(x - 4) / 4]->coord, is_taking);
                return 1;
            }
            break;
        case king:
            if ((cb->piece_map[(y - 2) / 2][(x - 4) / 4]->coord.file - cb->selected_piece->coord.file == 2 || 
                 cb->selected_piece->coord.file - cb->piece_map[(y - 2) / 2][(x - 4) / 4]->coord.file == 2) && 
                 cb->piece_map[(y - 2) / 2][(x - 4) / 4]->coord.rank - cb->selected_piece->coord.rank == 0) {
                if (color && cb->can_castle_white_long && cb->selected_piece->coord.file - cb->piece_map[(y - 2) / 2][(x - 4) / 4]->coord.file == 2) {
                    if (move_castle(cb, 0, white)) // chessboard - castle version - color
                        return 1;
                } else if (color && cb->can_castle_white_short && cb->piece_map[(y - 2) / 2][(x - 4) / 4]->coord.file - cb->selected_piece->coord.file == 2) {
                    if (move_castle(cb, 1, white)) // chessboard - castle version - color
                        return 1;
                } else if (cb->can_castle_black_long && cb->selected_piece->coord.file - cb->piece_map[(y - 2) / 2][(x - 4) / 4]->coord.file == 2) {
                    if (move_castle(cb, 2, black)) // chessboard - castle version - color
                        return 1;
                } else if (cb->can_castle_black_short && cb->piece_map[(y - 2) / 2][(x - 4) / 4]->coord.file - cb->selected_piece->coord.file == 2) {
                    if (move_castle(cb, 3, black)) // chessboard - castle version - color
                        return 1;
                }
            }
            if (move_check_king(cb, cb->selected_piece->coord, cb->piece_map[(y - 2) / 2][(x - 4) / 4]->coord, color, is_taking, 1)) {
                if (is_taking)
                    cb->num_taken++;
                move_piece(cb, cb->piece_map[(y - 2) / 2][(x - 4) / 4]->coord, is_taking);
                return 1;
            }
            break;
    }
    return 0;
}

int move_castle (chessboard *cb, int version, int color)
{
    switch (version) {
        case 0: // white long
            if (move_check_rook(cb, cb->piece_map[7][0]->coord, cb->piece_map[7][3]->coord, 0)) {
                if (move_check_king(cb, cb->piece_map[7][4]->coord, cb->piece_map[7][3]->coord, white, 0, 1) && 
                    move_check_king(cb, cb->piece_map[7][3]->coord, cb->piece_map[7][2]->coord, white, 0, 1)) {
                    move_piece(cb, cb->piece_map[7][0]->coord, cb->piece_map[7][3]->coord); // Rook
                    move_piece(cb, cb->piece_map[7][4]->coord, cb->piece_map[7][2]->coord); // King
                    return 1;
                }
            }
            break;
        case 1: // white short
            if (move_check_rook(cb, cb->piece_map[7][7]->coord, cb->piece_map[7][5]->coord, 0)) {
                if (move_check_king(cb, cb->piece_map[7][4]->coord, cb->piece_map[7][5]->coord, white, 0, 1) && 
                    move_check_king(cb, cb->piece_map[7][5]->coord, cb->piece_map[7][6]->coord, white, 0, 1)) {
                    move_piece(cb, cb->piece_map[7][7]->coord, cb->piece_map[7][5]->coord); // Rook
                    move_piece(cb, cb->piece_map[7][4]->coord, cb->piece_map[7][6]->coord); // King
                    return 1;
                }
            }
            break; 
        case 2: // black long
            if (move_check_rook(cb, cb->piece_map[0][0]->coord, cb->piece_map[0][3]->coord, 0)) {
                if (move_check_king(cb, cb->piece_map[0][4]->coord, cb->piece_map[0][3]->coord, black, 0, 1) && 
                    move_check_king(cb, cb->piece_map[0][3]->coord, cb->piece_map[0][2]->coord, black, 0, 1)) {
                    move_piece(cb, cb->piece_map[0][0]->coord, cb->piece_map[0][3]->coord); // Rook a8
                    move_piece(cb, cb->piece_map[0][4]->coord, cb->piece_map[0][2]->coord); // King
                    return 1;
                }
            }
            break;
        case 3: // black short
            if (move_check_rook(cb, cb->piece_map[0][7]->coord, cb->piece_map[0][5]->coord, 0)) {
                if (move_check_king(cb, cb->piece_map[0][4]->coord, cb->piece_map[0][5]->coord, black, 0, 1) && 
                    move_check_king(cb, cb->piece_map[0][5]->coord, cb->piece_map[0][6]->coord, black, 0, 1)) {
                    move_piece(cb, cb->piece_map[0][7]->coord, cb->piece_map[0][5]->coord); // Rook h8
                    move_piece(cb, cb->piece_map[0][4]->coord, cb->piece_map[0][6]->coord); // King
                    return 1;
                }
            }
            break;
    }
    return 0;
}

// Returns 1 for can move, 0 for can't move
int move_check_pawn (chessboard *cb, coordinates prev, coordinates next, bool color, bool is_taking)
{
    // Need to check if any pieces are in the way
    if (color) { // White
        if (pr == 2) {  // If on second rank
            if (nr_pr == 2 && nf == pf && cb->piece_map[8 - nr][nf - 97]->type == empty && 
                cb->piece_map[8 - nr - 1][nf - 97]->type == empty) { // Move 2 forwards
                return 1;
            } else if (nr_pr == 1 && nf == pf && cb->piece_map[8 - nr][nf - 97]->type == empty) { // Move 1 forwards
                return 1;
            } else if (nr_pr == 1 && is_taking) {
                if (nf > pf && nf_pf == 1) { // Take diag right
                    return 1;
                } else if (nf < pf && pf_nf == 1) { // Take diag left
                    return 1;
                }
            }
        } else if (nr_pr == 1 && nf == pf && cb->piece_map[8 - nr][nf - 97]->type == empty) { // Move 1 forwards
            return 1;
        } else if (nr_pr == 1 && is_taking) {
            if (nf > pf && nf_pf == 1) { // Take diag right
                return 1;
            } else if (nf < pf && pf_nf == 1) { // Take diag left
                return 1;
            }
        }
    }
    
    if (!color) { // Black
        if (pr == 7) { // Move 2 forwards
            if (pr_nr == 2 && nf == pf && cb->piece_map[8 - nr][nf - 97]->type == empty && 
                cb->piece_map[8 - nr - 1][nf - 97]->type == empty) {
                return 1;
            } else if (pr_nr == 1 && nf == pf && cb->piece_map[8 - nr][nf - 97]->type == empty) {
                return 1;
            } else if (pr_nr == 1 && is_taking) {
                if (nf > pf && nf_pf == 1) { // Take diag right (perspective)
                    return 1;
                } else if (nf < pf && pf_nf == 1) { // Take diag left (perspective)
                    return 1;
                }
            }
        } else if (pr_nr == 1 && nf == pf && cb->piece_map[8 - nr][nf - 97]->type == empty) { // Move 1 forwards
            if (nf > pf && nf_pf == 1) { // Take diag right (perspective)
                return 1;
            } else if (pr_nr == 1 && is_taking) {
                if (nf > pf && nf_pf == 1) { // Take diag right (perspective)
                    return 1;
                } else if (nf < pf && pf_nf == 1) { // Take diag left (perspective)
                    return 1;
                }
            }
            return 1;
        } else if (pr_nr == 1 && is_taking) {
            if (nf > pf && nf_pf == 1) { // Take diag right (perspective)
                return 1;
            } else if (nf < pf && pf_nf == 1) { // Take diag left (perspective)
                return 1;
            }
        }
    }
    return 0;
}

int move_check_knight (chessboard *cb, coordinates prev, coordinates next, bool is_taking)
{
    if (!is_taking) {
        if (cb->piece_map[8 - nr][nf - 97]->type != empty)
            return 0;
    }
    
    if (nr_pr == 2 && nf_pf == 1) {
        return 1;
    } else if (nr_pr == 2 && pf_nf == 1) {
        return 1;
    } else if (pr_nr == 2 && nf_pf == 1) {
        return 1;
    } else if (pr_nr == 2 && pf_nf == 1) {
        return 1;
    } else if (nf_pf == 2 && nr_pr == 1) {
        return 1;
    } else if (nf_pf == 2 && pr_nr == 1) {
        return 1;
    } else if (pf_nf == 2 && nr_pr == 1) {
        return 1;
    } else if (pf_nf == 2 && pr_nr == 1 ) {
        return 1;
    }
    return 0;
}

int move_check_bishop (chessboard *cb, coordinates prev, coordinates next, bool is_taking) 
{
    int i;
    if ((nf_pf != 0 && nr_pr != 0) || (cb->piece_map[8 - pr][pf - 97]->type == 'Q' && (nf_pf != 0 && nr_pr != 0))) {
        if (pr_nr > 0 && pf_nf > 0 && pr_nr == pf_nf){ //down left
            for (i = 1; i <= pr_nr; i++){
                if(cb->piece_map[8 - pr + i][(pf - 97) - i]->type != empty){
                    if(is_taking && i == pr_nr){
                        return 1;
                    }
                    return 0;
                }
            }
            return 1;
        } else if (pr_nr > 0 && nf_pf > 0 && pr_nr == nf_pf) { //down right
            for (i = 1; i <= pr_nr; i++) {
                if (cb->piece_map[8 - pr + i][i + (pf - 97)]->type != empty) {
                    if (is_taking && i == pr_nr) {
                        return 1;
                    }
                    return 0;
                }
            }
            return 1;
        } else if (nr_pr > 0 && pf_nf > 0 && nr_pr == pf_nf) { //up left
            for (i = 1; i <= nr_pr; i++) {
                if (cb->piece_map[8 - pr - i][(pf - 97) - i]->type != empty) {
                    if (is_taking && i == nr_pr) {
                        return 1;
                    }
                    return 0;
                }
            }
            return 1;
        } else if (nr_pr > 0 && nf_pf > 0 && nr_pr == nf_pf) { //up right
            for (i = 1; i <= nr_pr; i++) {
                if (cb->piece_map[8 - pr - i][i + (pf - 97)]->type != empty) {
                    if (is_taking && i == nr_pr) {
                        return 1;
                    }
                    return 0;
                }
            }
            return 1;
        }
    }
    return 0;
}

int move_check_rook (chessboard *cb, coordinates prev, coordinates next, bool is_taking) 
{
    int i;
    if (nf_pf == 0 || nr_pr == 0 || (cb->piece_map[8 - pr][pf - 97]->type == 'Q' && (nf_pf == 0 || nr_pr == 0))) {
        if (pr_nr > 0) { // down
            for (i = 1; i <= pr_nr; i++) {
                if (cb->piece_map[8 - pr + i][pf - 97]->type != empty) {
                    if (is_taking && i == pr_nr) {
                        return 1;
                    }
                    return 0;
                }
            }
            return 1;
        } else if (nr_pr > 0) { // up
            for (i = 1; i <= nr_pr; i++) {
                if (cb->piece_map[8 - pr - i][pf - 97]->type != empty) {
                    if (is_taking && i == nr_pr) {
                        return 1;
                    }
                    return 0;
                }
            }
            return 1;
        } else if (pf_nf > 0) { // left
            for (i = 1; i <= pf_nf; i++) {
                if (cb->piece_map[8 - pr][pf - i - 97]->type != empty) {
                    if (is_taking && i == pf_nf) {
                        return 1;
                    }
                    return 0;
                }
            }
            return 1;
        } else if (nf_pf > 0) { // right
            for (i = 1; i <= nf_pf; i++) {
                if (cb->piece_map[8 - pr][pf + i - 97]->type != empty) {
                    if (is_taking && i == nf_pf) {
                        return 1;
                    }
                    return 0;
                }
            }
            return 1;
        }
    }
    return 0;
}

int move_check_queen (chessboard *cb, coordinates prev, coordinates next, bool is_taking) 
{
    if (move_check_rook(cb, prev, next, is_taking))
        return 1;
    else 
        return (move_check_bishop(cb, prev, next, is_taking));
}

int move_check_king (chessboard *cb, coordinates prev, coordinates next, bool color, bool is_taking, bool is_moving) 
{
    int y, x;
    //nr_pr > 0 up
    //pr_nr > 0 down
    //nf_pf > 0 right
    //pf_nf > 0 left
    // if ((up or down) and (possibly right or possibly left)) or ((right or left) and (possibly up or possibly down))
    if (cb->piece_map[8 - nr][nf - 97]->type != empty && !is_taking ) {
        return 0;
    }
        // up, down, right, left, down left, up left, down right, up right
    if ( (nr_pr == 1 && nf_pf == 0) || (pr_nr == 1 && nf_pf == 0) || (nf_pf == 1 && pr_nr == 0) || (pf_nf == 1 && pr_nr == 0) || 
         (pf_nf == 1 && pr_nr == 1) || (pf_nf == 1 && nr_pr == 1) || (nf_pf == 1 && pr_nr == 1) || (nf_pf == 1 && nr_pr == 1)) {
        if (color && is_moving) {//checks for if moving into check
            for (y = 0; y < 8; y++) {
                for (x = 0; x < 8; x++) {
                    if (cb->piece_map[y][x]->type != empty) {
                        if (!cb->piece_map[y][x]->color) {
                            if (cb->piece_map[y][x]->type == 'p')
                                if (move_check_pawn(cb, cb->piece_map[y][x]->coord, next, 0, 1))
                                    return 0;
                            if (cb->piece_map[y][x]->type == 'R')
                                if (move_check_rook(cb, cb->piece_map[y][x]->coord, next, 1))
                                    return 0;
                            if (cb->piece_map[y][x]->type == 'N')
                                if (move_check_knight(cb, cb->piece_map[y][x]->coord, next, 1))
                                    return 0;
                            if (cb->piece_map[y][x]->type == 'B')
                                if (move_check_bishop(cb, cb->piece_map[y][x]->coord, next, 1))
                                    return 0;
                            if (cb->piece_map[y][x]->type == 'Q')
                                if (move_check_queen(cb, cb->piece_map[y][x]->coord, next, 1))
                                    return 0;
                            if (cb->piece_map[y][x]->type == 'K')
                                if (move_check_king(cb, cb->piece_map[y][x]->coord, next, 0, 1, 0))
                                    return 0;
                        }
                    }
                }
            }
        } else if (is_moving) {
            for (y = 0; y < 8; y++) {
                for (x = 0; x < 8; x++) {
                    if (cb->piece_map[y][x]->type != empty) {
                        if (cb->piece_map[y][x]->color) {
                            if (cb->piece_map[y][x]->type == 'p')
                                if (move_check_pawn(cb, cb->piece_map[y][x]->coord, next, 1, 1))
                                    return 0;
                            if (cb->piece_map[y][x]->type == 'R')
                                if (move_check_rook(cb, cb->piece_map[y][x]->coord, next, 1))
                                    return 0;
                            if (cb->piece_map[y][x]->type == 'N')
                                if (move_check_knight(cb, cb->piece_map[y][x]->coord, next, 1))
                                    return 0;
                            if (cb->piece_map[y][x]->type == 'B')
                                if (move_check_bishop(cb, cb->piece_map[y][x]->coord, next, 1))
                                    return 0;
                            if (cb->piece_map[y][x]->type == 'Q')
                                if (move_check_queen(cb, cb->piece_map[y][x]->coord, next, 1))
                                    return 0;
                            if (cb->piece_map[y][x]->type == 'K')
                                if (move_check_king(cb, cb->piece_map[y][x]->coord, next, 1, 1, 0))
                                    return 0;
                        }
                    }
                }
            }
        }
        return 1;
    }
    return 0;
}

void count_tot_pieces(chessboard *cb)
{
    int y, x, i;

    for (i = 0; i < 6; i++) {
        cb->white_pieces[i] = 0;
        cb->black_pieces[i] = 0;
    }

    for (y = 0; y < 8; y++) {
        for (x = 0; x < 8; x++) {
            if (cb->piece_map[y][x]->type != empty) {
                if (cb->piece_map[y][x]->color) {
                    if (cb->piece_map[y][x]->type == 'p')
                        cb->white_pieces[0]++;
                    if (cb->piece_map[y][x]->type == 'R')
                        cb->white_pieces[1]++;
                    if (cb->piece_map[y][x]->type == 'N')
                        cb->white_pieces[2]++;
                    if (cb->piece_map[y][x]->type == 'B')
                        cb->white_pieces[3]++;
                    if (cb->piece_map[y][x]->type == 'Q')
                        cb->white_pieces[4]++;
                    if (cb->piece_map[y][x]->type == 'K')
                        cb->white_pieces[5]++;
                } else {
                    if (cb->piece_map[y][x]->type == 'p')
                        cb->black_pieces[0]++;
                    if (cb->piece_map[y][x]->type == 'R')
                        cb->black_pieces[1]++;
                    if (cb->piece_map[y][x]->type == 'N')
                        cb->black_pieces[2]++;
                    if (cb->piece_map[y][x]->type == 'B')
                        cb->black_pieces[3]++;
                    if (cb->piece_map[y][x]->type == 'Q')
                        cb->black_pieces[4]++;
                    if (cb->piece_map[y][x]->type == 'K')
                        cb->black_pieces[5]++;
                }
            }
        }
    }
}


int isCheckmate(chessboard *cb, bool colorP){// pass in color of the king 
    int i, j, k;
    int y, x;
    int state = 0;
    std::vector<coordinates> tryToTake;
    std::vector<coordinates> tryToBlock;
    std::vector<coordinates> availTakers;
    std::vector<coordinates> availBlockers;
    std::vector<coordinates> LOS; // line of sight
    std::vector<std::vector<coordinates>> availMoves;
    // 0 is king can move
    // 1 is in check can move
    // 2 is in check cant move
    // 3 is in check cant move can take
    // 4 is in check cant move can block
    // 5 is in check cant move can take & block
    // 6 is in check cant move cant block can take
    // 7 is in check cant move cant take can block
    // 8 is checkmate

    for(i = 0; i < 8; i++){
        for(j = 0; j < 8; j++){
            if(cb->piece_map[i][j]->color == colorP){// search for king
                if(cb->piece_map[i][j]->type == 'K'){
                    y = i;
                    x = j;
                }
            }
        }
    }

    for(i = 0; i < 8; i++){// checks for if in check
        for(j = 0; j < 8; j++){
            if(cb->piece_map[i][j]->color != colorP){
               if(cb->piece_map[i][j]->type == 'p'){
                    if(move_check_pawn(cb, cb->piece_map[i][j]->coord, cb->piece_map[y][x]->coord, !colorP, true) == true){
                        tryToBlock.push_back(cb->piece_map[i][j]->coord); // adds pieces that put king in check
                        tryToTake.push_back(cb->piece_map[i][j]->coord);
                        state = 1;
                    }
                }
                if(cb->piece_map[i][j]->type == 'R'){
                    if(move_check_rook(cb, cb->piece_map[i][j]->coord, cb->piece_map[y][x]->coord, true) == true){
                        tryToBlock.push_back(cb->piece_map[i][j]->coord);
                        tryToTake.push_back(cb->piece_map[i][j]->coord);
                        state = 1;
                    }
                }
                if(cb->piece_map[i][j]->type == 'N'){
                    if(move_check_knight(cb, cb->piece_map[i][j]->coord, cb->piece_map[y][x]->coord, true) == true){
                        tryToBlock.push_back(cb->piece_map[i][j]->coord);
                        tryToTake.push_back(cb->piece_map[i][j]->coord);
                        state = 1;
                    }
                }
                if(cb->piece_map[i][j]->type == 'B'){
                    if(move_check_bishop(cb, cb->piece_map[i][j]->coord, cb->piece_map[y][x]->coord, true) == true){
                        tryToBlock.push_back(cb->piece_map[i][j]->coord);
                        tryToTake.push_back(cb->piece_map[i][j]->coord);
                        state = 1;
                    }
                }
                if(cb->piece_map[i][j]->type == 'Q'){
                    if(move_check_queen(cb, cb->piece_map[i][j]->coord, cb->piece_map[y][x]->coord, true) == true){
                        tryToBlock.push_back(cb->piece_map[i][j]->coord);
                        tryToTake.push_back(cb->piece_map[i][j]->coord);
                        state = 1;
                    }
                }
            }
        }
    }

    if (state == 1){
        for(i = -1; i < 3; i++){// checks for if king can move
            for(j = -1; j < 3; j++){
                if(j != 0 && i != 0){
                    if(0 < i + y && i + y < 8 && 0 < j + x && j + x < 8){
                        if(move_check_king(cb, cb->piece_map[y][x]->coord, cb->piece_map[y + i][x + j]->coord, colorP, true, true) == false && 
                           move_check_king(cb, cb->piece_map[y][x]->coord, cb->piece_map[y + i][x + j]->coord, colorP, false, true) == false){
                            state = 2;
                        }
                    }
                }
            }
        }
    }

    if(state == 2 && (int)tryToTake.size() == 1){//try to take
        for(i = 0; i < 8; i++){
            for(j = 0; j < 8; j++){
                if(cb->piece_map[i][j]->color == colorP){
                    if(cb->piece_map[i][j]->type == 'p'){
                        if(move_check_pawn(cb, cb->piece_map[i][j]->coord, tryToTake[0], colorP, true) == true){
                            availTakers.push_back(cb->piece_map[i][j]->coord);
                        }
                    }
                    if(cb->piece_map[i][j]->type == 'R'){
                        if(move_check_rook(cb, cb->piece_map[i][j]->coord, tryToTake[0], true) == true){
                            availTakers.push_back(cb->piece_map[i][j]->coord);
                        }
                    }
                    if(cb->piece_map[i][j]->type == 'N'){
                        if(move_check_knight(cb, cb->piece_map[i][j]->coord, tryToTake[0], true) == true){
                            availTakers.push_back(cb->piece_map[i][j]->coord);
                        }
                    }
                    if(cb->piece_map[i][j]->type == 'B'){
                        if(move_check_bishop(cb, cb->piece_map[i][j]->coord, tryToTake[0], true) == true){
                            availTakers.push_back(cb->piece_map[i][j]->coord);
                        }
                    }
                    if(cb->piece_map[i][j]->type == 'Q'){
                        if(move_check_queen(cb, cb->piece_map[i][j]->coord, tryToTake[0], true) == true){
                            availTakers.push_back(cb->piece_map[i][j]->coord);
                        }
                    }
                }
            }
        }

        if((int)tryToTake.size() == 1){
            tryToTake.pop_back();
        }
    }

    if(state == 2 && (int)tryToBlock.size() == 1){//checks for line of sight spaces between taker and king
        for(i = 0; i < 8; i++){
            for(j = 0; j < 8; j++){
                if(cb->piece_map[8 - tryToBlock[0].rank][tryToBlock[0].file - 97]->type == 'B'){
                    if (move_check_bishop(cb, tryToBlock[0], cb->piece_map[i][j]->coord, true)) {
                            if((8 - tryToBlock[0].rank - y)/abs(8 - tryToBlock[0].rank - y) == (8 - tryToBlock[0].rank - i)/abs(8 - tryToBlock[0].rank - i) 
                            && (tryToBlock[0].file - x - 97)/abs(tryToBlock[0].file - x - 97) == (tryToBlock[0].file - j - 97)/abs(tryToBlock[0].file - j - 97)){
                                if(sqrt(pow(8 - tryToBlock[0].rank - y, 2) + pow(tryToBlock[0].file - x - 97, 2)) > sqrt(pow(8 - tryToBlock[0].rank - i, 2) + pow(tryToBlock[0].file - j - 97, 2))){
                                    LOS.push_back(cb->piece_map[i][j]->coord);
                                }
                            }
                        }
                }
                if(cb->piece_map[8 - tryToBlock[0].rank][tryToBlock[0].file - 97]->type == 'R'){
                    if (move_check_rook(cb, tryToBlock[0], cb->piece_map[i][j]->coord, true)) {
                            if((8 - tryToBlock[0].rank - y)/abs(8 - tryToBlock[0].rank - y) == (8 - tryToBlock[0].rank - i)/abs(8 - tryToBlock[0].rank - i) 
                            && (tryToBlock[0].file - x - 97)/abs(tryToBlock[0].file - x - 97) == (tryToBlock[0].file - j - 97)/abs(tryToBlock[0].file - j - 97)){
                                if(sqrt(pow(8 - tryToBlock[0].rank - y, 2) + pow(tryToBlock[0].file - x - 97, 2)) > sqrt(pow(8 - tryToBlock[0].rank - i, 2) + pow(tryToBlock[0].file - j - 97, 2))){
                                    LOS.push_back(cb->piece_map[i][j]->coord);
                                }
                            }
                        }
                }
                if(cb->piece_map[8 - tryToBlock[0].rank][tryToBlock[0].file - 97]->type == 'Q'){
                    if(move_check_bishop(cb, tryToBlock[0], cb->piece_map[y][x]->coord, true)){
                        if (move_check_bishop(cb, tryToBlock[0], cb->piece_map[i][j]->coord, true)) {
                            if((8 - tryToBlock[0].rank - y)/abs(8 - tryToBlock[0].rank - y) == (8 - tryToBlock[0].rank - i)/abs(8 - tryToBlock[0].rank - i) 
                            && (tryToBlock[0].file - x - 97)/abs(tryToBlock[0].file - x - 97) == (tryToBlock[0].file - j - 97)/abs(tryToBlock[0].file - j - 97)){
                                if(sqrt(pow(8 - tryToBlock[0].rank - y, 2) + pow(tryToBlock[0].file - x - 97, 2)) > sqrt(pow(8 - tryToBlock[0].rank - i, 2) + pow(tryToBlock[0].file - j - 97, 2))){
                                    LOS.push_back(cb->piece_map[i][j]->coord);
                                }
                            }
                        }
                    }else if(move_check_rook(cb, tryToBlock[0], cb->piece_map[y][x]->coord, true)){
                        if (move_check_rook(cb, tryToBlock[0], cb->piece_map[i][j]->coord, true)) {
                            if((8 - tryToBlock[0].rank - y)/abs(8 - tryToBlock[0].rank - y) == (8 - tryToBlock[0].rank - i)/abs(8 - tryToBlock[0].rank - i) 
                            && (tryToBlock[0].file - x - 97)/abs(tryToBlock[0].file - x - 97) == (tryToBlock[0].file - j - 97)/abs(tryToBlock[0].file - j - 97)){
                                if(sqrt(pow(8 - tryToBlock[0].rank - y, 2) + pow(tryToBlock[0].file - x - 97, 2)) > sqrt(pow(8 - tryToBlock[0].rank - i, 2) + pow(tryToBlock[0].file - j - 97, 2))){
                                    LOS.push_back(cb->piece_map[i][j]->coord);
                                }
                            }
                        }
                    }
                }
            }
        }

        if((int)tryToBlock.size() == 1){
            tryToBlock.pop_back();
        }
    }

    if(state == 2){
        
        for(i = 0; i < (int)LOS.size(); i++){
            availMoves.push_back(std::vector<coordinates>());
            availMoves[i].push_back(LOS[i]);
        }
        
        for(i = 0; i < (int)availMoves.size(); i++) {//checks for pieces to move into line of sight between piece checking king and king
            for(j = 0; j < 8; j++){
                for(k = 0; k < 8; k++){
                    if(cb->piece_map[j][k]->color == colorP){
                        if(cb->piece_map[j][k]->type == 'p'){
                            if(move_check_pawn(cb, cb->piece_map[j][k]->coord, availMoves[i][0], colorP, false)){
                                availMoves[i].push_back(cb->piece_map[j][k]->coord);
                            }
                        }
                        if(cb->piece_map[j][k]->type == 'B'){
                            if(move_check_bishop(cb, cb->piece_map[j][k]->coord, availMoves[i][0], false)){
                                availMoves[i].push_back(cb->piece_map[j][k]->coord);
                            }
                        }
                        if(cb->piece_map[j][k]->type == 'R'){
                            if(move_check_rook(cb, cb->piece_map[j][k]->coord, availMoves[i][0], false)){
                                availMoves[i].push_back(cb->piece_map[j][k]->coord);
                            }
                        }
                        if(cb->piece_map[j][k]->type == 'N'){
                            if(move_check_knight(cb, cb->piece_map[j][k]->coord, availMoves[i][0], false)){
                                availMoves[i].push_back(cb->piece_map[j][k]->coord);
                            }
                        }
                        if(cb->piece_map[j][k]->type == 'Q'){
                            if(move_check_queen(cb, cb->piece_map[j][k]->coord, availMoves[i][0], false)){
                                availMoves[i].push_back(cb->piece_map[j][k]->coord);
                            }
                        }
                    }
                }
            }
        }
    }
    int moves = 0;
    for(i = 0; i < (int)availMoves.size(); i++){
        for(j = 0; j < (int)availMoves[i].size(); j++){
            if(j != 0){
                moves++;
            }
        }
    }

    if((int)availTakers.size() != 0){
        state = 3;
    }

    if(moves!= 0 && state == 3){
        state = 4;
    }

    if(moves != 0 && state == 2){
        state = 5;
    }

    if(moves == 0 && state == 3){
        state = 6;
    }

    if((int)availTakers.size() == 0 && state == 4){
        state = 7;
    }

    if((int)availTakers.size() == 0 && moves == 0 && state != 0){
        state = 8;
    }
    
    return state;
}