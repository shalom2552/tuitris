#include "board.h"

#include "color.h"
#include "state.h"

#include <stdbool.h>

// === Variables ==============================================================

/* Board data structure. */
struct Cell {
    int free;
    Color color;
};
static struct Cell board[BOARD_HIGHT][BOARD_WIDTH];

// === Helper Functions =======================================================

/* Returns 1 if the position is outside of the board */
static int invalid_pos(int y, int x)
{
    return (y < 0 || y >= BOARD_HIGHT || x < 0 || x >= BOARD_WIDTH);
}

/* Returns true if the given row is full */
static bool full_row(int row)
{
    for (int i = 0; i < BOARD_WIDTH; ++i) {
        if (board_is_free(row, i)) {
            return false;
        }
    }
    return true;
}

/* Collapses the given row down with all above rows */
static void collapse_row(int row)
{
    for (int i = row; i > 0; --i) {
        for (int j = 0; j < BOARD_WIDTH; ++j) {
            board[i][j] = board[i - 1][j];
        }
    }
}

// === Public API =============================================================

void board_init(void)
{
    for (int i = 0; i < BOARD_HIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            board[i][j].free = 1;
            board[i][j].color = NONE;
        }
    }
}

void board_set(int y, int x, Color color)
{
    if (invalid_pos(y, x)) return;
    board[y][x].free = 0;
    board[y][x].color = color;
}

void board_remove(int y, int x)
{
    if (invalid_pos(y, x)) return;
    board[y][x].free = 1;
    board[y][x].color = NONE;
}

int board_is_free(int y, int x)
{
    if (invalid_pos(y, x)) return 0;
    return board[y][x].free;
}

Color board_get_color(int y, int x)
{
    if (invalid_pos(y, x)) return NONE;
    return board[y][x].color;
}

void board_clear_lines(void)
{
    for (int row = 0; row < BOARD_HIGHT; ++row) {
        if (full_row(row)) {
            collapse_row(row);
            state_add_score(10);
        }
    }
}

