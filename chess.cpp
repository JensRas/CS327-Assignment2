#include <cstdio>
#include <cstdlib>
#include <curses.h>

#include "chessboard.h"
#include "printer.h"
#include "move.h"
#include "io.h"

int main(int argc, char *argv[])
{
    chessboard cb;

    cb_place_pieces(&cb);

    init_terminal();
    
    print_board(&cb);

    do {
        move_turn(&cb);
    } while (!cb.end_game_flag);

    end_terminal();
    return 0;
}