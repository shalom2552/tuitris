#include "help.h"

#include "color.h"
#include "state.h"
#include "tdraw.h"
#include "ui.h"

#define BOX_H 19
#define BOX_W 35

/* Draws the help box */
static void draw_help_box(int top, int left)
{
    tdraw_set_color(C_DIM C_BLUE);
    tdraw_draw_frame(top, left, top + BOX_H, left + BOX_W);
    tdraw_draw_at(top, left + 2, C_RESET C_BOLD C_CYAN " HELP " C_RESET);
    tdraw_draw_at(top + 2,  left + 3, C_RESET C_CYAN "GAMPLAY" C_RESET);
    tdraw_draw_at(top + 3, left + 5, C_RESET C_DIM "Clear 10 lines to level up");
    tdraw_draw_at(top + 4, left + 5, C_RESET C_DIM "Speed increases every level");
    tdraw_draw_at(top + 6,  left + 3, C_RESET C_CYAN "SCORE" C_RESET);
    tdraw_draw_at(top + 7, left + 5, C_RESET C_DIM "10 points for new piecs");
    tdraw_draw_at(top + 8, left + 5, C_RESET C_DIM "Points for clearing lines:");
    tdraw_draw_at(top + 9, left + 5, C_RESET C_DIM "%d * level * lines * lines", SCORE_LINE_CLEARED);
    tdraw_draw_at(top + 11,  left + 3, C_RESET C_CYAN "CONTROLS" C_RESET);
    tdraw_draw_at(top + 12,  left + 5, C_RESET C_MAGENTA "←/→      " C_RESET C_DIM " Move left / right");
    tdraw_draw_at(top + 13,  left + 5, C_RESET C_MAGENTA "↓        " C_RESET C_DIM " Move down");
    tdraw_draw_at(top + 14,  left + 5, C_RESET C_MAGENTA "R, Space " C_RESET C_DIM " Rotate right");
    tdraw_draw_at(top + 15,  left + 5, C_RESET C_MAGENTA "P        " C_RESET C_DIM " Pause / resume");
    tdraw_draw_at(top + 16,  left + 5, C_RESET C_MAGENTA "H        " C_RESET C_DIM " Show this help");
    tdraw_draw_at(top + 17,  left + 5, C_RESET C_MAGENTA "Q        " C_RESET C_DIM " Quit to menu");
    tdraw_draw_at(top + BOX_H, left + BOX_W / 2 - 8, C_DIM " press any key... ");
    tdraw_set_color(C_RESET);
}

void help_card(void)
{
    int h; int w; tdraw_term_size(&h, &w);
    draw_help_box(h / 2 - BOX_H / 2, w / 2 - BOX_W / 2);
    tdraw_flush();
    getchar();
    tdraw_clear();
}

void help_run(void)
{
    int h; int w; tdraw_term_size(&h, &w);
    tdraw_clear();
    ui_title();
    ui_subtitle();
    ui_credits();
    draw_help_box(h / 2 - BOX_H / 2, w / 2 - BOX_W / 2);
    tdraw_flush();
    getchar();
    tdraw_clear();
}

