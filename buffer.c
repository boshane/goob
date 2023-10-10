#include "buffer.h"

const char* rootstring = "01234567890";

void 
rope_destroy(node* rope) {
  if (rope->left && rope->right) {
    rope_destroy(rope->left);
    rope_destroy(rope->right);
    free(rope);
  }
  else {
    free(rope->str);
    free(rope);
  }
}

void
rope_print(node* rope) {
  if (rope->left && rope->right) {
    rope_print(rope->left);
    rope_print(rope->right);
  }
  else {
    printf("%s", rope->str);
  }
}

void
buffer_destroy() {
  line *tmp = buf.firstline;
  line *prev = tmp;

  while(tmp->next != NULL) {
    rope_destroy((*tmp->root));
    prev = tmp;
    tmp = tmp->next;
    free(prev);
  }
}

void
buffer_print() {
  line *current_line = buf.firstline;
  
  while (current_line != NULL) {
    rope_print(*current_line->root);
    puts("");
    current_line = current_line->next;
  }
}

void
buffer_append_string(const char* string) {
  line *tmp = buf.firstline;

  while (tmp->next != NULL) {
    tmp = tmp->next;
  }
  
  line *newline = malloc(sizeof(line));
  newline->root = malloc(sizeof(node));
  newline->next = NULL;

  size_t length = strlen(string);

  make_node(NULL, newline->root, string, 0, length);

  tmp->next = newline;
}

void
buffer_create() {
  buf.firstline = malloc(sizeof(struct line)); 
  buf.firstline->next = NULL;
  buf.firstline->root = malloc(sizeof(node));
  make_node(NULL, buf.firstline->root, rootstring, 0, strlen(rootstring));
  buf.current_line = buf.firstline;
  buf.current_leaf = get_first_leaf(*buf.firstline->root);
  buf.firstline->leafcol = 0;
  buf.firstline->linecol = 0;
}

void
get_nth_leaf(node* root, node** leaf, int n) {
    int res = n - root->len;

    if(root->left == NULL && root->right == NULL) {
        *leaf = root;
        return;
    }

    if(res < 1) {
        get_nth_leaf(root->left, leaf, n);
    }
    else get_nth_leaf(root->right, leaf, res);
}


cursor_movement_t
cursor_char_forward() {
    if(buf.current_line->leafcol < buf.current_leaf->len) {
        buf.current_line->leafcol++;
        return FORWARD_CHAR;
    }

    int n = buf.current_line->leafcol;
    n++;
    node* tmp = NULL;
    get_nth_leaf(*buf.current_line->root, &tmp, n);

    if(buf.current_leaf != get_last_leaf(*buf.current_line->root)) {
        buf.current_leaf = tmp;
        buf.current_line->leafcol = 0;
        return FORWARD_CHAR;
    }

    return END_OF_FILE;
}

node* 
get_first_leaf(node* n) {
    node* tmp = n;

    while(tmp->left != NULL) {
        tmp = tmp->left;
    }

    return tmp;
}

node* 
get_last_leaf(node* n) {
    node* tmp = n;

    while(tmp->right != NULL) {
        tmp = tmp->right;
    }

    return tmp;
}

node*
get_root_node(node* leaf) {
    node* tmp = leaf;

    while (tmp->par != NULL) {
        tmp = tmp->par;
    }
    return tmp;
}

void
make_node(node* parent, node** cur, const char* string, int l, int r) {
  node* tmp = malloc(sizeof(node));
  tmp->left = tmp->right = NULL;
  if (parent) {
    tmp->par = parent;
  }

  int mod = (l + r) / 2;
 
  if ((r - l) > MAX_LEN) {
    tmp->str = NULL;
    tmp->len = (r - l) / 2;
    /* if the length of the node is odd, we need to keep the left side longer than the right */
    if ((r - l) % 2 != 0) {
        tmp->len++;
        *cur = tmp;
        make_node(*cur, &(*cur)->left, string, l, mod+1);
        make_node(*cur, &(*cur)->right, string, mod+1, r);
    }
    else {
        *cur = tmp;
        make_node(*cur, &(*cur)->left, string, l, mod);
        make_node(*cur, &(*cur)->right, string, mod, r);
    }
  }
  else {
    tmp->str = malloc((r - l) * sizeof(char));
    tmp->len = (r - l);

    int j = 0;
    for (int i = l; i < r; i++) {
      tmp->str[j++] = string[i];
    }
    *cur = tmp;
  }
}

