#include "game.h"

#include "board.h"
#include "tetromino.h"
#include "tdraw.h"
#include "input.h"

#include <stdatomic.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

pthread_mutex_t mtx;

static void next(void)
{
    tetromino_move_down();
}

static void* input_task(void* args)
{
    (void)args;
    while (1) {
        InputEvent event = get_user_input();
        pthread_mutex_lock(&mtx);
        switch (event) {
            case INPUT_QUIT:
                exit(0);
            case INPUT_LEFT:
                tetromino_move_left();
                break;
            case INPUT_RIGHT:
                tetromino_move_right();
                break;
            case INPUT_DOWN:
                next();
                break;
            case INPUT_ROTATE_CW:
                tetromino_rotate_left();
                break;
            default:
                break;
        }
        board_draw();
        pthread_mutex_unlock(&mtx);
    }
    return NULL;
}

void game_init(void)
{
    input_init();
    tdraw_init();
    board_init();

    srand(time(NULL));
}

void game_start(void)
{
    pthread_mutex_init(&mtx, NULL);
    pthread_t tid;
    if (pthread_create(&tid, NULL, input_task, NULL) != 0) {
        exit(1);
    }

    board_draw();
    tetromino_create();
    while (1) {
        while (!tdraw_term_size_ok(REQUIRE_HEIGHT, REQUIRE_WIDTH)) {
            tdraw_delay(50);
        }
        pthread_mutex_lock(&mtx);
        if (tetromino_locked()) {
            // board_clear_lines(); // TODO: implement
            if (tetromino_create()) {
                exit(0);
            }
        } else {
            tetromino_move_down();
        }
        board_draw();
        pthread_mutex_unlock(&mtx);
        tdraw_delay(250);
    }
}

