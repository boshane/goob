#include "buffer.h"

const char *rootstring = "";

typedef enum {
        LF = 0x0D,
        CR = 0x0A
} control_chars;

int open_file(char* file, buffer **buf) {
	FILE *fp;

	size_t filestrn = strlen(file)+1;
	(*buf)->filename = malloc(filestrn);
	memcpy((*buf)->filename, file, filestrn);

	fp = fopen(file, "r");
	if (!fp) {
		return 1;
	}

	char* line = NULL;
	size_t linen = 0;
	ssize_t linelen;
	while((linelen = getline(&line, &linen, fp)) != -1) {
		if (linelen && (line[linelen-1] == '\n' || line[linelen-1] == '\r'))
			line[--linelen] = '\0';
		buffer_append_string(buf, line);
	}
	free(line);
	fclose(fp);
	return 0;
}

void
rope_destroy(node *rope)
{
	if (rope->left && rope->right) {
		rope_destroy(rope->left);
		rope_destroy(rope->right);
		free(rope);
	} else {
		free(rope->str);
		free(rope);
	}
}

void
rope_print(node *rope)
{
	if (rope->left && rope->right) {
		rope_print(rope->left);
		rope_print(rope->right);
	} else {
		printf("%s", rope->str);
	}
}

void
buffer_destroy(buffer **buf)
{
	line *tmp = (*buf)->firstline;
	line *prev = tmp;

	while (tmp->next != NULL) {
		rope_destroy((*tmp->root));
		prev = tmp;
		tmp = tmp->next;
		free(prev);
	}
}

void
buffer_display(buffer **buf)
{
	line *current_line = (*buf)->firstline;
	printf("\x1b[1;1H");

	while (current_line != NULL) {
		rope_print(*current_line->root);
		printf("\x1b[E");
		current_line = current_line->next;
	}
	printf("\x1b[1;1H");
}

void
buffer_append_string(buffer **buf, char *string)
{
	line *tmp = (*buf)->firstline;

	line *newline = malloc(sizeof(line));
	newline->root = malloc(sizeof(node));
	newline->next = NULL;

	int length = strlen(string) + 1;

	make_node(NULL, newline->root, string, 0, length);

	/* if there is a first line, iterate through the lines to get to the last one
	 * otherwise, make the new line the first line                                */
	if (tmp) {
		while (tmp->next != NULL) {
			tmp = tmp->next;
		}
		tmp->next = newline;
	} else
		(*buf)->firstline = newline;
}

void
buffer_init(buffer **buf, char *str)
{
	(*buf)->firstline = NULL;
	(*buf)->filename = str;
	open_file(str, buf);
	(*buf)->current_line = (*buf)->firstline;
}

int
rope_length(node *root)
{
	node* tmp = root;
	int length = tmp->len;

	while (tmp->right != NULL) {
		tmp = tmp->right;
		length += tmp->len;
	}
	return length;
}

/* Store the leaf that is holding the nth character in the rope into dest
 * and return the remaining number of characters before the end of the leaf
 * is reached. */
int
leaf_of_nthchar(node *root, int n, node** dest)
{
	if (root->left == NULL && root->right == NULL) {
		if (n > root->len) {
			*dest = NULL;
			return -1;
		} else {
			*dest = root;
			return root->len - n;
		}
	}
	if ((n - root->len) < 1) {
		leaf_of_nthchar(root->left, n, dest);
	} else
		leaf_of_nthchar(root->right, n - root->len, dest);
	return -1;
}

void
get_nth_char(node *root, int n, char **c)
{
	if (root->left == NULL && root->right == NULL) {
		if (n > root->len) {
			*c = NULL;
			return;
		} else {
			*c = &root->str[n];
			return;
		}
	}
	int result = n - root->len;
	if (result < 0) {
		get_nth_char(root->left, n, c);
	} else
		get_nth_char(root->right, result, c);
}

node *
get_next_leaf(node *cur, node *prev)
{
	do {
		prev = cur;
		cur = cur->par;
	}
	while(cur->right == prev);

	return get_first_leaf(cur->right);
}

node *
get_first_leaf(node *n)
{
	node *tmp = n;
	while (tmp->left != NULL) {
		tmp = tmp->left;
	}

	return tmp;
}

void
get_last_leaf(node *root, node **last)
{
	node *tmp = root;

	while (tmp->right != NULL) {
		tmp = tmp->right;
	}
	*last = tmp;
}

node *
get_root_node(node *leaf)
{
	node *tmp = leaf;

	while (tmp->par != NULL) {
		tmp = tmp->par;
	}
	return tmp;
}

void
make_node(node *parent, node **cur, const char *string, int l, int r)
{
	node *tmp = malloc(sizeof(node));
	tmp->left = tmp->right = NULL;
	if (parent) {
		tmp->par = parent;
	}

	int mod = (l + r) / 2;

	if ((r - l) > MAX_LEN) {
		tmp->str = NULL;
		tmp->len = (r - l) / 2;
		/* if the length of the node is odd, we need to keep the left
		 * side longer than the right */
		if ((r - l) % 2 != 0) {
			tmp->len++;
			*cur = tmp;
			make_node(*cur, &(*cur)->left, string, l, mod + 1);
			make_node(*cur, &(*cur)->right, string, mod + 1, r);
		} else {
			*cur = tmp;
			make_node(*cur, &(*cur)->left, string, l, mod);
			make_node(*cur, &(*cur)->right, string, mod, r);
		}
	} else {
		tmp->str = malloc((r - l) * sizeof(char));
		tmp->len = (r - l);

		int j = 0;
		for (int i = l; i < r; i++) {
			tmp->str[j++] = string[i];
		}
		*cur = tmp;
	}
}

