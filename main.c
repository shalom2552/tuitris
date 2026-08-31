#include "game.h"
#include "help.h"
#include "menu.h"
#include "input.h"
#include "tdraw.h"

int main(void)
{
    input_init();
    tdraw_init();

    while (1) {
        MenuOption opt = menu_run();
        switch (opt) {
            case OPTION_START:
                game_init();
                game_start();
                continue;
            case OPTION_HELP:
                help_run();
                continue;
            case OPTION_EXIT:
                return 0;
            default:
                continue;
        }
    }
    return 0;
}
