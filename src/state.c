#include "state.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>

#define MAX_SCORE_FILE "data/max_score.txt"

// === Variables ==============================================================
typedef struct {
    int score;
    int lines;
    int level;
    int max_score;
} GameState;

GameState game_state;

// === Helper Functions =======================================================
/* Save max score to a file */
static void save_max_score(void)
{
    if (mkdir("data", 0755) != 0 && errno != EEXIST) return;
    FILE* f = fopen(MAX_SCORE_FILE, "wb");
    if (f == NULL) return;
    fprintf(f, "%d\n", game_state.max_score);
    fclose(f);
}

/* Load max score from a file */
static void load_max_score(void)
{
    FILE* f = fopen(MAX_SCORE_FILE, "rb");
    if (f != NULL) {
        fscanf(f, "%d", &game_state.max_score);
        fclose(f);
    }
}

// === Public API =============================================================
void state_init(void)
{
    game_state.score = 0;
    game_state.lines = 0;
    game_state.max_score = 0;
    load_max_score();
}

int state_score(void)
{
    return game_state.score;
}

int state_level(void)
{
    return game_state.lines / 10 + 1;
}

int state_lines(void)
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

int state_max_score(void)
{
    return game_state.max_score;
}

void state_save_max_score(void)
{
    if (game_state.score > game_state.max_score) {
        game_state.max_score = game_state.score;
    }
    save_max_score();
}

