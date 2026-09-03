#include "menu.h"

#include "input.h"
#include "ui.h"

static int selection = 0;

char* menu_options[OPTION_COUNT] = {
    "START",
    "HELP",
    "RESET",
    "EXIT"
};

MenuOption menu_run(void)
{
    while (1) {

        ui_draw_menu(selection);

        InputEvent event = get_user_input();

        switch (event) {

            case INPUT_SELECT:
                return selection;
                break;

            case INPUT_DOWN:
                selection = selection < OPTION_COUNT - 1 ? selection + 1 : 0;
                break;

            case INPUT_UP:
                selection = selection > 0 ? selection - 1 : OPTION_COUNT - 1;
                break;

            case INPUT_QUIT:
                return OPTION_EXIT;
                break;

            default:
                break;
        }
    }
}

