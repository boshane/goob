#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../soap/buffer.h"
#include "../soap/command.h"
#include "unistd.h"
#include "util.h"
#include "term.h"

typedef struct {
	int ci, cj;
} editor;

struct termios tios_prev;
struct termios tios_new;
struct winsize term_size;
buffer *buf;

void
menubar_display(int crow, int ccol)
{
	int row_dest = term_size.ws_row;
	double namelen = strlen(buf->filename);
	char rowcolbuf[32];
	int max = sizeof(rowcolbuf);
	int bufsz = snprintf(rowcolbuf, max, "[ %d, %d ]", crow, ccol);

	CURSOR_ROW(row_dest);
	BG_COLOR(128);
	printf("%s", rowcolbuf);
	for (int i = bufsz; i < term_size.ws_col-namelen; i++) {
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

	editor ed = { .ci=0, .cj=0 };

	term_init();
	buffer_init(&buf, argv[1]);
	CLEAR_SCREEN();
	buffer_display(&buf);
	fflush(stdout);
	//pretty_print_rope(*buf->firstline->root);
	char c, seq[3];
	int running = 1;

	CURSOR_ROW_COL(ed.ci+1, ed.cj+1);

	while (running) {
		get_cursor_position(STDIN_FILENO, STDOUT_FILENO, &ed.ci, &ed.cj);
		read(STDIN_FILENO, &c, 1);

		if (c == ESC) {
			read(STDIN_FILENO, seq, 1);

			switch (seq[0]) {
			case 'q':
				running = 0;
				break;
			case 'f':
				forward_word(&ed.ci, &ed.cj);
				break;
			case 'b':
				break;
			}
		} else {
			switch (c) {
			case CTRL_F:
				forward_char(&ed.cj);
				break;
			case CTRL_B:
				backward_char(&ed.cj);
				break;
			case CTRL_N:
				next_line(&ed.cj);
			default:
				break;
			}
		}
		menubar_display(ed.ci+1, ed.cj+1);
		CURSOR_ROW_COL(ed.ci+1, ed.cj+1);
		fflush(stdout);

	}
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &tios_prev);
	CLEAR_SCREEN();
	buffer_destroy(&buf);
	fflush(stdout);
	return 0;
}
