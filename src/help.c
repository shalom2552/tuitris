#include "help.h"

#include "color.h"
#include "tdraw.h"
#include "ui.h"

#define BOX_H 13
#define BOX_W 37

/* Draws the help box */
static void draw_help_box(int top, int left)
{
    tdraw_set_color(C_DIM C_BLUE);
    tdraw_draw_frame(top, left, top + BOX_H, left + BOX_W);
    tdraw_set_color(C_RESET);
    tdraw_draw_at(top, left + 2, C_BOLD C_CYAN " HELP " C_RESET);
    tdraw_draw_at(top + 2,  left + 3, C_CYAN "CONTROLS" C_RESET);
    tdraw_draw_at(top + 3,  left + 5, C_MAGENTA "←/→      " C_RESET C_DIM " Move left / right" C_RESET);
    tdraw_draw_at(top + 4,  left + 5, C_MAGENTA "↓        " C_RESET C_DIM " Move down" C_RESET);
    tdraw_draw_at(top + 5,  left + 5, C_MAGENTA "R, Space " C_RESET C_DIM " Rotate right" C_RESET);
    tdraw_draw_at(top + 6,  left + 5, C_MAGENTA "P        " C_RESET C_DIM " Pause / resume" C_RESET);
    tdraw_draw_at(top + 7,  left + 5, C_MAGENTA "H        " C_RESET C_DIM " Show this help" C_RESET);
    tdraw_draw_at(top + 8,  left + 5, C_MAGENTA "Q        " C_RESET C_DIM " Quit to menu" C_RESET);
    tdraw_draw_at(top + 10, left + 5, C_DIM "Clear 10 lines to level up" C_RESET);
    tdraw_draw_at(top + 11, left + 5, C_DIM "Speed increases every level" C_RESET);
    tdraw_set_color(C_DIM);
    tdraw_draw_at(top + BOX_H, left + BOX_W / 2 - 8, " press any key... ");
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
    draw_help_box(h / 2 - 2, w / 2 - BOX_W / 2);
    ui_credits();
    tdraw_flush();
    getchar();
    tdraw_clear();
}

