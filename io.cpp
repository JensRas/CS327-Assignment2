#include <curses.h>

#include "io.h"
#include "move.h"
#include "chessboard.h"
#include "printer.h"

int io_select_piece(chessboard *cb, int y, int x)
{
    if (cb->whose_turn == cb->piece_map[(y - 2) / 2][(x - 4) / 4]->color) {
        if (cb->piece_map[(y - 2) / 2][(x - 4) / 4]->type != empty) {
            cb->selected_piece = cb->piece_map[(y - 2) / 2][(x - 4) / 4];
            return 1;
        } else 
            return 0;
    }
    return 0;
}

int io_check_color(chessboard *cb, int y, int x)
{
    return cb->piece_map[(y - 2) / 2][(x - 4) / 4]->color;
}

void io_move_cursor(chessboard *cb, int is_selected, int y, int x)
{
    int key;
    char my_cursor;
    int sel_piece_y = y;
    int sel_piece_x = x;

    if (is_selected) {
        my_cursor = cb->piece_map[(y - 2) / 2][(x - 4) / 4]->type;
    }
    move(y, x);

    do {
        y = getcury(stdscr);
        x = getcurx(stdscr);

        switch (key = getch()) {
            case KEY_UP:
            case 'w':
                if (y == 2) // Check bounds
                    break;
                if (cb->piece_map[(y - 2) / 2][(x - 4) / 4]->type != empty) {
                    if (!cb->piece_map[(y - 2) / 2][(x - 4) / 4]->color)
                        attron(COLOR_PAIR(1));
                    mvaddch(y, x, cb->piece_map[(y - 2) / 2][(x - 4) / 4]->type);
                    if (!cb->piece_map[(y - 2) / 2][(x - 4) / 4]->color)
                        attroff(COLOR_PAIR(1));
                } else 
                    mvaddch(y, x, ' ');
                move(y - 2, x);
                if (is_selected) {
                    mvaddch(sel_piece_y, sel_piece_x, ' ');
                    if (!cb->selected_piece->color)
                        attron(COLOR_PAIR(1));
                    mvaddch(y - 2, x, my_cursor);
                    if (!cb->selected_piece->color)
                        attroff(COLOR_PAIR(1));
                    move(y - 2, x);
                }
                refresh();
                break;
            case KEY_RIGHT:
            case 'd':
                if (x == 32) // Check bounds
                    break;
                if (cb->piece_map[(y - 2) / 2][(x - 4) / 4]->type != empty) {
                    if (!cb->piece_map[(y - 2) / 2][(x - 4) / 4]->color)
                        attron(COLOR_PAIR(1));
                    mvaddch(y, x, cb->piece_map[(y - 1) / 2][(x - 4) / 4]->type);
                    if (!cb->piece_map[(y - 2) / 2][(x - 4) / 4]->color)
                        attroff(COLOR_PAIR(1));
                } else 
                    mvaddch(y, x, ' ');
                move(y, x + 4);
                if (is_selected) {
                    mvaddch(sel_piece_y, sel_piece_x, ' ');
                    if (!cb->selected_piece->color)
                        attron(COLOR_PAIR(1));
                    mvaddch(y, x + 4, my_cursor);
                     if (!cb->selected_piece->color)
                        attroff(COLOR_PAIR(1));
                    move(y, x + 4);
                }
                refresh();
                break;
            case KEY_DOWN:
            case 's':
                if (y == 16) // Check bounds
                    break;
                if (cb->piece_map[(y - 2) / 2][(x - 4) / 4]->type != empty) {
                    if (!cb->piece_map[(y - 2) / 2][(x - 4) / 4]->color)
                        attron(COLOR_PAIR(1));
                    mvaddch(y, x, cb->piece_map[(y - 2) / 2][(x - 4) / 4]->type);
                    if (!cb->piece_map[(y - 2) / 2][(x - 4) / 4]->color)
                        attroff(COLOR_PAIR(1));
                } else 
                    mvaddch(y, x, ' ');
                move(y + 2, x);
                if (is_selected) {
                    mvaddch(sel_piece_y, sel_piece_x, ' ');
                    if (!cb->selected_piece->color)
                        attron(COLOR_PAIR(1));
                    mvaddch(y + 2, x, my_cursor);
                     if (!cb->selected_piece->color)
                        attroff(COLOR_PAIR(1));
                    move(y + 2, x);
                }
                refresh();
                break;
            case KEY_LEFT:
            case 'a':
                if (x == 4) // Check bounds
                    break;
                if (cb->piece_map[(y - 2) / 2][(x - 4) / 4]->type != empty) {
                    if (!cb->piece_map[(y - 2) / 2][(x - 4) / 4]->color)
                        attron(COLOR_PAIR(1));
                    mvaddch(y, x, cb->piece_map[(y - 1) / 2][(x - 4) / 4]->type);
                    if (!cb->piece_map[(y - 2) / 2][(x - 4) / 4]->color)
                        attroff(COLOR_PAIR(1));
                } else 
                    mvaddch(y, x, ' ');
                move(y, x - 4);
                if (is_selected) {
                    mvaddch(sel_piece_y, sel_piece_x, ' ');
                    if (!cb->selected_piece->color)
                        attron(COLOR_PAIR(1));
                    mvaddch(y, x - 4, my_cursor);
                     if (!cb->selected_piece->color)
                        attroff(COLOR_PAIR(1));
                    move(y, x - 4);
                }
                refresh();
                break;
            case 10:
                if(is_selected) {
                    if (move_check_piece(cb)) {
                        cb->placing = true;
                        break;
                    }
                    else 
                        break;
                }
                else if (io_select_piece(cb, y, x)) {
                        io_move_cursor(cb, 1, y, x);
                }
                break;
        }
        
    } while (key != 27 && !cb->placing);
    refresh();

    if(key == 27 && !is_selected) {
        cb->end_game_flag = 3; // Nobody wins
    } else if (key == 27 && is_selected) {
        if(cb->piece_map[(y - 2) / 2][(x - 4) / 4]->type != empty)
            mvaddch(y, x, cb->piece_map[(y - 2)  / 2][(x - 4) / 4]->type);
        else 
            mvaddch(y, x, ' ');
        refresh();   
        mvaddch(((8 - cb->selected_piece->coord.rank) * 2 + 2), ((cb->selected_piece->coord.file - 97) * 4) + 4, cb->selected_piece->type);
        move(((8 - cb->selected_piece->coord.rank) * 2 + 2), ((cb->selected_piece->coord.file - 97) * 4) + 4);
    }
}

void init_terminal()
{
    initscr();
    raw();
    noecho();
    curs_set(1);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_WHITE);
}

void end_terminal()
{
    endwin();
}