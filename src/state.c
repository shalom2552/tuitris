#include "state.h"

typedef struct {
    int score;
} GameState;

GameState gameState;

void state_init(void)
{
    gameState.score = 0;
}

void state_add_score(int score)
{
    gameState.score += score;
}

int state_get_score(void)
{
    return gameState.score;
}
