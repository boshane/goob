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
test_get_leaf_of_nthchar0(void)
{
	int n;
	node* dst = NULL;
	n = leaf_of_nthchar(*buf->firstline->root, 0, &dst);
	TEST_ASSERT_EQUAL_PTR(leaves[0], dst);
	TEST_ASSERT_EQUAL_INT(n, 3);
}
void
test_get_leaf_of_nthchar1(void)
{
	int n;
	node* dst = NULL;
	n = leaf_of_nthchar(*buf->firstline->root, 1, &dst);
	TEST_ASSERT_EQUAL_PTR(leaves[0], dst);
	TEST_ASSERT_EQUAL_INT(n, 2);
}
void
test_get_leaf_of_nthchar2(void)
{
	int n;
	node* dst = NULL;
	n = leaf_of_nthchar(*buf->firstline->root, 2, &dst);
	TEST_ASSERT_EQUAL_PTR(leaves[0], dst);
	TEST_ASSERT_EQUAL_INT(n, 1);
}
void
test_get_leaf_of_nthchar3(void)
{
	int n;
	node* dst = NULL;
	n = leaf_of_nthchar(*buf->firstline->root, 3, &dst);
	TEST_ASSERT_EQUAL_PTR(leaves[1], dst);
	TEST_ASSERT_EQUAL_INT(n, 3);
}
void
test_get_leaf_of_nthchar4(void)
{
	int n;
	node* dst = NULL;
	n = leaf_of_nthchar(*buf->firstline->root, 4, &dst);
	TEST_ASSERT_EQUAL_PTR(leaves[1], dst);
	TEST_ASSERT_EQUAL_INT(n, 2);
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
	TEST_ASSERT_EQUAL_PTR(leaves[1], n);

	n = get_next_leaf(leaves[1], NULL);
	TEST_ASSERT_EQUAL_PTR(leaves[2], n);

	n = get_next_leaf(leaves[2], NULL);
	TEST_ASSERT_EQUAL_PTR(leaves[3], n);

	n = get_next_leaf(leaves[3], NULL);
	TEST_ASSERT_EQUAL_PTR(leaves[4], n);

	n = get_next_leaf(leaves[4], NULL);
	TEST_ASSERT_EQUAL_PTR(leaves[5], n);

	n = get_next_leaf(leaves[5], NULL);
	TEST_ASSERT_EQUAL_PTR(leaves[6], n);

	n = get_next_leaf(leaves[6], NULL);
	TEST_ASSERT_EQUAL_PTR(leaves[7], n);
}

int
main(int argc, char **argv)
{
	prep_buffer();
	UNITY_BEGIN();
	RUN_TEST(test_get_nth_char);
	RUN_TEST(test_get_next_leaf);
	RUN_TEST(test_get_leaf_of_nthchar0);
	RUN_TEST(test_get_leaf_of_nthchar1);
	RUN_TEST(test_get_leaf_of_nthchar2);
	RUN_TEST(test_get_leaf_of_nthchar3);
	RUN_TEST(test_get_leaf_of_nthchar4);

	return UNITY_END();
}
