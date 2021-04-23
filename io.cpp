#include <curses.h>

#include "io.h"
#include "chessboard.h"
#include "printer.h"

int io_select_piece(chessboard *cb, int y, int x)
{
    if (cb->piece_map[(y - 2) / 2][(x - 5) / 4])
        return 1;
    else 
        return 0;
}

int io_check_color(chessboard *cb, int y, int x)
{
    return cb->piece_map[(y - 2) / 2][(x - 5) / 4]->color;
}

void io_move_cursor(chessboard *cb, int is_selected)
{
    bool finished = false;
    int key;
    
    do {
        int y = getcury(stdscr);
        int x = getcurx(stdscr);

        switch (key = getch()) {
            case KEY_UP:
            case 'w':
                //mvaddch(y, x, " ");
                move(y - 1, x);
                if (is_selected) {
                    print_board(cb);
                    addch(cb->piece_map[(y - 2) / 2][(x - 5) / 4]->type);
                }
                refresh();
                break;
            case KEY_RIGHT:
            case 'd':
                move(y, x + 1);
                if (is_selected) 
                    addch(cb->piece_map[(y - 2) / 2][(x - 5) / 4]->type);
                refresh();
                break;
            case KEY_DOWN:
            case 's':
                move(y + 1, x);
                if (is_selected) 
                    addch(cb->piece_map[(y - 2) / 2][(x - 5) / 4]->type);
                refresh();
                break;
            case KEY_LEFT:
            case 'a':
                move(y, x - 1);
                if (is_selected) 
                    addch(cb->piece_map[(y - 2) / 2][(x - 5) / 4]->type);
                refresh();
                break;
            case 10:
                // Select a piece
                if (io_select_piece(cb, y, x))
                    if (io_check_color(cb, y, x))
                        io_move_cursor(cb, 1);
                    //else thing

                break;
        }
        
    } while (key != 27 && !finished);
}

void init_terminal()
{
    initscr();
    raw();
    noecho();
    curs_set(0);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_WHITE);
}

void end_terminal()
{
    endwin();
}