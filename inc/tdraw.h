#ifndef TDRAW_H_1331063137
#define TDRAW_H_1331063137

/* Get current terminal height and width */
void tdraw_term_size(int* h, int* w);
/* Flush the output buffer */
void tdraw_flush(void);
/* Flush stdout and sleeps for a given number of milliseconds */
void tdraw_delay(int ms);
/* Clears the screen */
void tdraw_clear(void);
/* Clears a given line */
void tdraw_clear_line(int y);
/* Draw an a color/style escape sequence */
void tdraw_set_color(const char* c);
/* Draws text at the specified position */
void tdraw_draw_at(int y, int x, const char* format, ...);
/* Draws a centered line at the specified position */
void tdraw_draw_centered_line(int y, const char* format, ...);
/* Draw a frame around two edges */
void tdraw_draw_frame(int y1, int x1, int y2, int x2);
/* Returns 1 if the terminal size is sufficient for the requested dimensions, 0 otherwise */
int tdraw_term_size_ok(int req_h, int req_w);
/* Reset all styles and modes */
void tdraw_reset(void);
/* Initialize the terminal drawing library */
void tdraw_init(void);

// #define TDRAW_IMPL
#ifdef TDRAW_IMPL

#include <ctype.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <threads.h>
#include <time.h>
#include <unistd.h>

// === Functions ==============================================================

void tdraw_term_size(int* h, int* w) {
    struct winsize ws;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    if (h) *h = ws.ws_row;
    if (w) *w = ws.ws_col;
}

void tdraw_flush(void) {
    printf("\033[?2026l"); // unlock output
    fflush(stdout);
    printf("\033[?2026h"); // relock output
}

void tdraw_delay(int ms) {
    tdraw_flush();
    struct timespec ts = {ms / 1000, ms % 1000 * 1000 * 1000};
    thrd_sleep(&ts, NULL);
}

// === Drawing ================================================================

void tdraw_clear(void) {
    printf("\033[H\033[J");
}

void tdraw_clear_line(int y) {
    printf("\033[%d;1H\033[2K", y);
}

void tdraw_set_color(const char* c) {
    printf("%s", c);
}

void tdraw_draw_at(int y, int x, const char* format, ...) {
    char buf[256]; va_list ap;
    va_start(ap, format); vsnprintf(buf, sizeof(buf), format, ap); va_end(ap);
    printf("\033[%d;%dH%s", y, x, buf);
}

void tdraw_draw_centered_line(int y, const char* format, ...) {
    char buf[256]; va_list ap;
    va_start(ap, format); vsnprintf(buf, sizeof(buf), format, ap); va_end(ap);
    int w; tdraw_term_size(NULL, &w);
    int len = 0; for (char* p = buf; *p; p++) if (*p == '\033') { while (*p && !isalpha(*p)) p++; } else len += (*p & 0xC0) != 0x80;
    int x = (w - len) / 2 + 1;
    printf("\033[%d;%dH%s", y, x < 1 ? 1 : x, buf);
}

// === Boarder ================================================================

void tdraw_draw_frame(int y1, int x1, int y2, int x2) {
    for (int i = y1; i <= y2; ++i) {
        for (int j = x1 + 1; j < x2; ++j) {
            tdraw_draw_at(i, j, " ");
        }
        tdraw_draw_at(i, x1, "│");
        tdraw_draw_at(i, x2, "│");
    }
    for (int i = x1; i <= x2; ++i) {
        tdraw_draw_at(y1, i, "─");
        tdraw_draw_at(y2, i, "─");
    }
    tdraw_draw_at(y1, x1, "╭");
    tdraw_draw_at(y1, x2, "╮");
    tdraw_draw_at(y2, x1, "╰");
    tdraw_draw_at(y2, x2, "╯");
}

// === Utils ==================================================================

int tdraw_term_size_ok(int req_h, int req_w) {
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

static char _tdraw_buf[65536];
static int _tdraw_active = 0;

void tdraw_reset(void) {
    if (_tdraw_active) {
        printf("\033[?2026l\033[?25h\033[0m\033[?1049l");
        _tdraw_active = 0;
        fflush(stdout);
    }
}

#ifdef TDRAW_SIG_HANDLER
static void _tdraw_sig_handler(int sig) {
    (void)sig;
    tdraw_reset();
    exit(0);
}
#endif // TDRAW_SIG_HANDLER

void tdraw_init(void) {
#ifdef TDRAW_SIG_HANDLER
    signal(SIGINT, _tdraw_sig_handler);
    signal(SIGTERM, _tdraw_sig_handler);
#endif // TDRAW_SIG_HANDLER
    atexit(tdraw_reset);
    _tdraw_active = 1;
    setvbuf(stdout, _tdraw_buf, _IOFBF, sizeof(_tdraw_buf));
    printf("\033[?1049h\033[?25l\033[H\033[J\033[?2026h");
    fflush(stdout);
}

#endif // TDRAW_IMPL

#endif // !TDRAW_H_1331063137

