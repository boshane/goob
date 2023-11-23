#include "buffer.h"
#include "command.h"

extern buffer* buf;

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

/* Advance forward one 'word' delimited by a space */
void
forward_word(int *row, int *col)
{
	node *src, *cur;
	int offset, total, ropelen;

	src = *buf->current_line->root;
	offset = leaf_of_nthchar(src, *col, &cur);

	ropelen = rope_length(src);
	total = 0;

	while ((*col + total) < ropelen) {
		do {
			if (cur->str[offset] == ' ') {
				if (total != 0) {
					*col += ++total;
					return;
				}
			}
			total++;
			offset++;
		} while(offset < cur->len);

		if ((*col + total) >= ropelen) {
			*col = ropelen;
			return;
		}
		cur = get_next_leaf(cur, NULL);
		offset = 0;
	}
	if ((*col + total) >= ropelen) {
		*buf->current_line = *buf->current_line->next;
		*col = 0;
		++*row;
		return;
	}

	*col += total;
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

