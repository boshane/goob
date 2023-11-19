#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../soap/buffer.h"
#include "../soap/command.h"
#include "util.h"

#define CLEAR_SCREEN "\x1b[H\x1b[J"
#define BG_COLOR(c) (printf("\x1b[48;5;%dm", c))
#define RESET_COLOR() (printf("\x1b[0m"))
#define CURSOR_ROW_COL(i, j) (printf("\x1b[%d;%dH",i,j))
#define CURSOR_ROW(i) (printf("\x1b[H\x1b[%dB",i))
#define CURSOR_COL(j) (printf("\x1b[H\x1b[%dC",j))

enum TERM_KEYS {
	KEY_NULL = 0,
	CTRL_B = 2,
	CTRL_D = 4,
	CTRL_F = 6,
	CTRL_N = 14,
	CTRL_P = 16,
	CTRL_Q = 17,
	ESC = 27
};

typedef struct {
	struct termios termios_prev;
	struct termios termios_new;
	struct winsize termsize;
	buffer *active;
	int ci, cj;
} termstate;

/* the buffer for the file */
termstate term;
buffer *buf;

void
term_init()
{
	int retval;

	retval = tcgetattr(STDIN_FILENO, &term.termios_prev);
	if (retval == -1) {
		exit(1);
	}
	retval = tcgetattr(STDIN_FILENO, &term.termios_new);
	if (retval == -1) {
		exit(1);
	}

	term.termios_new.c_lflag &= ~(ICANON | ECHO | IEXTEN | ISIG);
	term.termios_new.c_iflag &= ~(BRKINT | ICRNL | ISTRIP | INPCK | IXON);
	term.termios_new.c_oflag &= ~(OPOST);
	term.termios_new.c_cc[VMIN] = 1;
	term.termios_new.c_cc[VTIME] = 0;

	retval = tcsetattr(STDIN_FILENO, TCSAFLUSH, &term.termios_new);
	if (retval == -1) {
		exit(1);
	}

	retval = ioctl(1, TIOCGWINSZ, &term.termsize);
	if (retval == -1) {
		exit(1);
	}
	term.ci = 1;
	term.cj = 1;
	CURSOR_ROW_COL(term.ci,term.cj);
}

void
menubar_display(buffer *buf)
{
	int row_dest = term.termsize.ws_row;
	double namelen = strlen(buf->filename);
	char rowcolbuf[32];
	int max = sizeof(rowcolbuf);
	int bufsz = snprintf(rowcolbuf, max, "[ %d, %d ]", term.ci, term.cj);

	CURSOR_ROW(row_dest);
	BG_COLOR(128);
	printf("%s", rowcolbuf);
	for (int i = bufsz; i < term.termsize.ws_col-namelen; i++) {
		putchar(' ');
	}
	printf("%s", buf->filename);
	RESET_COLOR();
}

int
main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    buf = malloc(sizeof(struct buffer));

    term_init();
    buffer_init(&buf, argv[1]);
    puts(CLEAR_SCREEN);
    buffer_display(&buf);
	menubar_display(buf);
//	pretty_print_rope(*buf->firstline->root);
	CURSOR_ROW_COL(1,1);
    fflush(stdout);

    char c, seq[3];
    int running = 1;

    while (running) {
	    int getcursor;
	    getcursor = get_cursor_position(STDIN_FILENO, STDOUT_FILENO, &term.ci,
		&term.cj);
	    if (getcursor == -1)
		    running = 0;

	    read(STDIN_FILENO, &c, 1);

	    if (c == ESC) {
		    read(STDIN_FILENO, seq, 1);

		    switch (seq[0]) {
		    case 'q':
			    running = 0;
			    break;
		    case 'f':
				forward_word(&term.cj);
			    break;
		    case 'b':
			    break;
		    }
	    } else {
		    switch (c) {
		    case CTRL_F:
			    forward_char(&term.cj);
			    break;
		    case CTRL_B:
				backward_char(&term.cj);
			    break;
			case CTRL_N:
				next_line(&term.cj);
		    default:
			    break;
		    }
	    }
		menubar_display(buf);
	    CURSOR_ROW_COL(term.ci, term.cj);
	    fflush(stdout);
    }
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &term.termios_prev);
	puts(CLEAR_SCREEN);
	buffer_destroy(&buf);
	fflush(stdout);
	return 0;
}
