#ifndef TETROMINO_H
#define TETROMINO_H

typedef enum {
    Straight,
    Square,
    T,
    L,
    ReverseL,
    Z,
    ReverseZ
} TetrominoType;

typedef struct {
    TetrominoType type;
    int rotation;
} Tetromino;

#endif // !TETROMINO_H

