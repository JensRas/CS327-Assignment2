

#include "move.h"
#include "chessboard.h"

void move_turn ()
{
    
}
// Returns 1 for can move, 0 for can't move
int move_pawn (chessboard *cb, coordinates prev, coordinates next, bool color, bool is_taking)
{
    // Need to check if any pieces are in the way
    if (color) { // White
        if (prev.rank == 2) { // Move 2 forwards
            if(next.rank - prev.rank <= 2 && next.rank - prev.rank > 0 && next.file == prev.file)
                return 1;
        } else if (next.rank - prev.rank == 1 && next.file == prev.file) { // Move 1 forwards
            return 1;
        } else if (next.rank - prev.rank == 1 && is_taking) {
            if(next.file > prev.file && next.file - prev.file == 1) { // Take diag right
                return 1;
            } else if (next.file < prev.file && prev.file - next.file == 1) { // Take diag left
                return 1;
            }
        }
    }
    
    if (!color) { // Black
        if(prev.rank == 7) { // Move 2 forwards
            if(prev.rank - next.rank <= 2 && prev.rank - next.rank > 0 && next.file == prev.file)
                return 1;
        } else if (prev.rank - next.rank == 1 && next.file == prev.file) { // Move 1 forwards
            
        } else if () {
            if () { // Take diag right (perspective)

            } else if () { // Take diag left (perspective)

            }
        }
    }
    return 0;
}

void move_knight (chessboard *cb, coordinates prev, coordinates next) 
{
    
}

void move_bishop (chessboard *cb, coordinates prev, coordinates next) 
{
    
}

void move_rook (chessboard *cb, coordinates prev, coordinates next) 
{
    
}

void move_queen (chessboard *cb, coordinates prev, coordinates next) 
{
    
}

void move_king (chessboard *cb, coordinates prev, coordinates next) 
{
    
}