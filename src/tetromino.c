#include "tetromino.h"

#include "color.h"
#include "board.h"

#include <stdlib.h>

// === Defines ================================================================

#define BLOCKS_COUNT 4

typedef struct { int x; int y; } Pos;
typedef struct { Pos blocks[BLOCKS_COUNT]; } Shape;

static const Shape INITIAL_SHAPES[TETROMINO_COUNT] = {
    [Straight] = { .blocks = { { 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 } } },
    [Square]   = { .blocks = { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 1, 2 } } },
    [T]        = { .blocks = { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 1 } } },
    [L]        = { .blocks = { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 1 } } },
    [ReverseL] = { .blocks = { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 1 } } },
    [Z]        = { .blocks = { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 1 } } },
    [ReverseZ] = { .blocks = { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 1 } } },
};

static const Color TYPE_COLOR[TETROMINO_COUNT] = {
    [Straight] = RED,
    [Square]   = YELLOW,
    [T]        = CYAN,
    [L]        = GREEN,
    [ReverseL] = BLUE,
    [Z]        = MAGENTA,
    [ReverseZ] = WHITE,
};

// === Variables ==============================================================

struct Tetromino {
    TetrominoType type;
    Shape shape;
    Color color;
};
static Tetromino t;


// === Helper Functions =======================================================

static void draw_tetromino(void)
{
    for (int i = 0; i < BLOCKS_COUNT; i++) {
        Pos pos = t.shape.blocks[i];
        board_set(pos.y, pos.x, t.color);
    }
}

static void remove_tetromino(void)
{
    for (int i = 0; i < BLOCKS_COUNT; i++) {
        Pos pos = t.shape.blocks[i];
        board_remove(pos.y, pos.x);
    }
}

// === Public API =============================================================

void tetromino_create(void)
{
    t.type = rand() % TETROMINO_COUNT;
    t.shape = INITIAL_SHAPES[t.type];
    t.color = TYPE_COLOR[t.type];
    draw_tetromino();
}

void tetromino_move(void)
{
    remove_tetromino();
    for (int i = 0; i < BLOCKS_COUNT; i++) {
        t.shape.blocks[i].y++;
    }
    draw_tetromino();
}

