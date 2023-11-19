#include "../buffer.h"
#include "unity.h"

#define TESTFILE "../valoob"

struct buffer *buf;
struct node *leaves[48];
char* testline;
ssize_t linelen;
int leafn = 0;

void
populate_testline()
{
	FILE *fp;

	fp = fopen(TESTFILE, "r");

	size_t linen = 0;

	linelen = getline(&testline, &linen, fp);
}

void
populate_leaves(node **root, node **cur)
{
	if ((*cur)->left != NULL && (*cur)->right != NULL) {
		populate_leaves(root, &(*cur)->left);
		populate_leaves(root, &(*cur)->right);
	} else {
		leaves[leafn++] = *cur;
	}
}

void
prep_buffer()
{
	buf = malloc(sizeof(struct buffer));
	buffer_init(&buf, TESTFILE);
	populate_leaves(buf->firstline->root, buf->firstline->root);
	populate_testline();

	for (int i = 0; i < leafn; i++) {
		printf("%6s:%10p\n", leaves[i]->str, (void*)leaves[i]);
	}

}

void
setUp(void)
{
}

void
tearDown(void)
{
}

void
test_get_nth_char(void)
{
	char *c;

	for (int i = 0; i < linelen - 1; i++) {
		get_nth_char(*buf->firstline->root, i, &c);
		TEST_ASSERT_EQUAL_CHAR(testline[i], *c);
	}
}

void
test_get_next_leaf(void)
{
	node *n = get_next_leaf(leaves[0], NULL);
	TEST_MESSAGE(leaves[0]->str);
	TEST_ASSERT_EQUAL_PTR(leaves[1], n);

	n = get_next_leaf(leaves[1], NULL);
	TEST_MESSAGE(leaves[1]->str);
	TEST_ASSERT_EQUAL_PTR(leaves[2], n);

	n = get_next_leaf(leaves[2], NULL);
	TEST_MESSAGE(leaves[2]->str);
	TEST_ASSERT_EQUAL_PTR(leaves[3], n);

	n = get_next_leaf(leaves[3], NULL);
	TEST_MESSAGE(leaves[3]->str);
	TEST_ASSERT_EQUAL_PTR(leaves[4], n);

	n = get_next_leaf(leaves[4], NULL);
	TEST_MESSAGE(leaves[4]->str);
	TEST_ASSERT_EQUAL_PTR(leaves[5], n);

	n = get_next_leaf(leaves[5], NULL);
	TEST_MESSAGE(leaves[5]->str);
	TEST_ASSERT_EQUAL_PTR(leaves[6], n);

	n = get_next_leaf(leaves[6], NULL);
	TEST_MESSAGE(leaves[6]->str);
	TEST_ASSERT_EQUAL_PTR(leaves[7], n);
}

int
main(int argc, char **argv)
{
	prep_buffer();
	UNITY_BEGIN();
	RUN_TEST(test_get_nth_char);
	RUN_TEST(test_get_next_leaf);
	return UNITY_END();
}
