#include "tetromino.h"

#include "color.h"
#include "board.h"

#include <stdlib.h>

// === Defines ================================================================

#define BLOCKS_COUNT 4

typedef struct { int y; int x; } Pos;
typedef struct { Pos blocks[BLOCKS_COUNT]; } Shape;

// Shape position is relative to tetromino position
static const Shape INITIAL_SHAPES[TETROMINO_COUNT] = {
    [Straight] = { .blocks = { { 0, -1 }, { 0, 0 }, { 0, 1 }, { 0, 2} } },
    [Square]   = { .blocks = { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } } },
    [T]        = { .blocks = { { 0, -1 }, { 0, 0 }, { 0, 1 }, { 1, 0 } } },
    [L]        = { .blocks = { { -1, 0 }, { 0, 0 }, { 1, 0 }, { 1, 1 } } },
    [ReverseL] = { .blocks = { { -1, 0 }, { 0, 0 }, { 1, 0 }, { 1, -1 } } },
    [Z]        = { .blocks = { { -1, 0 }, { 0, 0 }, { 0, 1 }, { 1, 1 } } },
    [ReverseZ] = { .blocks = { { -1, 0 }, { 0, 0 }, { 0, -1 }, { 1, -1 } } },
};

static const Color TYPE_COLOR[TETROMINO_COUNT] = {
    [Straight] = RED,
    [Square]   = YELLOW,
    [T]        = CYAN,
    [L]        = GREEN,
    [ReverseL] = GREEN,
    [Z]        = MAGENTA,
    [ReverseZ] = MAGENTA,
};

// === Variables ==============================================================

struct Tetromino {
    Pos pos;
    TetrominoType type;
    Shape shape;
    Color color;
};
static Tetromino t;


// === Helper Functions =======================================================

/* Returns the position of the i'th block. */
static Pos block_pos(int i)
{
    return (Pos){ .y = t.pos.y + t.shape.blocks[i].y, .x = t.pos.x + t.shape.blocks[i].x };
}

/* Adds the tetromino to the board. */
static void add_tetromino(void)
{
    for (int i = 0; i < BLOCKS_COUNT; ++i) {
        Pos p = block_pos(i);
        board_set(p.y, p.x, t.color);
    }
}

/* Removes the tetromino from the board. */
static void remove_tetromino(void)
{
    for (int i = 0; i < BLOCKS_COUNT; ++i) {
        Pos p = block_pos(i);
        board_remove(p.y, p.x);
    }
}

/* Returns 1 if the tetromino can move down, 0 otherwise. */
static int can_move(int dy, int dx)
{
    for (int i = 0; i < BLOCKS_COUNT; ++i) {
        Pos p = block_pos(i);
        if (!board_is_free(p.y + dy, p.x + dx)) {
            return 0;
        }
    }
    return 1;
}

static void move(int dy, int dx)
{
    remove_tetromino();
    if (can_move(dy, dx)) {
        t.pos.y += dy;
        t.pos.x += dx;
    }
    add_tetromino();
}

/* Rotates a block around its center by a given direction (1 or -1). */
static void rotate_block(Pos* pos, int dir)
{
    int x = pos->x;
    int y = pos->y;
    pos->x = -y * dir;
    pos->y = x * dir;
}

/* Returns 1 if the tetromino can rotate, 0 otherwise. */
static int can_rotate(int dir)
{
    for (int i = 0; i < BLOCKS_COUNT; ++i) {
        Pos p = t.shape.blocks[i];
        rotate_block(&p, dir);
        if (!board_is_free(t.pos.y + p.y, t.pos.x + p.x)) {
            return 0;
        }
    }
    return 1;
}

/* Rotates the tetromino around its center. */
static void rotate(int dir)
{
    if (t.type == Square) return; // no-op
    if (can_rotate(dir)) {
        for (int i = 0; i < BLOCKS_COUNT; ++i) {
            rotate_block(&t.shape.blocks[i] , dir);
        }
    }
}

// === Public API =============================================================

void tetromino_create(void)
{
    t.pos.y = 0; t.pos.x = 4;
    t.type = rand() % TETROMINO_COUNT;
    t.shape = INITIAL_SHAPES[t.type];
    t.color = TYPE_COLOR[t.type];
    if (rand() % 2) rotate(1);
    add_tetromino(); // TODO: check if can create
}

void tetromino_move_down(void)
{
    move(1, 0);
}

void tetromino_move_right(void)
{
    move(0, 1);
}

void tetromino_move_left(void)
{
    move(0, -1);
}

void tetromino_rotate_right(void)
{
    remove_tetromino();
    rotate(1);
    add_tetromino();
}

void tetromino_rotate_left(void)
{
    remove_tetromino();
    rotate(-1);
    add_tetromino();
}

int tetromino_locked(void)
{
    return can_move(1, 0);
}
