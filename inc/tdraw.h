#ifndef TDRAW_H_1331063137
#define TDRAW_H_1331063137

#include <stdio.h>
#include <stdarg.h>
#include <signal.h>
#include <stdlib.h>
#include <threads.h>
#include <time.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <unistd.h>


// === Functions ==============================================================
/* Get current terminal height and width */
static inline void tdraw_term_size(int* h, int* w) {
    struct winsize ws;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    if (h) *h = ws.ws_row;
    if (w) *w = ws.ws_col;
}

/* Flush stdout and sleeps for a given number of milliseconds */
static inline void tdraw_delay(int ms) {
    fflush(stdout);
    struct timespec ts = {ms / 1000, ms % 1000 * 1000 * 1000};
    thrd_sleep(&ts, NULL);
}

// === Drawing ================================================================
/* Clears the screen */
static inline void tdraw_clear(void) {
    printf("\033[H\033[J");
}

/* Clears a given line */
static inline void tdraw_clear_line(int y) {
    printf("\033[s\033[%d;1H\033[2K\033[u", y);
}

/* Draws text at the specified position */
static inline void tdraw_draw_at(int y, int x, const char* format, ...) {
    char buf[256]; va_list ap;
    va_start(ap, format); vsnprintf(buf, sizeof(buf), format, ap); va_end(ap);
    printf("\033[s\033[%d;%dH%s\033[u", y, x, buf);
}

/* Draws a centered line at the specified position */
static inline void tdraw_draw_centered_line(int y, const char* format, ...) {
    char buf[256]; va_list ap;
    va_start(ap, format); vsnprintf(buf, sizeof(buf), format, ap); va_end(ap);
    int w; tdraw_term_size(NULL, &w);
    int len = 0; for (char* p = buf; *p; p++) if (*p == '\033') { while (*p && !isalpha(*p)) p++; } else len++;
    int x = (w - len) / 2 + 1;
    printf("\033[s\033[%d;1H\033[%d;%dH%s\033[u", y, y, x < 1 ? 1 : x, buf);
}

// === Boarder ================================================================
/* Draw a frame around two edges */
static inline void tdraw_draw_frame(int y1, int x1, int y2, int x2) {
    for (int i = y1; i <= y2; ++i) {
        tdraw_draw_at(i, x1, "│");
        tdraw_draw_at(i, x2, "│");
    }
    for (int i = x1; i <= x2; ++i) {
        tdraw_draw_at(y1, i, "─");
        tdraw_draw_at(y2, i, "─");
    }
    tdraw_draw_at(y1, x1, "┌");
    tdraw_draw_at(y1, x2, "┐");
    tdraw_draw_at(y2, x1, "└");
    tdraw_draw_at(y2, x2, "┘");
}

// === Utils ==================================================================

/* Returns 1 if the terminal size is sufficient for the requested dimensions, 0 otherwise */
static inline int tdraw_term_size_ok(int req_h, int req_w) {
    int h; int w;
    tdraw_term_size(&h, &w);
    if (h < req_h || w < req_w) {
        tdraw_clear();
        tdraw_draw_centered_line(h / 2 - 1, "Terminal too small.");
        tdraw_draw_centered_line(h / 2 - 0, "Width = \033[3%dm%d\033[0m Height = \033[3%dm%d\033[0m",
                                w < req_w ? 1 : 2, w, h < req_h ? 1 : 2, h);
        tdraw_draw_centered_line(h / 2 + 2, "Needed:");
        tdraw_draw_centered_line(h / 2 + 3, "Width: %d Height: %d", req_w, req_h);
        return 0;
    }
    return 1;
}

// === Init & handlers ========================================================
/* Reset all styles and modes */
static inline void tdraw_reset(void) {
    printf("\033[0m\033[?25h\033[?1049l");
    fflush(stdout);
}

static inline void _sig_handler(int sig) {
    (void)sig;
    tdraw_reset();
    exit(0);
}

/* Initialize the terminal drawing library */
static inline void tdraw_init(void) {
    signal(SIGINT, _sig_handler);
    signal(SIGTERM, _sig_handler);
    atexit(tdraw_reset);
    printf("\033[?1049h\033[?25l\033[H\033[J");
}

#endif // !TDRAW_H_1331063137

