#ifndef TETROMINO_H
#define TETROMINO_H

#include "color.h"
#include <stdbool.h>

#define SHAPE_SIZE 4

typedef struct { int y; int x; } Pos;
typedef struct { Pos blocks[SHAPE_SIZE]; } Shape;

typedef struct {
    Shape shape;
    Color color;
} TetrominoPeek;

/* Initialize the first tetromino */
void tetromino_init(void);

/* Spawns the next tetromino, returns 0 on success, 1 on failure */
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

/* Returns peak of the next tetromino */
TetrominoPeek tetromino_peek_next(void);

#endif // !TETROMINO_H

