#include "help.h"

#include "tdraw.h"
#include "ui.h"

#define BOX_H 12
#define BOX_W 36

/* Draws the help box */
static void draw_help_box(int top, int left)
{
    tdraw_draw_frame(top, left, top + BOX_H, left + BOX_W);
    tdraw_draw_at(top, left + 2, " HELP ");
    tdraw_draw_at(top + 2,  left + 3, "CONTROLS");
    tdraw_draw_at(top + 3,  left + 5, "%s", "←/→       Move left / right");
    tdraw_draw_at(top + 4,  left + 5, "%s", "↓         Move down");
    tdraw_draw_at(top + 5,  left + 5, "R, Space  Rotate right");
    tdraw_draw_at(top + 6,  left + 5, "P         Pause / resume");
    tdraw_draw_at(top + 7,  left + 5, "Q         Quit to menu");
    tdraw_draw_at(top + 10, left + 5, "clear 10 lines to level up");
    tdraw_draw_at(top + BOX_H, left + BOX_W / 2 - 8, " press any key... ");
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
    tdraw_flush();
    getchar();
    tdraw_clear();
}

