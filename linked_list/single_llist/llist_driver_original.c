#include "llist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct node_t node_t;

struct node_t {
	char value;
};

void char_destroy(node_t *node)
{
	if (!node) {
		return;
	}

	free(node);
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

void reverse_q(llist_t *queue)
{
	llist_t *stack = llist_create();

	node_t *val;

	while (get_size(queue)) {
		val = llist_dequeue(queue);
		printf("%c", val->value);
		llist_push(stack, val);
	}
	while (get_size(stack)) {
		val = llist_pop(stack);
		printf("%c", val->value);
		char_destroy(val);
	}
}

void enqueue_dequeue(const char *input)
{
	llist_t *queue = llist_create();
	size_t len = strlen(input);
	for (int i = 0; i < len; ++i) {
		if (input[i] >= 65 && input[i] <= 90) {
			node_t *node = node_create(input[i]);
			llist_enqueue(queue, node);
		}
	}
	reverse_q(queue);
	//EXIT_ENQUEUE_DEQUEUE:
	printf("\n");
	llist_delete(queue, (void (*)(void *))char_destroy);
}

void push_pop(const char *input)
{
	llist_t *stack = llist_create();
	size_t len = strlen(input);
	for (size_t i = 0; i < len; ++i) {
		if (input[i] == '*') {
			if (get_size(stack)) {
				node_t *val = llist_pop(stack);
				printf("%c", val->value);
				char_destroy(val);
			} else {
				printf("Pop called on empty stack");
				goto EXIT_PUSH_POP;
			}
		} else if (input[i] >= 65 && input[i] <= 90) {
			node_t *node = node_create(input[i]);
			llist_push(stack, node);
		}
	}
EXIT_PUSH_POP:
	printf("\n");
	llist_delete(stack, (void (*)(void *))char_destroy);
}

void reverse(const char *input)
{
	llist_t *stack = llist_create();
	size_t len = strlen(input);

	printf("Original: ");
	for (int i = 0; i < len; ++i) {
		node_t *node = node_create(input[i]);
		printf("%c", node->value);
		llist_push(stack, node);
	}

	printf("\t\tReverse: ");
	while (get_size(stack)) {
		node_t *val = llist_pop(stack);
		printf("%c", val->value);
		char_destroy(val);
	}
	printf("\n");

	llist_delete(stack, (void (*)(void *))char_destroy);
}

void double_stack_queue(const char *input)
{
	llist_t *stack = llist_create();
	llist_t *stack2 = llist_create();

	node_t *val = NULL;

	size_t len = strlen(input);
	for (int i = 0; i < len; ++i) {
		if (input[i] == '*') {
			if (!get_size(stack2)) {
				while (get_size(stack)) {
					val = llist_pop(stack);
					llist_push(stack2, val);
				}
			}

			val = llist_pop(stack2);
			printf("%c", val->value);
			free(val);
		} else if (input[i] >= 65 && input[i] <= 90) {
			node_t *node = node_create(input[i]);
			llist_push(stack, node);
		}
	}
	printf("\n");
	llist_delete(stack, (void (*)(void *))char_destroy);
	llist_delete(stack2, (void (*)(void *))char_destroy);
}

void int_to_bin(int value)
{
	for (int i = 31; i >= 0; --i) {
		printf("%d", (value >> i) & 1);
	}
	printf("\n");
}

void char_print(char value)
{
	if (!value) {
		return;
	}

	printf("%d ", value);
}

int main(void)
{
	const char *input1 = "E A S * Y * Q U E * * * S T * * * I O * N * * *";
	printf("Ex 1: ");
	push_pop(input1);

	const char *input2 =
		"L A * S T I * N * F I R * S T * * O U * T * * * * * *";
	printf("Ex 2: ");
	push_pop(input2);

	const char *invalid_input = "* * * * * *";
	printf("Ex 3: ");
	push_pop(invalid_input);

	const char *input4 = "E A S * Y * Q U E * * * S T * * * I O * N * * *";
	printf("Ex 4: ");
	enqueue_dequeue(input4);

	const char *input7 = "This will be a great day!";
	printf("Ex 5: ");
	reverse(input7);

	printf("Ex 6: INT_MAX = \n");
	for (int i = 0; i < 31; ++i) {
		int_to_bin(1 << i);
	}
	int_to_bin(INT_MAX);

	printf("Ex 7: ");
	double_stack_queue(input4);
}
