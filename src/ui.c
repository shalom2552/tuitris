#include "ui.h"

#include "color.h"
#include "tdraw.h"
#include "board.h"

// === Defines ================================================================

#define REQUIRE_HEIGHT (BOARD_HIGHT + 4)
#define REQUIRE_WIDTH (BOARD_WIDTH * 2 + 4 + BOARD_WIDTH + 4)

#define BOARD_START_POS_Y(h) (((h) - BOARD_HIGHT) / 2 + 1)
#define BOARD_START_POS_X(w) (((w) - BOARD_WIDTH * 2) / 2 + 1 - BOARD_WIDTH / 2)

#define BLOCK_EMPTY "  "
#define BLOCK_FILL "██"
#define BAR_FILL "████████████████████████"

// === Variables ==============================================================

// === Helper Functions =======================================================

/* Clear the screen only if terminal size changed */
static void clear_screen(void)
{
    static int last_h = -1; static int last_w = -1;
    int h; int w; tdraw_term_size(&h, &w);
    if (last_h != h || last_w != w) {
        last_h = h; last_w = w;
        tdraw_clear();
    }
}

/* Draws the border of the board. */
static void draw_board_border(int y, int x)
{
    tdraw_draw_at(y - 1, x - 2, BAR_FILL C_RESET);
    for (int i = 0; i < BOARD_HIGHT; ++i) {
        tdraw_draw_at(y + i, x - 2, BLOCK_FILL C_RESET);
        tdraw_draw_at(y + i, x + BOARD_WIDTH * 2, BLOCK_FILL C_RESET);
    }
    tdraw_draw_at(y + BOARD_HIGHT, x - 2, BAR_FILL C_RESET);
}

/* Draws the cells of the board. */
static void draw_board_cells(int y, int x)
{
    for (int row = 0; row < BOARD_HIGHT; row++) {
        for (int col = 0; col < BOARD_WIDTH; col++) {
            if (board_is_free(row, col)) {
                tdraw_draw_at(y + row, x + 2 * col, BLOCK_EMPTY);
            } else {
                char* color = color_code(board_get_color(row, col));
                tdraw_draw_at(y + row, x + 2 * col, "%s%s%s", color, BLOCK_FILL, C_RESET);
            }
        }
    }
}

// === Public API =============================================================

void ui_draw(void)
{
    clear_screen(); // conditional
    int h; int w; tdraw_term_size(&h, &w);
    int y = BOARD_START_POS_Y(h);
    int x = BOARD_START_POS_X(w);

    tdraw_draw_frame(y - 2, x - 3, y + BOARD_HIGHT + 1, x + 3 * BOARD_WIDTH + 2);
    // tdraw_draw_frame(y - 2, x - 3, y + BOARD_HIGHT + 1, x + 4 * BOARD_WIDTH + 2);
    draw_board_border(y, x);
    draw_board_cells(y, x);
    tdraw_flush();
}

void ui_validate(void)
{
    while (!tdraw_term_size_ok(REQUIRE_HEIGHT, REQUIRE_WIDTH)) {
        tdraw_delay(10);
    }
}
