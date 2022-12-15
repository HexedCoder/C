#include <stdio.h>
#include <string.h>
#include <stdint-gcc.h>
#include "avl.h"

void print_num(void *data)
{
	printf("%d ", *(int *)data);
}

void print_str(void *data)
{
	printf("%s ", (char *)data);
}

int person_compare(char *name_1, char *name_2)
{
	int index = 0;
	while (name_1[index] == name_2[index] && name_2[index]) {
		++index;
	}

	return name_2[index] - name_1[index];
}

int int_cmp(const void *input_1, const void *input_2)
{
	return *(int *)input_1 - *(int *)input_2;
}

int main(void)
{
	uint64_t arr[] = { 90, 1, 8, 20, 89, 13, 81, 61, 62, 39, 96, 29, 93 };
	uint64_t arr2[] = { 90, 1, 8, 20, 89, 13, 81, 61, 62, 39, 96, 29, 93 };
	//      const char *arr[] = { "Kyle",  "Jacob",   "Chante", "Tim",
	//                            "Craig", "Sheriff", "Jill",   "Jillian" };

	size_t arr_size = sizeof(arr) / sizeof(arr[0]);

	avl_t *tree = tree_create((int (*)(void *, void *))int_cmp,
				  (void (*)(void *))print_num);
	int ret = 0;
	for (size_t i = 0; i < arr_size; ++i) {
		ret = *(int *)avl_insert(tree, &arr[i]);
		print_visual(tree);
		if (!ret) {
			return 1;
		}
	}
	printf("Arr Size: %zu\n", arr_size);

	printf("Preorder: ");
	preorder(tree);
	printf("Postorder: ");
	postorder(tree);
	printf("Inorder: ");
	inorder(tree);
	printf("Level order: ");
	level_order(tree);

	printf("Size: %d\n", tree_size(tree));

	void *sizes = tree_maximum(tree);
	if (sizes) {
		printf("Tree Max: %d\n", *(char *)sizes);
	} else {
		printf("Tree Max: 0\n");
	}
	sizes = tree_minimum(tree);
	if (sizes) {
		printf("Tree Min: %d\n", *(char *)sizes);
	} else {
		printf("Tree Min: 0\n");
	}

	for (int i = 0; i < arr_size; ++i) {
		printf("Attempting to delete: %ld\n", arr2[i]);
		ret = tree_delete(&tree, &arr2[i]);

		if (ret) {
			print_visual(tree);
			printf("Deleted: %ld\n", arr2[i]);
		}
	}

	print_visual(tree);

	printf("Size: %d\n", tree_size(tree));

	tree_destroy(&tree);
}
