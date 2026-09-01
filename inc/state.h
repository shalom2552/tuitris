#ifndef STATS_H
#define STATS_H

#define SCORE_NEW_TETROMINO 10
#define SCORE_LINE_CLEARED 1000

/* Initialize the game state */
void state_init(void);

/* Get the current score */
int state_score(void);

/* Get the current level */
int state_level(void);

/* Get the total lines cleared */
int state_lines(void);

/* Add score to the game state */
void state_add_score(int score);

/* Add lines and update the score */
void state_add_lines(int lines);

/* Get max score from previous games */
int state_high_score(void);

/* Save the max score to the file */
void state_save_high_score(void);

#endif // !STATS_H

