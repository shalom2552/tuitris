#ifndef TDRAW_H_1331063137
#define TDRAW_H_1331063137

#include <stdio.h>
#include <stdarg.h>
#include <signal.h>
#include <stdlib.h>
#include <threads.h>
#include <time.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

/* Returns the width of the terminal */
static inline int tdraw_terminal_width(void)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
}

/* Returns the height of the terminal */
static inline int tdraw_terminal_height(void)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_row;
}

/* Sleeps for the specified number of milliseconds */
static inline void tdraw_delay(int ms)
{
    fflush(stdout);
    struct timespec ts = {ms / 1000, ms % 1000 * 1000 * 1000};
    thrd_sleep(&ts, NULL);
}

/* Saves the current cursor position */
static inline void tdraw_save_cursor(void)
{
    printf("\033[s");
}

/* Restores the cursor to its saved position */
static inline void tdraw_restor_cursor(void)
{
    printf("\033[u");
}

/* Clears the screen */
static inline void tdraw_clear(void)
{
    printf("\033[H\033[J");
}

/* Clears a given line */
static inline void tdraw_clear_line(int y)
{
    printf("\033[s\033[%d;1H\033[2K\033[u", y);
}

/* Draws text at the specified position */
static inline void tdraw_draw_at(int y, int x, const char* format, ...)
{
    char buf[256];
    va_list ap;
    va_start(ap, format); vsnprintf(buf, sizeof(buf), format, ap); va_end(ap);
    printf("\033[s\033[%d;%dH%s\033[u", y, x, buf);
}

/* Draws a centered line at the specified position */
static inline void tdraw_draw_centered_line(int y, const char* format, ...)
{
    char buf[256];
    va_list ap;
    va_start(ap, format); vsnprintf(buf, sizeof(buf), format, ap); va_end(ap);
    int x = (tdraw_terminal_width() - (int)strlen(buf)) / 2 + 1;
    printf("\033[s\033[%d;1H\033[2K\033[%d;%dH%s\033[u", y, y, x < 1 ? 1 : x, buf);
}

/* Returns 1 if the terminal size is sufficient for the requested dimensions, 0 otherwise */
static inline int tdraw_size_ok(int h, int w, int req_h, int req_w)
{
    if (h < req_h || w < req_w) {
        tdraw_clear();
        tdraw_draw_centered_line(h / 2 - 2, "Terminal too small.");
        tdraw_draw_centered_line(h / 2 - 1, "Width = %d Height = %d", w, h);
        tdraw_draw_centered_line(h / 2 + 1, "Needed:");
        tdraw_draw_centered_line(h / 2 + 2, "Width: %d Height: %d", req_w, req_h);
        return 0;
    }
    return 1;
}

/* Reset all styles and modes */
static inline void tdraw_reset(void)
{
    printf("\033[0m\033[?25h");
    fflush(stdout);
}

static inline void _sig_handler(int sig)
{
    (void)sig;
    tdraw_reset();
    exit(0);
}

/* Initialize the terminal drawing library */
static inline void tdraw_init(void)
{
    signal(SIGINT, _sig_handler);
    signal(SIGTERM, _sig_handler);
    atexit(tdraw_reset);
    printf("\033[?25l\033[H");
}

#endif // !TDRAW_H_1331063137

