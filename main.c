#include "board.h"

#include "color.h"
#include "inc/tdraw.h"

int main(void)
{
    tdraw_init();

    board_init();

    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10; j++) {
            board_set(i, j, WHITE);
        }
    }
    board_set(2, 3, RED);
    board_set(3, 2, GREEN);
    board_set(4, 2, BLUE);
    board_set(5, 2, YELLOW);
    board_set(2, 5, RED);
    board_set(3, 3, GREEN);
    board_set(4, 3, BLUE);
    board_set(5, 3, YELLOW);
    board_set(8, 3, WHITE);

    while (1) {
        board_draw();
        tdraw_delay(30);
    }

    tdraw_reset();
    return 0;
}
