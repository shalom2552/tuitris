#include "board.h"
#include "tdraw.h"
#include "color.h"

// === Defines ================================================================

#define BLOCK_EMPTY "  "
#define BLOCK_FILL "██"
#define BAR_FILL "████████████████████████"
#define BOARD_START_POS_Y(h) (h - BOARD_HIGHT) / 2 + 1
#define BOARD_START_POS_X(w) (w - BOARD_WIDTH * 2) / 2 + 1

// === Globals ================================================================

/* Board data structure. */
struct Cell { int free; Color color; };
static struct Cell board[BOARD_HIGHT][BOARD_WIDTH];

// === Helper Functions =======================================================

/* Returns 1 if the position is outside of the board */
static int invalid_pos(int y, int x)
{
    return (y < 0 || y >= BOARD_HIGHT || x < 0 || x >= BOARD_WIDTH);
}

/* Draws the border of the board. */
static void draw_board_border(void)
{
    int h; int w; tdraw_term_size(&h, &w);
    tdraw_draw_at(BOARD_START_POS_Y(h) - 1, BOARD_START_POS_X(w) - 2, BAR_FILL C_RESET);
    for (int i = 0; i < BOARD_HIGHT; ++i) {
        tdraw_draw_at(BOARD_START_POS_Y(h) + i, BOARD_START_POS_X(w) - 2, BLOCK_FILL C_RESET);
        tdraw_draw_at(BOARD_START_POS_Y(h) + i, BOARD_START_POS_X(w) + BOARD_WIDTH * 2, BLOCK_FILL C_RESET);
    }
    tdraw_draw_at(BOARD_START_POS_Y(h) + BOARD_HIGHT, BOARD_START_POS_X(w) - 2, BAR_FILL C_RESET);
}

/* Draws the cells of the board. */
static void draw_board_cells(void)
{
    int h; int w; tdraw_term_size(&h, &w);
    for (int row = 0; row < BOARD_HIGHT; row++) {
        for (int col = 0; col < BOARD_WIDTH; col++) {
            int y = row + BOARD_START_POS_Y(h);
            int x = col * 2 + BOARD_START_POS_X(w);
            if (board[row][col].free) {
                tdraw_draw_at(y, x, BLOCK_EMPTY);
            } else {
                char* color = color_code(board[row][col].color);
                tdraw_draw_at(y, x, "%s%s%s", color, BLOCK_FILL, C_RESET);
            }
        }
    }
}

/* clear the screen only if terminal size changed */
static void clear_screen(void)
{
    static int last_h = -1; static int last_w = -1;
    int h; int w; tdraw_term_size(&h, &w);
    if (last_h != h || last_w != w) {
        last_h = h; last_w = w;
        tdraw_clear();
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

void board_draw(void)
{
    clear_screen();
    draw_board_border();
    draw_board_cells();
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

