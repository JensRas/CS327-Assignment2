#ifndef IO_H
#define IO_H

class chessboard;

int io_select_piece(chessboard *cb, int y, int x);
int io_check_color(chessboard *cb, int y, int x);
void io_move_cursor(chessboard *cb, int is_selected, int y, int x);
void init_terminal();
void end_terminal();

#endif