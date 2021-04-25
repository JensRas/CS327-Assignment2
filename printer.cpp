#include <curses.h>

#include "printer.h"
#include "chessboard.h"

/* #define board "  ---------------------------------\n" +
              "8 |   |   |   |   |   |   |   |   |\n" +
              "  ---------------------------------\n" +
              "7 |   |   |   |   |   |   |   |   |\n" +
              "  ---------------------------------\n" +
              "6 |   |   |   |   |   |   |   |   |\n" +
              "  ---------------------------------\n" +
              "5 |   |   |   |   |   |   |   |   |\n" +
              "  ---------------------------------\n" +
              "4 |   |   |   |   |   |   |   |   |\n" +
              "  ---------------------------------\n" +
              "3 |   |   |   |   |   |   |   |   |\n" +
              "  ---------------------------------\n" +
              "2 |   |   |   |   |   |   |   |   |\n" +
              "  ---------------------------------\n" +
              "1 |   |   |   |   |   |   |   |   |\n" + 
              "  ---------------------------------\n" +
              "    a   b   c   d   e   f   g   h  " */

void print_board(chessboard *cb)
{
    int y, x;
    int sum = 8;

    mvprintw(0, 0, "MAKE YOUR MOVE!");

    for (y = 2; y < 19; y++) {
        if (y % 2 == 0) {
            mvprintw(y - 1, 0, "  ---------------------------------");
        }
        for (x = 2; x < 35; x++) {
            if (x == 2 && y % 2 == 1) {
                mvprintw(y - 1, 0, "%d", sum--);
            }
            if (x % 4 == 2) {
                mvprintw(y, x, "|");
            } else { // x = 5, 9, 13, 17, 21, 25, 29, 33       // y = 2, 4, 6, 8, 10, 12, 14, 16
                // Initial print of pieces
                if ((x == 5 || x == 9 || x == 13 || x == 17 || x == 21 || x == 25 || x == 29 || x == 33) && y % 2 == 0 && cb->piece_map[(y - 1) / 2][(x - 4) / 4] != 0) {
                    if(cb->piece_map[(y - 1) / 2][(x - 4) / 4]->color)
                        attron(COLOR_PAIR(1));
                    mvprintw(y, x - 1, "%c", cb->piece_map[(y - 1) / 2][(x - 4) / 4]->type);
                    if(cb->piece_map[(y - 1) / 2][(x - 4) / 4]->color)
                        attroff(COLOR_PAIR(1));
                    refresh();
                } else {
                    // TODO Color white squares white
                    /* if( ) 
                        attron(COLOR_PAIR(2));
                    mvprintw(y, x, " ");
                    if(x % 3 == 0 || x % 5 == 0)
                        attroff(COLOR_PAIR(2)); */
                }                
            }
        }
    }
    
    mvprintw(18, 0, "    a   b   c   d   e   f   g   h  ");
    refresh();
}