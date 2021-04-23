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
        io_move_cursor(&cb, 0);
    } while (getch() != 27);

    end_terminal();
    return 0;
}