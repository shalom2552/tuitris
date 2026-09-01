#include "game.h"

#include "state.h"
#include "board.h"
#include "tetromino.h"
#include "tdraw.h"
#include "input.h"
#include "help.h"
#include "ui.h"

#include <stdatomic.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX_DELAY 1000 // ms
#define MIN_DELAY 100 // ms

// === Variables ==============================================================
pthread_t tid;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
atomic_int g_delay = MAX_DELAY;
atomic_bool g_pause = false;
atomic_bool running = true;

// === Helper Functions =======================================================
/* Handles user input events */
static void* input_task(void* args)
{
    (void)args;
    while (running) {
        InputEvent event = get_user_input(); // blocking
        if (g_pause && event != INPUT_PAUSE && event != INPUT_QUIT) continue;
        pthread_mutex_lock(&mtx);
        switch (event) {
            case INPUT_QUIT: running = false; break;
            case INPUT_PAUSE: if (g_pause) game_resume(); else game_pause(); break;
            case INPUT_LEFT: tetromino_move_left(); break;
            case INPUT_RIGHT: tetromino_move_right(); break;
            case INPUT_DOWN: tetromino_move_down(); break;
            case INPUT_HELP: help_card(); break;
            case INPUT_ROTATE_CW: tetromino_rotate_right(); break;
            default: break;
        }
        if (!g_pause) ui_draw_game();
        pthread_mutex_unlock(&mtx);
    }
    return NULL;
}

/* Spawns next tetromino and update score */
static void next_tetromino(void)
{
    int cleard = board_clear_lines();
    state_add_lines(cleard);
    if (!tetromino_create()) {
        running = false;
        pthread_cancel(tid);
    } else {
        state_add_score(SCORE_NEW_TETROMINO);
    }
}

static int delay(void)
{
    g_delay = MAX_DELAY - 50 * (state_level() - 1);
    g_delay = g_delay > MIN_DELAY ? g_delay : MIN_DELAY;
    return g_delay;
}

// === Public API =============================================================
void game_init(void)
{
    srand(time(NULL));
    board_init();
    state_init();
    tetromino_init();
    tdraw_clear();
    running = true;
    g_pause = false;
    g_delay = MAX_DELAY;
}

void game_start(void)
{
    if (pthread_create(&tid, NULL, input_task, NULL) != 0) {
        return;
    }

    ui_draw_game();
    tetromino_create();
    while (running) {
        ui_validate();
        pthread_mutex_lock(&mtx);
        if (!g_pause) {
            if (tetromino_locked()) {
                next_tetromino();
            } else {
                tetromino_move_down();
            }
            ui_draw_game();
        }
        pthread_mutex_unlock(&mtx);
        tdraw_delay(delay());
    }
    pthread_join(tid, NULL);
    game_end();
}

void game_pause(void)
{
    g_pause = true;
    ui_pause();
}

void game_resume(void)
{
    g_pause = false;
    tdraw_clear();
}

int game_speed(void)
{
    return 1 + (MAX_DELAY / 100) - g_delay / 100;
}

void game_end(void)
{
    state_save_max_score();
    ui_game_over();
}

