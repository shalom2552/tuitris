#ifndef UI_H
#define UI_H

/* Main ui function */
void ui_draw_game(void);

/* Validate termianl size (blocking) */
void ui_validate(void);

/* Draw game title */
void ui_title(void);

/* Draw pause screen */
void ui_pause(void);

/* Draw game over screen */
void ui_game_over(void);

#endif // !UI_H
