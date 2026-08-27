#ifndef INPUT_H
#define INPUT_H

typedef enum {
	INPUT_UP,
	INPUT_DOWN,
	INPUT_LEFT,
	INPUT_RIGHT,
	INPUT_SELECT,
    INPUT_ROTATE_CW,
    INPUT_ROTATE_CCW,
	INPUT_QUIT
} InputEvent;

/* Setup input configurations */
void input_init(void);

/* Get input event from user (blocking) */
InputEvent get_user_input(void);

#endif // INPUT_Hs

