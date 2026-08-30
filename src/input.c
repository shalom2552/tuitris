#define _DEFAULT_SOURCE
#include "input.h"

#include <sys/select.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>

static struct termios original_termios;

/* Revert the terminal stage to it's original state */
static void cleanup(void) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);
    write(STDOUT_FILENO, "\033[?25h", 6);
}

/* Handle the SIGINT (ctl+c) to exit safely */
static void handle_sigint_signal(int sig)
{
	(void)sig;
	exit(0);
}

void input_init(void) {
	tcgetattr(STDIN_FILENO, &original_termios);
	write(STDOUT_FILENO, "\033[?25l", 6);

	atexit(cleanup);

	signal(SIGINT, handle_sigint_signal);

	struct termios raw = original_termios;
	raw.c_lflag &= ~(ECHO | ICANON);
	raw.c_cc[VMIN] = 1;
	raw.c_cc[VTIME] = 0;

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

InputEvent get_user_input(void)
{
	char c;
	while (1) {
		fd_set readfds;
		FD_ZERO(&readfds);
		FD_SET(STDIN_FILENO, &readfds);
		select(STDIN_FILENO + 1, &readfds, NULL, NULL, NULL);
		if (read(STDIN_FILENO, &c, 1) != 1) {
			continue;
		}

		switch (c) {
			case 'q': return INPUT_QUIT;
			case ' ': case 'r': return INPUT_ROTATE_CW;
			case '\n': return INPUT_SELECT;
            case '+': return INPUT_PLUS;
            case '-': return INPUT_MINUS;
            case 'p': return INPUT_PAUSE;
			case '\033':
				{
					char seq[2];
					if (read(STDIN_FILENO, &seq[0], 1) != 1 ||
						read(STDIN_FILENO, &seq[1], 1) != 1)  {
						return INPUT_QUIT;
					}

					if (seq[0] == '[') {
						switch (seq[1]) {
							case 'A': return INPUT_UP;
							case 'B': return INPUT_DOWN;
							case 'C': return INPUT_RIGHT;
							case 'D': return INPUT_LEFT;
						}
					}
				}
				break;
		}
	}
}

