#include "ui.h"

#include "color.h"
#include "tdraw.h"
#include "board.h"

#include <string.h>

// === Defines ================================================================

#define PANEL_WIDTH 14
#define BOARD_HIGHT (BOARD_ROWS + 2)
#define BOARD_WIDTH (BOARD_COLS * 2 + 2)

#define DRAW_START_Y(h) (((h) - BOARD_HIGHT) / 2)
#define DRAW_START_X(w) (((w) - BOARD_WIDTH - PANEL_WIDTH) / 2)

#define BLOCK_EMPTY "  "
#define BLOCK_FILL "██"
#define BAR_FILL "████████████████████████"

// === Variables ==============================================================

static int y;
static int x;

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

/* Draws the border of the board */
static void draw_board_border(void)
{
    tdraw_draw_at(y + 1, x + 1, BAR_FILL C_RESET);
    for (int i = 0; i < BOARD_ROWS; ++i) {
        tdraw_draw_at(y + 2 + i, x + 1, BLOCK_FILL C_RESET);
        tdraw_draw_at(y + 2 + i, x + 3 + BOARD_COLS * 2, BLOCK_FILL C_RESET);
    }
    tdraw_draw_at(y + 2 + BOARD_ROWS, x + 1, BAR_FILL C_RESET);
}

/* Draws the cells of the board */
static void draw_board_cells(void)
{
    for (int row = 0; row < BOARD_ROWS; row++) {
        for (int col = 0; col < BOARD_COLS; col++) {
            if (board_is_free(row, col)) {
                tdraw_draw_at(y + 2 + row, 3 + x + 2 * col, BLOCK_EMPTY);
            } else {
                char* color = color_code(board_get_color(row, col));
                tdraw_draw_at(y + 2 + row, 3 + x + 2 * col, "%s%s%s", color, BLOCK_FILL, C_RESET);
            }
        }
    }
}

/* Draw main frame and title */
static void draw_frame(void)
{
    tdraw_draw_frame(y, x, y + BOARD_HIGHT + 1, x + BOARD_WIDTH + PANEL_WIDTH + 2);
    // char* title = C_CYAN "Tuitris" C_RESET;
    // int pos_x = (BOARD_WIDTH + PANEL_WIDTH) / 2 - strlen("Tuitris") / 2;
    // tdraw_draw_at(y, x + pos_x, title);
}

/* Draw board and panel frames */
static void draw_frames(void)
{
    int preview_size = PANEL_WIDTH / 2 - 2;
    int preview_y = y + 2; int preview_x = x + BOARD_WIDTH + 4;
    tdraw_draw_frame(preview_y, preview_x, preview_y + preview_size, preview_x + 2 * preview_size);
    tdraw_draw_at(preview_y - 1, preview_x + 3, "Next:");
}

// === Public API =============================================================

void ui_draw(void)
{
    int h; int w; tdraw_term_size(&h, &w);
    y = DRAW_START_Y(h);
    x = DRAW_START_X(w);

    clear_screen();

    draw_frame();
    draw_frames();
    draw_board_border();
    draw_board_cells();

    tdraw_flush();
}

void ui_validate(void)
{
    int require_y = BOARD_HIGHT + 2;
    int require_x = BOARD_WIDTH + PANEL_WIDTH + 4;
    while (!tdraw_term_size_ok(require_y, require_x)) {
        tdraw_delay(10);
    }
}
