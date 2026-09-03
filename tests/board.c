#include "board.h"

#include "color.h"

#include <assert.h>
#include <stdio.h>

// === HELPERS ================================================================
static int print_board(void)
{
    printf("Board:\n");
    for (int row = 0; row < BOARD_ROWS; ++row) {
        for (int col = 0; col < BOARD_COLS; ++col) {
            if (board_is_free(row, col)) {
                printf("_");
            } else {
                printf("X");
            }
        }
        printf("\n");
    }
    return 0;
}

static int is_board_empty(void)
{
    for (int row = 0; row < BOARD_ROWS; ++row) {
        for (int col = 0; col < BOARD_COLS; ++col) {
            if (!board_is_free(row, col) && board_get_color(row, col) != NONE) {
                return 0;
            }
        }
    }
    return 1;
}

// === TESTS ================================================================
static void test_board_init(void)
{
    board_init();
    assert(is_board_empty());
}

static void test_board_set(void)
{
    int y = 0;
    int x = 0;
    board_init();
    board_set(y, x, GREEN);
    assert(board_get_color(y, x) == GREEN);
    assert(!board_is_free(y, x));
    for (int row = 0; row < BOARD_ROWS; ++row) {
        for (int col = 0; col < BOARD_COLS; ++col) {
            if (row != y && col != x) {
                assert(board_is_free(row, col));
                assert(board_get_color(row, col) == NONE);
            }
        }
    }
}

static void test_board_set_overide(void)
{
    int y = 0;
    int x = 0;
    board_init();
    board_set(y, x, RED);
    assert(board_get_color(y, x) == RED);
    board_set(y, x, GREEN);
    assert(board_get_color(y, x) == GREEN);
}

static void test_board_remove(void)
{
    int y = 0;
    int x = 0;
    board_init();
    board_set(y, x, GREEN);
    board_remove(y, x);
    assert(is_board_empty());
}

static void test_board_is_free(void)
{
    int y = 0;
    int x = 0;
    board_init();
    board_set(y, x, GREEN);
    assert(!board_is_free(y, x));
    board_remove(y, x);
    assert(board_is_free(y, x));
}

static void test_board_get_color(void)
{
    int y = 0;
    int x = 0;
    board_init();
    board_set(y, x, GREEN);
    assert(board_get_color(y, x) == GREEN);
    board_set(y, x, RED);
    assert(board_get_color(y, x) == RED);
    board_remove(y, x);
    assert(board_get_color(y, x) == NONE);
}

static void test_board_clear_lines(void)
{
    int row = BOARD_ROWS - 1;
    board_init();
    for (int col = 0; col < BOARD_COLS; ++col) {
        board_set(col, row, RED);
        assert(!board_is_free(col, row));
    }
    board_clear_lines();
    assert(is_board_empty());
}

static void test_board_clear_lines_moves_down(void)
{
    int y = 1;
    int x = 1;
    int row = BOARD_ROWS - 1;
    board_init();
    for (int col = 0; col < BOARD_COLS; ++col) {
        board_set(row, col, RED);
        assert(!board_is_free(col, row));
    }
    board_set(y, x, GREEN);
    board_clear_lines();
    assert(board_is_free(y, x) || print_board());
    assert(!board_is_free(y + 1, x) || print_board());
}

static void test_board_bad_input(void)
{
    int y1 = -1;
    int x1 = -1;
    int y2 = BOARD_ROWS;
    int x2 = BOARD_COLS;
    board_init();
    board_set(y1, x1, GREEN);
    board_set(y2, x2, GREEN);
    assert(is_board_empty());
    board_remove(y1, x1);
    board_remove(y2, x2);
    assert(is_board_empty());
    board_get_color(y1, x1);
    board_get_color(y2, x2);
    board_is_free(y1, x1);
    board_is_free(y2, x2);
}

// === Main ===================================================================
void test_board(void)
{
    test_board_init();
    test_board_set();
    test_board_set_overide();
    test_board_remove();
    test_board_is_free();
    test_board_get_color();
    test_board_clear_lines();
    test_board_clear_lines_moves_down();
    test_board_bad_input();
}

