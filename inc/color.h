#ifndef COLORS_H
#define COLORS_H

#define C_RESET "\033[0m"
#define C_BLACK "\033[30m"
#define C_RED "\033[31m"
#define C_GREEN "\033[32m"
#define C_YELLOW "\033[33m"
#define C_BLUE "\033[34m"
#define C_MAGENTA "\033[35m"
#define C_CYAN "\033[36m"
#define C_WHITE "\033[37m"

typedef enum {
    NONE = 0,
    BLACK,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,
    COLORS_COUNT
} Color;

static inline char* color_code(Color color) {
    static char* Colors[COLORS_COUNT] = {
        C_RESET,
        C_BLACK,
        C_RED,
        C_GREEN,
        C_YELLOW,
        C_BLUE,
        C_MAGENTA,
        C_CYAN,
        C_WHITE
    };
    return color < COLORS_COUNT ? Colors[color] : C_RESET;
}

#endif // !COLORS_H

