#ifndef UI_H
#define UI_H

/* Debug output. stdout is taken by the game ui, so messages are written to a
 * separate descriptor instead. The target is picked once, on first use:
 *   - $TUITRIS_DEBUG=<path>: append to that file
 *   - stderr when it is not the terminal the game draws on (e.g. `2>debug.log`)
 *   - otherwise the file below, so the ui is never corrupted
 * Compiling with -DNDEBUG compiles DEBUG() away completely. */
#define DEBUG_LOG_FILE "tuitris-debug.log"

void ui_debug(const char* file, int line, const char* fmt, ...)
    __attribute__((format(printf, 3, 4)));

#ifdef NDEBUG
#define DEBUG(...) do { } while (0)
#else
#define DEBUG(...) ui_debug(__FILE__, __LINE__, __VA_ARGS__)
#endif // NDEBUG

/* Validate termianl size (blocking) */
void ui_validate(void);

/* Draw menu screen */
void ui_draw_menu(int selection);

/* Main ui function */
void ui_draw_game(void);

/* highlight a full line to be clear */
void ui_highlight_full_line(int row);

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
