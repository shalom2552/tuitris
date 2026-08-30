#ifndef STATS_H
#define STATS_H

/* Initialize the game state */
void state_init(void);

/* Add score to the game state */
void state_add_score(int score);

/* Get the current score */
int state_get_score(void);

#endif // !STATS_H

