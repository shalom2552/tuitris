#include "menu.h"

#include "color.h"
#include "input.h"
#include "tdraw.h"
#include "ui.h"

static int selection = 0;

char* menu_options[OPTION_COUNT] = {
    "START",
    "HELP",
    "EXIT"
};

/* Draw the menu */
static void menu_draw(void)
{
    tdraw_clear();
    ui_title();
    int h; tdraw_term_size(&h, NULL);
    for (int i = 0; i < OPTION_COUNT; ++i) {
        if (i == selection) {
            tdraw_draw_at(1, 1, C_CYAN);
        }
        tdraw_draw_centered_line(h / 2 + 2 * i, menu_options[i]);
        tdraw_draw_at(1, 1, C_RESET);
    }
    tdraw_flush();
}

MenuOption menu_run(void)
{
    while (1) {
        menu_draw();
        InputEvent event = get_user_input();
        switch (event) {
            case INPUT_SELECT:
                return selection; break;
            case INPUT_DOWN:
                selection = selection < OPTION_COUNT - 1 ? selection + 1 : 0;
                break;
            case INPUT_UP:
                selection = selection > 0 ? selection - 1 : OPTION_COUNT - 1;
                break;
            case INPUT_QUIT:
                return OPTION_EXIT; break;
            default:
                break;
        }
    }
}
