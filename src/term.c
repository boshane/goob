#include "term.h"

extern struct termios tios_prev;
extern struct termios tios_new;
extern struct winsize term_size;

void
term_init()
{
	int retval;

	retval = tcgetattr(STDIN_FILENO, &tios_prev);
	if (retval == -1) {
		exit(1);
	}

	retval = tcgetattr(STDIN_FILENO, &tios_new);
	if (retval == -1) {
		exit(1);
	}

	tios_new.c_lflag &= ~(ICANON | ECHO | IEXTEN | ISIG);
	tios_new.c_iflag &= ~(BRKINT | ICRNL | ISTRIP | INPCK | IXON);
	tios_new.c_oflag &= ~(OPOST);
	tios_new.c_cc[VMIN] = 1;
	tios_new.c_cc[VTIME] = 0;

	retval = tcsetattr(STDIN_FILENO, TCSAFLUSH, &tios_new);
	if (retval == -1) {
		exit(1);
	}

	retval = ioctl(1, TIOCGWINSZ, &term_size);
	if (retval == -1) {
		exit(1);
	}
}

int
get_cursor_position(int fdi, int fdo, int *row, int *col)
{
	char buf[32];
	unsigned int i = 0;

	if (write(fdo, "\x1b[6n", 4) != 4) return -1;

	while (i < sizeof(buf) - 1) {
		if (read(fdi, buf + i, 1) != 1)
			break;
		if (buf[i] == 'R')
			break;
		i++;
	}
	buf[i] = '\0';

	if (buf[0] != '\e' || buf[1] != '[')
		return -1;

	int ci, cj;
	if (sscanf(buf + 2, "%d;%d", &ci, &cj) != 2)
		return -1;
	*row = ci - 1;
	*col = cj - 1;

	return 0;
}
