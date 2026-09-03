#ifndef UI_H
#define UI_H

/* Validate termianl size (blocking) */
void ui_validate(void);

/* Draw menu screen */
void ui_draw_menu(int selection);

/* Main ui function */
void ui_draw_game(void);

/* Draw game title */
void ui_title(void);

/* Draw subtitle line */
void ui_subtitle(void);

/* Draw credits line */
void ui_credits(void);

/* Draw pause screen */
void ui_pause(void);

/* Draw game over screen */
void ui_game_over(void);

/* Draw a confirmation dialog */
int ui_confirm(const char* msg);

/* Draw a floating message box */
void ui_message(const char* msg);

#endif // !UI_H
