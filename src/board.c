#include "board.h"
#include "tdraw.h"
#include "color.h"

#define REWQUIRE_HEIGHT BOARD_HIGHT + 4
#define REWQUIRE_WIDTH (BOARD_WIDTH + 4) * 2

static const char* BLOCK_FILL = "██";
static const char* BLOCK_EMPTY = "  ";

typedef struct {
    int free;
    Color color;
} Cell;

static Cell board[BOARD_HIGHT][BOARD_WIDTH];

/* Draws the border of the board. */
static void draw_board_border(int term_height, int term_width);

void board_init(void)
{
    for (int i = 0; i < BOARD_HIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            board[i][j].free = 1;
            board[i][j].color = NONE;
        }
    }
}

static void draw_board_border(int term_height, int term_width)
{
    int top = (term_height - BOARD_HIGHT) / 2 + 1;
    int left = (term_width - BOARD_WIDTH * 2) / 2 + 1;

    tdraw_draw_at(top - 1, left - 2, "████████████████████████"); // top row
    for (int i = 0; i < BOARD_HIGHT; ++i) {
        tdraw_draw_at(top + i, left - 2, "██                    ██");
    }
    tdraw_draw_at(top + BOARD_HIGHT, left - 2, "████████████████████████"); // bottom row
}

void board_draw(void)
{
    int term_height = tdraw_terminal_height();
    int term_width = tdraw_terminal_width();

    if (!tdraw_size_ok(term_height, term_width, REWQUIRE_HEIGHT, REWQUIRE_WIDTH)) {
        return;
    }

    tdraw_clear();
    draw_board_border(term_height, term_width);
    for (int row = 0; row < BOARD_HIGHT; row++) {
        for (int col = 0; col < BOARD_WIDTH; col++) {
            int y = row + (term_height - BOARD_HIGHT) / 2 + 1;
            int x = col * 2 + (term_width - BOARD_WIDTH * 2) / 2 + 1;
            if (board[row][col].free) {
                tdraw_draw_at(y, x, BLOCK_EMPTY);
            } else {
                char* color = Colors[board[row][col].color];
                tdraw_draw_at(y, x, "%s%s%s", color, BLOCK_FILL, C_RESET);
            }
        }
    }
}

void board_set(int y, int x, Color color)
{
    board[y][x].free = 0;
    board[y][x].color = color;
}

void board_remove(int y, int x)
{
    board[y][x].free = 1;
    board[y][x].color = NONE;
}

int board_is_free(int y, int x)
{
    return board[y][x].free;
}

