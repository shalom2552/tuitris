#include "tetromino.h"

#include "color.h"
#include "board.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define UNREACHABLE(...) do { fprintf(stderr, "UNREACHABLE: \n"__VA_ARGS__); abort(); } while (0)
// === Defines ================================================================
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

typedef struct {
    Pos           pos;
    TetrominoType type;
    Shape         shape;
    Color         color;
} Tetromino ;

// === Variables ==============================================================
// Shape position is relative to tetromino position
static const Shape INITIAL_SHAPES[TETROMINO_COUNT] = {
    [Straight] = { .blocks = { { 0,  -1 }, { 0, 0 }, { 0, 1  }, { 0, 2} } },
    [Square]   = { .blocks = { { 0,  0  }, { 0, 1 }, { 1, 0  }, { 1, 1  } } },
    [T]        = { .blocks = { { 0,  -1 }, { 0, 0 }, { 0, 1  }, { 1, 0  } } },
    [L]        = { .blocks = { { -1, 0  }, { 0, 0 }, { 1, 0  }, { 1, 1  } } },
    [ReverseL] = { .blocks = { { -1, 0  }, { 0, 0 }, { 1, 0  }, { 1, -1 } } },
    [Z]        = { .blocks = { { -1, 0  }, { 0, 0 }, { 0, 1  }, { 1, 1  } } },
    [ReverseZ] = { .blocks = { { -1, 0  }, { 0, 0 }, { 0, -1 }, { 1, -1 } } },
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

static Tetromino t;
static Tetromino next;
static Tetromino hold;

bool has_hold;
bool holded;

// === Helper Functions =======================================================
/* Returns the position of the i'th block */
static Pos block_pos(int i)
{
    return (Pos){ .y = t.pos.y + t.shape.blocks[i].y, .x = t.pos.x + t.shape.blocks[i].x };
}

/* Returns 1 if can place the tetromino, 0 otherwise */
static int can_place(void)
{
    for (int i = 0; i < SHAPE_SIZE; ++i) {
        Pos p = block_pos(i);
        if (!board_is_free(p.y, p.x)) {
            return 0;
        }
    }
    return 1;
}

/* Adds the tetromino to the board */
static bool place_tetromino(void)
{
    if (!can_place()) return false;
    for (int i = 0; i < SHAPE_SIZE; ++i) {
        Pos p = block_pos(i);
        board_set(p.y, p.x, t.color);
    }
    return true;
}

/* Removes the tetromino from the board */
static void remove_tetromino(void)
{
    for (int i = 0; i < SHAPE_SIZE; ++i) {
        Pos p = block_pos(i);
        board_remove(p.y, p.x);
    }
}

/* Returns 1 if the tetromino can move by delta, 0 otherwise */
static bool can_move(int dy, int dx)
{
    for (int i = 0; i < SHAPE_SIZE; ++i) {
        Pos p = block_pos(i);
        if (!board_is_free(p.y + dy, p.x + dx)) {
            return false;
        }
    }
    return true;
}

/* Move the tetromino by given delta */
static void move(int dy, int dx)
{
    remove_tetromino();
    if (can_move(dy, dx)) {
        t.pos.y += dy;
        t.pos.x += dx;
    }
    place_tetromino();
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
    for (int i = 0; i < SHAPE_SIZE; ++i) {
        Pos p = t.shape.blocks[i];
        rotate_block(&p, dir);
        if (!board_is_free(t.pos.y + p.y, t.pos.x + p.x)) {
            return false;
        }
    }
    return true;
}

/* Rotates the tetromino around its center. */
static void rotate(int dir)
{
    if (t.type == Square) return; // no-op
    if (can_rotate(dir)) {
        for (int i = 0; i < SHAPE_SIZE; ++i) {
            rotate_block(&t.shape.blocks[i] , dir);
        }
    }
}

static void create_tetromino(Tetromino* t, TetrominoType type){
    t->pos.y = 1; t->pos.x = 4;
    t->type = type;
    t->shape = INITIAL_SHAPES[t->type];
    t->color = TYPE_COLOR[t->type];
    if (rand() % 2) {
        for (int i = 0; i < SHAPE_SIZE; ++i) {
            rotate_block(&t->shape.blocks[i], 1);
        }
    }
}

static void create_random_tetromino(Tetromino* t){
    create_tetromino(t, rand() % TETROMINO_COUNT);
}

/* Creates the next tetromino */
static void next_tetromino(void)
{
    holded = false;
    create_random_tetromino(&next);
}

// === Public API =============================================================
void tetromino_init(void)
{
    has_hold = false;
    holded   = false;
    next_tetromino();
}

bool tetromino_create(void)
{
    t = next;
    next_tetromino();
    return place_tetromino();
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
    place_tetromino();
}

void tetromino_rotate_left(void)
{
    remove_tetromino();
    rotate(-1);
    place_tetromino();
}

void tetromino_hold(void)
{
    if (holded){
        return;
    }
    remove_tetromino();

    Tetromino previous_hold = hold;
    create_tetromino(&hold, t.type);

    t = has_hold ? previous_hold : next;
    if (!has_hold){
        next_tetromino();
        has_hold = true;
    }

    place_tetromino();
    holded = true;
}

void tetromino_hard_drop(void)
{
    while(true){
        if (tetromino_locked()){
            return;
        }
        move(1, 0);
    }
    UNREACHABLE("tetromino_hard_drop");
}

bool tetromino_has_hold(void)
{
    return has_hold;
}

bool tetromino_holded(void)
{
    return holded;
}

bool tetromino_locked(void)
{
    remove_tetromino();
    int locked = !can_move(1, 0);
    place_tetromino();
    return locked;
}

TetrominoPeek tetromino_peek_next(void)
{
    TetrominoPeek peek;
    peek.color = next.color;
    peek.shape = next.shape;
    return peek;
}

TetrominoPeek tetromino_peek_hold(void)
{
    TetrominoPeek peek;
    peek.color = hold.color;
    peek.shape = hold.shape;
    return peek;
}

