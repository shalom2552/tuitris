#include "game.h"

#include "board.h"
#include "tetromino.h"
#include "tdraw.h"
#include "input.h"
#include "ui.h"

#include <stdatomic.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// === Variables ==============================================================

pthread_mutex_t mtx;
atomic_int g_delay = 1000; // ms
bool g_pause = false;

// === Helper Functions =======================================================

/* Handles user input events */
static void* input_task(void* args)
{
    (void)args;
    while (1) {
        InputEvent event = get_user_input(); // blocking
        pthread_mutex_lock(&mtx);
        if (g_pause && event != INPUT_PAUSE && event != INPUT_QUIT) continue;
        switch (event) {
            case INPUT_QUIT: game_end(); break;
            case INPUT_PAUSE: g_pause = !g_pause; break;
            case INPUT_LEFT: tetromino_move_left(); break;
            case INPUT_RIGHT: tetromino_move_right(); break;
            case INPUT_DOWN: tetromino_move_down(); break;
            case INPUT_ROTATE_CW: tetromino_rotate_right(); break;
            case INPUT_PLUS: game_speed_up(); break;
            case INPUT_MINUS: game_speed_down(); break;
            default: break;
        }
        ui_draw();
        pthread_mutex_unlock(&mtx);
    }
    return NULL;
}

// === Public API =============================================================

void game_init(void)
{
    input_init();
    tdraw_init();
    board_init();
    srand(time(NULL));
    pthread_mutex_init(&mtx, NULL);
}

void game_start(void)
{
    pthread_t tid;
    if (pthread_create(&tid, NULL, input_task, NULL) != 0) {
        return;
    }

    ui_draw();
    tetromino_create();
    while (1) {
        ui_validate();
        pthread_mutex_lock(&mtx);
        if (!g_pause) {
            if (tetromino_locked()) {
                board_clear_lines();
                if (!tetromino_create()) {
                    game_end(); break;
                }
            } else {
                tetromino_move_down();
            }
        }
        ui_draw();
        pthread_mutex_unlock(&mtx);
        tdraw_delay(g_delay);
    }
    pthread_join(tid, NULL);
}

void game_pause(void)
{
    g_pause = true;
}

void game_resume(void)
{
    g_pause = false;
}

void game_speed_up(void)
{
    g_delay = g_delay > 100 ? g_delay - 100 : g_delay;
}

void game_speed_down(void)
{
    g_delay = g_delay < 1000 ? g_delay + 100 : g_delay;
}

void game_end(void)
{
    exit(0);
}

