#include "ui.h"

#include "color.h"
#include "game.h"
#include "tdraw.h"
#include "board.h"
#include "state.h"
#include "tetromino.h"

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

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

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
}

/* Draw board and panel frames */
static void draw_next_preview(void)
{
    int preview_size = PANEL_WIDTH / 2 - 2;
    int preview_y = y + 2; int preview_x = x + BOARD_WIDTH + 4;
    tdraw_draw_frame(preview_y, preview_x, preview_y + preview_size, preview_x + 2 * preview_size - 1);
    tdraw_draw_at(preview_y, preview_x + 3, "Next:");
    TetrominoPeek peek = tetromino_peek_next();
    int min_y = peek.shape.blocks[0].y; int max_y = min_y;
    int min_x = peek.shape.blocks[0].x; int max_x = min_x;
    for (int i = 0; i < SHAPE_SIZE; ++i) {
        Pos p = peek.shape.blocks[i];
        min_y = min(min_y, p.y); max_y = max(max_y, p.y);
        min_x = min(min_x, p.x); max_x = max(max_x, p.x);
    }
    int rows = preview_size - 1;
    int cols = 2 * preview_size - 2;
    int pad_y = (rows - (max_y - min_y + 1)) / 2;
    int pad_x = (cols - 2 * (max_x - min_x + 1)) / 2;

    for (int i = 0; i < SHAPE_SIZE; ++i) {
        Pos p = peek.shape.blocks[i];
        int py = preview_y + 1 + pad_y + (p.y - min_y);
        int px = preview_x + 1 + pad_x + 2 * (p.x - min_x);
        tdraw_draw_at(py, px, "%s%s%s", color_code(peek.color), BLOCK_FILL, C_RESET);
    }
}

/* Draw game state */
static void draw_state(void)
{
    int state_y = y + 8; int state_x = x + BOARD_WIDTH + 4;
    tdraw_draw_at(state_y, state_x + 2, "Score:");
    tdraw_draw_at(state_y + 1, state_x + 2, "%06d", state_get_score());
    tdraw_draw_at(state_y + 3, state_x + 1, "Speed: %d", game_speed());
}

/* Draw keys legend */
static void draw_legend(void)
{
    int legend_y = y + 14; int legend_x = x + BOARD_WIDTH + 5;
    tdraw_draw_at(legend_y + 1, legend_x, "Q   Quit ");
    tdraw_draw_at(legend_y + 2, legend_x, "P   Pause");
    tdraw_draw_at(legend_y + 3, legend_x, "R   Rotate");
    tdraw_draw_at(legend_y + 4, legend_x, "+/- Speed");
    tdraw_draw_at(legend_y + 5, legend_x, "←/→ Move");
    tdraw_draw_at(legend_y + 6, legend_x, "↓   Down");
}

// === Public API =============================================================

void ui_draw(void)
{
    int h; int w; tdraw_term_size(&h, &w);
    y = DRAW_START_Y(h);
    x = DRAW_START_X(w);

    clear_screen();

    draw_frame();
    draw_state();
    draw_legend();
    draw_next_preview();
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
