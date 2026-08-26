#include "game.h"

#include "board.h"
#include "tetromino.h"
#include "tdraw.h"

#include <stdlib.h>
#include <time.h>

void game_init(void)
{
    tdraw_init();
    board_init();
    srand(time(NULL));
}

void game_start(void)
{
    tetromino_create();
    while (1) {
        tetromino_move_down();
        board_draw();
        tdraw_delay(500);
    }
}

