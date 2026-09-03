#include "state.h"

#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>

#define MAX_SCORE_FILE "data/high_score"

// === Variables ==============================================================
typedef struct {
    unsigned int score;
    unsigned int lines;
    unsigned int high_score;
} GameState;

GameState game_state;

static int s_max_score_loaded = 0;

// === Helper Functions =======================================================
/* Save max score to a file */
static void save_high_score(void)
{
    if (mkdir("data", 0755) != 0 && errno != EEXIST) return;
    FILE* f = fopen(MAX_SCORE_FILE, "wb");
    if (f == NULL) return;
    fprintf(f, "%u\n", game_state.high_score);
    fclose(f);
}

/* Load max score from a file */
static void load_high_score(void)
{
    FILE* f = fopen(MAX_SCORE_FILE, "rb");
    if (f != NULL) {
        fscanf(f, "%u", &game_state.high_score);
        fclose(f);
    }
}

// === Public API =============================================================
void state_init(void)
{
    game_state.score = 0;
    game_state.lines = 0;
    game_state.high_score = 0;
    load_high_score();
}

unsigned int state_score(void)
{
    return game_state.score;
}

unsigned int state_level(void)
{
    return game_state.lines / 10 + 1;
}

unsigned int state_lines(void)
{
    return game_state.lines;
}

void state_add_score(int score)
{
    game_state.score += score;
}

void state_add_lines(int lines)
{
    game_state.score += lines * lines * SCORE_LINE_CLEARED * state_level();
    game_state.lines += lines;
}

unsigned int state_high_score(void)
{
    if (!s_max_score_loaded) {
        load_high_score();
        s_max_score_loaded = 1;
    }
    return game_state.high_score;
}

void state_save_high_score(void)
{
    if (game_state.score > game_state.high_score) {
        game_state.high_score = game_state.score;
    }
    save_high_score();
}

