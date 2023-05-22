/** @file llist_check.c
 *
 * @brief llist_check.c tests public functions from llist.h
 *
 * COPYRIGHT NOTICE: (c) 2022 Jacob Hitchcox
 *
 */

#include "../src/llist.h"
#include <check.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node_t node_t;

struct node_t {
	char value;
};

int char_cmp(void *char_1, void *char_2)
{
	return *(char *)char_2 - *(char *)char_1;
}

struct node_t *node_create(char value)
{
	node_t *node = calloc(1, sizeof(*node));

	if (!value) {
		goto PERSON_CREATE_EXIT;
	}

	if (node) {
		node->value = value;
	}
 PERSON_CREATE_EXIT:
	return node;
}

llist_t *setup_stack(const char *string, size_t len)
{
	llist_t *stack = llist_create();
	if (!stack) {
		return NULL;
	}

	for (size_t i = 0; i < len; ++i) {
		if (string[i] == '*') {
			if (get_size(stack)) {
				void *value = llist_pop(stack);
				free(value);
			} else {
				break;
			}
		} else {
			node_t *node = node_create(string[i]);
			llist_push(stack, node);
		}
	}
	return stack;
}

llist_t *setup_queue(const char *string, size_t len)
{
	llist_t *queue = llist_create();
	if (!queue) {
		return NULL;
	}

	for (size_t i = 0; i < len; ++i) {
		if (string[i] == '*') {
			if (get_size(queue)) {
				void *value = llist_dequeue(queue);
				free(value);
			} else {
				break;
			}
		} else {
			node_t *node = node_create(string[i]);
			llist_enqueue(queue, node);
		}
	}
	return queue;
}

START_TEST(test_valid_llist_create)
{
	llist_t *stack = llist_create();
	ck_assert(stack != NULL);
	llist_delete(stack, (void (*)(void *))free);
}

END_TEST START_TEST(test_valid_stack_operation)
{
	const char *valid_string = "THIS IS A VALID STRING";
	size_t len = strlen(valid_string);
	llist_t *stack = setup_stack(valid_string, len);

	for (int i = 0; i < len; ++i) {
		node_t *curr = (node_t *) llist_pop(stack);
		ck_assert(curr->value == valid_string[len - 1 - i]);
		free(curr);
	}
	ck_assert(get_size(stack) == 0);
	llist_delete(stack, (void (*)(void *))free);
}

END_TEST START_TEST(test_valid_queue_operation)
{
	const char *valid_string = "THIS IS A VALID STRING";
	size_t len = strlen(valid_string);
	llist_t *queue = setup_queue(valid_string, len);

	for (int i = 0; i < len; ++i) {
		node_t *curr = (node_t *) llist_dequeue(queue);
		ck_assert(curr->value == valid_string[i]);
		free(curr);
	}
	ck_assert(get_size(queue) == 0);
	llist_delete(queue, (void (*)(void *))free);
}

END_TEST START_TEST(test_valid_extract_back_operation)
{
	const char *valid_string = "THIS IS A VALID STRING";
	size_t len = strlen(valid_string);
	llist_t *llist = setup_queue(valid_string, len);

	for (int i = 0; i < len; ++i) {
		node_t *curr = (node_t *) llist_extract_back(llist);
		ck_assert(curr->value == valid_string[len - 1 - i]);
		free(curr);
	}

	ck_assert(get_size(llist) == 0);
	llist_delete(llist, (void (*)(void *))free);
}

END_TEST START_TEST(test_valid_find_operation)
{
	const char *valid_string = "THIS IS A VALID STRING";
	size_t len = strlen(valid_string);
	size_t search_idx = 6;

	llist_t *llist = setup_queue(valid_string, len);

	node_t *curr = (node_t *) llist_find(llist,
					     (void *)&valid_string[search_idx],
					     char_cmp);
	ck_assert(curr->value == valid_string[search_idx]);

	llist_delete(llist, (void (*)(void *))free);
}

END_TEST START_TEST(test_invalid_stack_operation)
{
	llist_t *stack = llist_create();
	node_t *curr = (node_t *) llist_pop(stack);
	ck_assert_ptr_null(curr);
	free(stack);
}

END_TEST START_TEST(test_invalid_queue_operation)
{
	llist_t *stack = llist_create();
	node_t *curr = (node_t *) llist_dequeue(stack);
	ck_assert_ptr_null(curr);
	free(stack);
}

END_TEST START_TEST(test_invalid_extract_back_operation)
{
	llist_t *stack = llist_create();
	node_t *curr = (node_t *) llist_extract_back(stack);
	ck_assert_ptr_null(curr);
	free(stack);
}

END_TEST START_TEST(test_invalid_find_operation)
{
	const char *valid_string = "THIS IS A VALID STRING";
	size_t len = strlen(valid_string);
	char search_char = 'Z';

	llist_t *llist = setup_queue(valid_string, len);

	node_t *curr =
		(node_t *)llist_find(llist, (void *)&search_char, char_cmp);
	ck_assert_ptr_null(curr);

	llist_delete(llist, (void (*)(void *))free);
}

END_TEST

START_TEST(test_linked_list_ops)
{
	fprintf(stderr, "-----LLIST----\n");

	fprintf(stderr, "Start: test_linked_list_ops\n");
	llist_t *list = llist_create();

	char *word_list[] = { (char *)"Word", (char *)"Word2", (char *)"Word3",
			      (char *)"Word4", (char *)"Word5" };

	fprintf(stderr, "\tTest: llist_enqueue/get_size\n");
	for (size_t i = 0; i < 5; ++i) {
		llist_enqueue(list, word_list[i]);

		// null inputs should not append
		llist_enqueue(list, NULL);
		ck_assert(get_size(list) == i + 1);
	}

	fprintf(stderr, "\tTest: llist_pop/get_size\n");
	for (size_t i = 5; i > 0; --i) {
		llist_pop(list);
		ck_assert(get_size(list) == i - 1);
	}

	fprintf(stderr, "\tTest: llist_print (empty list)\n");
	// Nothing should print
	llist_print(list, word_print);

	for (size_t i = 0; i < 5; ++i) {
		// null inputs should not append
		llist_enqueue(list, NULL);
		llist_enqueue(list, word_list[i]);
	}

	fprintf(stderr, "\tTest: llist_print/llist_clear/get_size\n");
	// each element should print on its own line
	llist_print(list, word_print);
	llist_clear(list);
	ck_assert(get_size(list) == 0);

	fprintf(stderr, "End: test_linked_list_ops\n");

	fprintf(stderr, "-----END LLIST----\n\n");
}

END_TEST

Suite *check_llist(void)
{
	Suite *suite;

	TCase *tc_valid, *tc_invalid, *tc_all;

	suite = suite_create("llist_tests");

	tc_valid = tcase_create("Valid");
	tcase_set_tags(tc_valid, "Valid");

	tcase_add_test(tc_valid, test_valid_llist_create);
	tcase_add_test(tc_valid, test_valid_stack_operation);
	tcase_add_test(tc_valid, test_valid_queue_operation);
	tcase_add_test(tc_valid, test_valid_extract_back_operation);
	tcase_add_test(tc_valid, test_valid_find_operation);
	tcase_add_test(tc_valid, test_linked_list_ops);

	tc_invalid = tcase_create("Invalid");
	tcase_set_tags(tc_invalid, "Invalid");

	tcase_add_test(tc_invalid, test_invalid_stack_operation);
	tcase_add_test(tc_invalid, test_invalid_queue_operation);
	tcase_add_test(tc_invalid, test_invalid_extract_back_operation);
	tcase_add_test(tc_invalid, test_invalid_find_operation);

	tc_all = tcase_create("All");
	tcase_set_tags(tc_all, "All");

	suite_add_tcase(suite, tc_valid);
	suite_add_tcase(suite, tc_invalid);
	suite_add_tcase(suite, tc_all);

	return suite;
}

int main()
{
	Suite *suite = check_llist();
	SRunner *runner = srunner_create(suite);

	srunner_set_fork_status(runner, CK_NOFORK);

	//	srunner_run_tagged(runner, NULL, NULL, "Valid", NULL, CK_VERBOSE);
	srunner_run_all(runner, CK_VERBOSE);

	int no_failed = srunner_ntests_failed(runner);

	srunner_free(runner);
	return (no_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

/*** end of file***/
