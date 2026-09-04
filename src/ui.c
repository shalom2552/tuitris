#include "ui.h"

#include "color.h"
#include "tdraw.h"
#include "board.h"
#include "state.h"
#include "input.h"
#include "menu.h"
#include "tetromino.h"
#include "ui_assets.h"

#include <unistd.h>

// === Defines ================================================================
#define PANEL_WIDTH 14
#define BOARD_HIGHT (BOARD_ROWS + 2)
#define BOARD_WIDTH (BOARD_COLS * 2 + 2)

#define FRAME_HIGHT (BOARD_HIGHT + 2)
#define FRAME_WIDTH (BOARD_WIDTH + PANEL_WIDTH + 4)

#define DRAW_START_Y(h) (((h) - FRAME_HIGHT) / 2 + 1)
#define DRAW_START_X(w) (((w) - FRAME_WIDTH) / 2 + 1)

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
    tdraw_draw_at(y + 1, x + 1, C_DIM C_CYAN BAR_FILL C_RESET);
    for (int i = 0; i < BOARD_ROWS; ++i) {
        tdraw_draw_at(y + 2 + i, x + 1, C_DIM C_CYAN BLOCK_FILL C_RESET);
        tdraw_draw_at(y + 2 + i, x + 3 + BOARD_COLS * 2, C_DIM C_CYAN BLOCK_FILL C_RESET);
    }
    tdraw_draw_at(y + 2 + BOARD_ROWS, x + 1, C_DIM C_CYAN BAR_FILL C_RESET);
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
    int panel_start = x + BOARD_WIDTH + 3;
    tdraw_set_color(C_DIM C_BLUE);
    tdraw_draw_frame(y, x, y + BOARD_HIGHT + 1, panel_start);
    tdraw_draw_frame(y, panel_start, y + BOARD_HIGHT + 1, panel_start + PANEL_WIDTH + 1);
    tdraw_draw_at(y, panel_start, "┬");
    tdraw_draw_at(y + BOARD_HIGHT + 1, panel_start, "┴");
    tdraw_set_color(C_RESET);
    tdraw_draw_at(y, x + 8 , C_DIM C_BLUE "<" C_RESET C_BOLD C_CYAN " TUITRIS " C_RESET C_DIM C_BLUE ">" C_RESET);
}

/* Draw board and panel frames */
static void draw_next_preview(void)
{
    int preview_size = PANEL_WIDTH / 2 - 2;
    int preview_y = y + 1; int preview_x = x + BOARD_WIDTH + 6;
    tdraw_set_color(C_DIM C_BLUE);
    tdraw_draw_frame(preview_y, preview_x - 1, preview_y + preview_size, preview_x + 2 * preview_size);
    tdraw_set_color(C_RESET);
    tdraw_draw_at(preview_y, preview_x + 1, C_CYAN "Next" C_RESET);
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
    int state_y = y + 7; int state_x = x + BOARD_WIDTH + 5;
    tdraw_set_color(C_DIM C_BLUE);
    tdraw_draw_frame(state_y, state_x + 0, state_y + 5, state_x + 11);
    tdraw_draw_frame(state_y + 6, state_x + 0, state_y + 8, state_x + 11);
    tdraw_set_color(C_RESET);
    tdraw_draw_at(state_y + 0, state_x + 1, C_CYAN "Score" C_RESET);
    tdraw_draw_at(state_y + 1, state_x + 2, C_BOLD "%08d" C_RESET, state_score());
    tdraw_draw_at(state_y + 2, state_x + 2, C_DIM "Lvl.  " C_RESET C_BOLD "%02d" C_RESET, state_level());
    tdraw_draw_at(state_y + 3, state_x + 3, C_CYAN "Lines" C_RESET);
    tdraw_draw_at(state_y + 4, state_x + 3, C_BOLD "%05d" C_RESET, state_lines());
    tdraw_draw_at(state_y + 6, state_x + 1, C_CYAN "High-Score" C_RESET);
    tdraw_draw_at(state_y + 6, state_x + 1, C_CYAN "High-Score" C_RESET);
    tdraw_draw_at(state_y + 7, state_x + 2, C_BOLD C_MAGENTA "%08d" C_RESET, state_high_score());
}

/* Draw keys legend */
static void draw_legend(void)
{
    int legend_y = y + 16; int legend_x = x + BOARD_WIDTH + 6;
    tdraw_set_color(C_DIM C_BLUE);
    tdraw_draw_frame(legend_y, legend_x - 1, legend_y + 6, legend_x + 10);
    tdraw_set_color(C_RESET);
    tdraw_draw_at(legend_y, legend_x, C_CYAN "Keys" C_RESET);
    tdraw_draw_at(legend_y + 1, legend_x, C_MAGENTA"←/→ "C_RESET C_DIM "Move"C_RESET);
    tdraw_draw_at(legend_y + 2, legend_x, C_MAGENTA"↓   "C_RESET C_DIM "Down"C_RESET);
    tdraw_draw_at(legend_y + 3, legend_x, C_MAGENTA"R   "C_RESET C_DIM "Rotat"C_RESET);
    tdraw_draw_at(legend_y + 4, legend_x, C_MAGENTA"P   "C_RESET C_DIM "Pause"C_RESET);
    tdraw_draw_at(legend_y + 5, legend_x, C_MAGENTA"Q   "C_RESET C_DIM "Quit "C_RESET);
}

/* Draws a block of equal-width lines, centered horizontally */
static void draw_block(int top, int width, const char** lines, int count)
{
    int w; tdraw_term_size(NULL, &w);
    int left = w / 2 - width / 2;
    if (left < 1) left = 1;
    if (top < 1) top = 1;
    for (int i = 0; i < count; ++i) {
        tdraw_draw_at(top + i, left, "%s", lines[i]);
    }
}

// === Public API =============================================================
void ui_validate(void)
{
    int require_y = BOARD_HIGHT + 4;
    int require_x = BOARD_WIDTH + PANEL_WIDTH + 8;
    while (!tdraw_term_size_ok(require_y, require_x)) {
        tdraw_delay(10);
    }
}

/* Draw the menu */
void ui_draw_menu(int selection)
{
    ui_validate();
    tdraw_clear();
    ui_title();
    ui_subtitle();
    int h; int w; tdraw_term_size(&h, &w);
    int top = h / 2 + 1;
    for (int i = 0; i < OPTION_COUNT; ++i) {
        if (i == selection) {
            tdraw_draw_at(1, 1, C_BOLD C_CYAN);
            tdraw_draw_at(top + 2 * i, w / 2 - 4, "→ %s",  menu_options[i]);
        } else {
            tdraw_draw_at(top + 2 * i, w / 2 - 2, C_DIM "%s", menu_options[i]);
        }
        tdraw_draw_at(1, 1, C_RESET);
    }
    ui_credits();
    tdraw_flush();
}

void ui_draw_game(void)
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

void ui_title(void)
{
    int h; int w; tdraw_term_size(&h, &w);

    const char** title; int count; int width;
    if (w >= 74 && h >= 20) {
        title = TITLE1; count = (int)(sizeof TITLE1 / sizeof *TITLE1); width = 70;
    } else if (w >= 39 && h >= 20) {
        title = TITLE2; count = (int)(sizeof TITLE2 / sizeof *TITLE2); width = 35;
    } else {
        title = TITLE3; count = (int)(sizeof TITLE3 / sizeof *TITLE3); width = 20;
    }

    draw_block(h / 2 - 2 - count, width, title, count);
    tdraw_flush();
}

void ui_subtitle(void)
{
    int h; tdraw_term_size(&h, NULL);
    tdraw_set_color(C_DIM);
    tdraw_draw_centered_line(h - 4, "High Score: %u", state_high_score());
    tdraw_set_color(C_RESET);
    tdraw_flush();
}

void ui_credits(void)
{
    int h; tdraw_term_size(&h, NULL);
    tdraw_draw_centered_line(h - 2, C_DIM DEV_CREDIT C_RESET);
}

void ui_pause(void)
{
    int h; int w; tdraw_term_size(&h, &w);
    tdraw_set_color(C_DIM C_BLUE);
    tdraw_draw_frame(h / 2 - 3, w / 2 - 12, h / 2 + 3, w / 2 + 12);
    tdraw_set_color(C_BOLD C_CYAN);
    draw_block(h / 2 - 2, 18, ASCII_PAUSED, (int)(sizeof ASCII_PAUSED / sizeof *ASCII_PAUSED));
    tdraw_set_color(C_RESET);
    tdraw_draw_at(h / 2 + 2, w / 2 - 9, C_DIM"Press p to continue"C_RESET);
    tdraw_flush();
}

void ui_game_over(void)
{
    tdraw_clear();
    int h; int w; tdraw_term_size(&h, &w);
    if (state_score() > state_high_score()) { // High Score
        tdraw_set_color(C_BOLD C_GREEN);
        draw_block(h / 2 - 6, 28, ASCII_HIGH, (int)(sizeof ASCII_HIGH / sizeof *ASCII_HIGH));
        draw_block(h / 2 + 1, 41, ASCII_SCORE, (int)(sizeof ASCII_SCORE / sizeof *ASCII_SCORE));
    } else { // Game Over
        tdraw_set_color(C_BOLD C_RED);
        draw_block(h / 2 - 6, 36, ASCII_GAME, (int)(sizeof ASCII_GAME / sizeof *ASCII_GAME));
        draw_block(h / 2 + 1, 34, ASCII_OVER, (int)(sizeof ASCII_OVER / sizeof *ASCII_OVER));
    }
    tdraw_set_color(C_RESET);

    tdraw_draw_at(h / 2 + 8, w / 2 - 6, C_CYAN "Score: " C_RESET C_BOLD "%d"C_RESET, state_score());
    tdraw_draw_at(h / 2 + 9, w / 2 - 11, C_CYAN "High-Score: " C_BOLD C_MAGENTA "%d"C_RESET, state_high_score());
    tdraw_draw_centered_line(h / 2 + 11, C_DIM"press ENTER to continue..."C_RESET);
    tdraw_flush();
    InputEvent event = get_user_input(); // continue on ENTER or QUIT only
    while (event != INPUT_SELECT && event != INPUT_QUIT) { event = get_user_input(); }
}

int ui_confirm(const char* msg) {
    int h; int w; tdraw_term_size(&h, &w);
    tdraw_set_color(C_BOLD C_BLUE);
    tdraw_draw_frame(h / 2 - 3, w / 2 - 20, h / 2 + 3, w / 2 + 21);
    tdraw_draw_centered_line(h / 2 - 1, msg);
    tdraw_set_color(C_RESET C_DIM);
    tdraw_draw_centered_line(h / 2 + 1, "[N/y]");
    tdraw_set_color(C_RESET);
    tdraw_flush();
    char c = getchar();
    return c == 'y';
}

void ui_message(const char* msg)
{
    int h; int w; tdraw_term_size(&h, &w);
    tdraw_set_color(C_BOLD C_GREEN);
    tdraw_draw_frame(h / 2 - 3, w / 2 - 20, h / 2 + 3, w / 2 + 21);
    tdraw_draw_centered_line(h / 2 - 1, msg);
    tdraw_set_color(C_RESET C_DIM);
    tdraw_draw_centered_line(h / 2 + 1, "Press any key");
    tdraw_set_color(C_RESET);
    tdraw_flush();
    getchar();
}

