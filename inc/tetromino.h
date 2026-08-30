#ifndef TETROMINO_H
#define TETROMINO_H

#include <stdbool.h>

typedef enum {
    Straight,
    Square,
    T,
    L,
    ReverseL,
    Z,
    ReverseZ,
    TETROMINO_COUNT,
} TetrominoType;

typedef struct Tetromino Tetromino;

/* Initialize a new tetromino with random type 
 * Returns 0 on success, 1 on failure */
bool tetromino_create(void);

/* Moves the tetromino down one row if it can */
void tetromino_move_down(void);

/* Moves the tetromino left one column if it can */
void tetromino_move_left(void);

/* Moves the tetromino right one column if it can */
void tetromino_move_right(void);

/* Rotates the tetromino clockwise if it can */
void tetromino_rotate_right(void);

/* Rotates the tetromino counterclockwise if it can */
void tetromino_rotate_left(void);

/* Returns 1 if the tetromino is locked in place, 0 otherwise */
bool tetromino_locked(void);

#endif // !TETROMINO_H

