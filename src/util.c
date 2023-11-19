#include "../soap/buffer.h"
#include "util.h"

#define NODE_H 1
#define NODE_W 3
#define SPACING 5
#define CORNER '.'
#define VERTICAL '|'
#define HORIZONTAL '-'

typedef struct position {
	int x, y;
} position;

void
draw_box(char **output, node *cur, position pos)
{
	/* draw the vertical sides of the box */
	for (int row = pos.y - NODE_H; row <= pos.y + NODE_H; row++) {
		output[row][pos.x - NODE_W] = VERTICAL;
		output[row][pos.x + NODE_W] = VERTICAL;
	}
	/* draw the horizontal top and bottom of the box, unless it's an edge,
	 * in which case print a '.' to indicate a corner */
	for (int col = pos.x - NODE_W; col <= pos.x + NODE_W; col++) {
		char c;

		if (col == pos.x - NODE_W || col == pos.x + NODE_W)
			c = CORNER;
		else
			c = HORIZONTAL;

		output[pos.y - NODE_H][col] = c;
		output[pos.y + NODE_H][col] = c;
	}
	/* print the node size into the middle of the box */
	char nodesize[3];
	int n = sprintf(nodesize, "%d", cur->len);
	memcpy(&output[pos.y][pos.x], nodesize, n);
}

void
draw_node(char **output, node *cur, int median, int mod, int depth)
{
	position pos = { .x = median, .y = depth * SPACING };

	draw_box(output, cur, pos);

	/* if we have reached a leaf, just print the string below the last drawn
	 * box */
	if (cur->left == NULL && cur->right == NULL) {
		char leafstr[10];
		int strlen;
		strlen = sprintf(leafstr, "%s:%d", cur->str, cur->len);
		memcpy(&output[2 + (depth * SPACING)][median - 2], leafstr,
		    strlen);
		return;
	}
	draw_node(output, cur->right, median + mod, mod / 2, depth + 1);
	draw_node(output, cur->left, median - mod, mod / 2, depth + 1);
}

void
pretty_print_rope(struct node *root)
{
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	/* investigate using setvbuf() */

	int rows = w.ws_row;
	int cols = w.ws_col;
	int depth = 1;

	char **output = malloc(rows * sizeof(char *));
	for (int i = 0; i < cols; i++) {
		output[i] = malloc(cols * sizeof(char));
	}

	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < cols; x++) {
			output[y][x] = ' ';
		}
	}

	int mod = w.ws_col / 4;
	int margin = w.ws_col / 2;
	if (root->par == NULL) {
		position center = { .x = w.ws_col / 2, .y = depth * SPACING };
		draw_box(output, root, center);
		char nodesize[3];
		int n = sprintf(nodesize, "%d", root->len);
		memcpy(&output[center.y][center.x], nodesize, n);
		depth++;
	}

	draw_node(output, root->left, margin - mod, mod / 2, depth);
	draw_node(output, root->right, margin + mod, mod / 2, depth);

	printf("\x1b[20;1H");
	putchar('\n');
	for (int j = 0; j < rows; j++) {
		for (int i = 0; i < cols; i++) {
			putchar(output[j][i]);
		}
	}
}
