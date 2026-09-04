#ifndef STATS_H
#define STATS_H

#define SCORE_NEW_TETROMINO 10
#define SCORE_LINE_CLEARED  100

/* Initialize the game state */
void state_init(void);

/* Get the current score */
unsigned int state_score(void);

/* Get the current level */
unsigned int state_level(void);

/* Get the total lines cleared */
unsigned int state_lines(void);

/* Add score to the game state */
void state_add_score(int score);

/* Add lines and update the score */
void state_add_lines(int lines);

/* Get max score from previous games */
unsigned int state_high_score(void);

/* Save the high score to the file */
void state_save_high_score(void);

/* Reset the high score vlaue */
void state_reset_high_score(void);

#endif // !STATS_H

