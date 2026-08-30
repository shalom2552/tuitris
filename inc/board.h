#ifndef BOARD_H
#define BOARD_H

#include "color.h"

#define BOARD_HIGHT 20
#define BOARD_WIDTH 10

/* Initialize an empty board */
void board_init(void);

/* Set a cell color at (x, y) position */
void board_set(int y, int x, Color color);

/* Remove a cell at (x, y) position */
void board_remove(int y, int x);

/* Return 1 if (x, y) is free, 0 otherwise */
int board_is_free(int y, int x);

/* Return the color of the cell at (x, y) position */
Color board_get_color(int y, int x);

/* Clear all lines that are full */
void board_clear_lines(void);

#endif // !BOARD_H

