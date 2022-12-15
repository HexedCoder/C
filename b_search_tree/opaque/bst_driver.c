#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include "tree.h"

void print_num(void *data)
{
	printf("%lu ", (uint64_t) data);
}

void print_str(void *data)
{
	person *tmp = (person *) data;
	printf("%s ", tmp->fname);
}

int person_compare(person * person_1, person * person_2)
{
	const char *name_1 = person_1->ssn;
	const char *name_2 = person_2->ssn;

	int index = 0;
	while (name_1[index] == name_2[index] && name_2[index]) {
		++index;
	}

	return name_2[index] - name_1[index];
}

int int_cmp(const void *input_1, const void *input_2)
{
	return (uint64_t) input_2 - (uint64_t) input_1;
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("No file name provided\n");
		return 1;
	}
	FILE *file;
	char *file_name = argv[1];
	file = fopen(file_name, "r");
	if (!file) {
		printf("Invalid file\n");
		return 1;
	}

	tree *tree = tree_create((int (*)(void *, void *))person_compare,
				 (void (*)(void *))print_str);
	int ret;
	int count = 1;
	char buffer[1024];
	while (fgets(buffer, 1024, file)) {
		person *tmp = calloc(1, sizeof(person));

		char *fields[8];
		char *p = buffer;

		for (int n = 0; n < 5; n++) {
			// save the pointer to the start of the field
			fields[n] = p;
			// skip to the end of the field
			p += strcspn(p, "\t\n");
			if (*p != '\0') {
				// set a null terminator for the field and skip to the next one
				*p++ = '\0';
			}
		}
		tmp->ssn = strdup(fields[0]);
		tmp->fname = strdup(fields[1]);
		tmp->lname = strdup(fields[2]);

		tmp->age = strtol(fields[3], NULL, 10);
		tmp->income = strtol(fields[4], NULL, 10);

		ret = tree_insert(tree, tmp);
		if (!ret) {
			return 1;
		}
	}
	fclose(file);

	printf("Arr Size: %d\n", count);

	printf("Preorder: ");
	preorder(tree);
	printf("\nPostorder: ");
	postorder(tree);
	printf("\nInorder: ");
	inorder(tree);
	printf("\nLevel order: ");
	levelorder(tree);

	printf("\nTree size: %d\n", tree_size(tree));

	void *sizes = tree_minimum(tree);
	if (sizes) {
		printf("Tree Min: %s\n", (char *)sizes);
	} else {
		printf("Tree Min: 0\n");
	}
	sizes = tree_maximum(tree);
	if (sizes) {
		printf("Tree Max: %s\n", (char *)sizes);
	} else {
		printf("Tree Max: 0\n");
	}

	int income = get_income(tree);
	printf("Average Income: $%d\n", income);

	income = get_age_income(tree, 50, 59);
	printf("Average Income between 50-59: $%d\n", income);

	//      print_visual(tree);

	tree_destroy(&tree);
}
