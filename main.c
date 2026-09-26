#define TDRAW_IMPL
#include "tdraw.h"
#include "game.h"
#include "help.h"
#include "menu.h"
#include "input.h"
#include "settings.h"

#include <signal.h>

static void sig_handler(int sig)
{
    (void)sig;
    tdraw_reset();
    input_reset();
    exit(0);
}

int main(void)
{
    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);
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

            case OPTION_RESET:
                settings_run();
                continue;

            case OPTION_EXIT:
                return 0;

            default:
                continue;
        }

    }

    return 0;
}

