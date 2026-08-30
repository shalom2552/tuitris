#ifndef STATS_H
#define STATS_H

#define SCORE_NEW_TETROMINO 100
#define SCORE_LINE_CLEARED 1000

/* Initialize the game state */
void state_init(void);

/* Add score to the game state */
void state_add_score(int score);

/* Get the current score */
int state_get_score(void);

/* Get max score from previous games */
int state_get_max_score(void);

/* Save the max score to the file */
void state_save_max_score(void);

#endif // !STATS_H

