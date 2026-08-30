#ifndef MENU_H
#define MENU_H

typedef enum {
    OPTION_START,
    OPTION_HELP,
    OPTION_EXIT,
    OPTION_COUNT
} MenuOption;

/* Display and get user selection from the items list */
MenuOption menu_run(void);

#endif // MENU_H

