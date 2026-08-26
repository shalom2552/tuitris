#ifndef TETROMINO_H
#define TETROMINO_H

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

/* Initialize a new tetromino with random type */
void tetromino_create(void);

/* Moves the tetromino down one row */
void tetromino_move(void);

#endif // !TETROMINO_H

