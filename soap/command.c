#include "buffer.h"
#include "command.h"

extern buffer* buf;

void
terminal_set(struct termios *new)
{
	tcgetattr(STDIN_FILENO, new);
	new->c_lflag &= ~(ICANON | ECHO | IEXTEN | ISIG);
	new->c_iflag &= ~(BRKINT | ICRNL | ISTRIP | INPCK | IXON);
	new->c_oflag &= ~(OPOST);
	new->c_cc[VMIN] = 1;
	new->c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, new);
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
	if (sscanf(buf + 2, "%d;%d", row, col) != 2)
		return -1;

	return 0;
}

void
forward_char(int *col)
{
	int ropelen = rope_length(*buf->current_line->root);
	if (*col < ropelen)
		++*col;
	return;
}

void
backward_char(int *col)
{
	if (*col > 0)
		--*col;
	return;
}

void
forward_word(int *col)
{
	node *src, *cur;
	int offset;

	src = *buf->current_line->root;
	offset = leaf_of_nthchar(src, *col, &cur);

	while (*col < rope_length(src)) {
		while (offset != cur->len) {
			if (cur->str[offset] == ' ') {
				*col += offset;
				return;
			}
			offset++;
		}
		*col += offset;
		offset = 0;
		cur = get_next_leaf(cur, NULL);
	}
}

void
backward_word()
{
}

void
next_line()
{
}

void
previous_line()
{
}

