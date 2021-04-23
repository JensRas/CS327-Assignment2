

#include "move.h"
#include "chessboard.h"

void move_turn ()
{
    
}
// Returns 1 for can move, 0 for can't move
int move_check_pawn (chessboard *cb, coordinates prev, coordinates next, bool color, bool is_taking)
{
    // Need to check if any pieces are in the way
    if (color) { // White
        if (pr == 2) { // Move 2 forwards
            if (nr_pr <= 2 && nr_pr > 0 && nf == pf && 
               !cb->piece_map[nr][nf - 97] && !cb->piece_map[nr - 1][nf - 97])
                return 1;
        } else if (nr_pr == 1 && nf == pf && !cb->piece_map[nr][nf - 97]) { // Move 1 forwards
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
            if (pr_nr <= 2 && pr_nr > 0 && nf == pf && 
               !cb->piece_map[nr][nf - 97] && !cb->piece_map[nr - 1][nf - 97])
                return 1;
        } else if (pr_nr == 1 && nf == pf && !cb->piece_map[nr][nf - 97]) { // Move 1 forwards
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
        if (cb->piece_map[nr][nf - 97])
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
    if ((nf_pf != 0 && nr_pr != 0) || cb->piece_map[pr][pf - 97]->type == 'Q') {
        if (pr_nr > 0 && pf_nf > 0){ //down left
            for (i = 0; i <= pr_nr; i++){
                if (pr - i) {
                    if(cb->piece_map[i + pr][i + (pf - 97)]){
                        if(is_taking && i == pr_nr){
                            return 1;
                        }
                        return 0;
                    }
                }
            }
            return 1;
        } else if (pr_nr > 0 && nf_pf > 0) { //down right
            for (i = 0; i <= pf_nf; i++) {
                if (pf - i) {
                    if (cb->piece_map[i + pr][i + (pf - 97)]) {
                        if (is_taking && i == pr_nr) {
                            return 1;
                        }
                        return 0;
                    }
                }
            }
            return 1;
        } else if (nr_pr > 0 && pf_nf > 0) { //up left
            for (i = 0; i <= nr_pr; i++) {
                if (pr - i) {
                    if (cb->piece_map[i + pr][i + (pf - 97)]) {
                        if (is_taking && i == pr_nr) {
                            return 1;
                        }
                        return 0;
                    }
                }
            }
            return 1;
        } else if (nr_pr > 0 && nf_pf > 0) { //up right
            for (i = 0; i <= nf_pf; i++) {
                if (pf - i){
                    if (cb->piece_map[i + pr][i + (pf - 97)]) {
                        if (is_taking && i == pr_nr) {
                            return 1;
                        }
                        return 0;
                    }
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
    if (nf_pf == 0 || nr_pr == 0 || cb->piece_map[pr][pf - 97]->type == 'Q') {
        if (pr_nr > 0) { // down
            for (i = 0; i <= pr_nr; i++) {
                if (pr - i){
                    if (cb->piece_map[pr + i][pf - 97]) {
                        if (is_taking && i == pr_nr) {
                            return 1;
                        }
                        return 0;
                    }
                }
            }
            return 1;
        } else if (nr_pr > 0) { //up
            for (i = 0; i <= nr_pr; i++) {
                if (pr + i) {
                    if (cb->piece_map[pr + i][pf - 97]) {
                        if (is_taking && i == nr_pr) {
                            return 1;
                        }
                        return 0;
                    }
                }
            }
            return 1;
        } else if (pf_nf > 0) { // left
            for (i = 0; i <= pf_nf; i++) {
                if (pf - i) {
                    if (cb->piece_map[pr][pf + i - 97]) {
                        if (is_taking && i == pf_nf) {
                            return 1;
                        }
                        return 0;
                    }
                }
            }
            return 1;
        } else if (nf_pf > 0) { //right
            for (i = 0; i <= nf_pf; i++) {
                if (pf + i) {
                    if (cb->piece_map[pr][pf + i - 97]) {
                        if (is_taking && i == nf_pf) {
                            return 1;
                        }
                        return 0;
                    }
                }
            }
            return 1;
        }
    }
    return 0;
}

int move_check_queen (chessboard *cb, coordinates prev, coordinates next, bool is_taking) 
{
    if (move_check_rook(cb, prev, next, is_taking)) // 1
        return (move_check_bishop(cb, prev, next, is_taking));
    return 0;
}

int move_check_king (chessboard *cb, coordinates prev, coordinates next, bool color, bool is_taking) 
{
    int y, x;

    if(((nr_pr == 1 || pr_nr == 1) && (nf_pf <= 1 || pf_nf <= 1)) || ((nf_pf == 1 || pf_nf == 1) && (pr_nr <= 1 || nr_pr <= 1))) {
        if (color) {//checks for if moving into check
            for (y = 0; y < 8; y++) {
                for (x = 0; x < 8; x++) {
                    if (cb->piece_map[y][x]) {
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
                                if (move_check_king(cb, cb->piece_map[y][x]->coord, next, 0, 1))
                                    return 0;
                        }
                    }
                }
            }
        } else {
            for (y = 0; y < 8; y++) {
                for (x = 0; x < 8; x++) {
                    if (cb->piece_map[y][x]) {
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
                                if (move_check_king(cb, cb->piece_map[y][x]->coord, next, 1, 1))
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
            if (cb->piece_map[y][x]) {
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
