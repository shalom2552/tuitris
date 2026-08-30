#ifndef GAME_H
#define GAME_H

/* Initialize the game */
void game_init(void);

/* Start the game */
void game_start(void);

/* Pause the game */
void game_pause(void);

/* Resume the game */
void game_resume(void);

/* Speed up the game */
void game_speed_up(void);

/* Slow down the game */
void game_speed_down(void);

/* Returns current speed */
int game_speed(void);

/* End the game */
void game_end(void);

#endif // !GAME_H

