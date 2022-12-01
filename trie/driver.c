#include <stdio.h>
#include <errno.h>

#include "trie.h"

#define MAX_LINE_LEN 128

int main()
{
	FILE *file = fopen("input", "r");
	if (!file) {
		printf("Error: %d\n", errno);
		exit(1);
	}

	// read lines from file
	char line[MAX_LINE_LEN];

	// create NULL node for dictionary
	node *dict_root = NULL;

	while (fgets(line, MAX_LINE_LEN, file) != NULL) {
		line[strcspn(line, "\n")] = 0;
		insert_node(&dict_root, line);
	}

	print_dictionary(dict_root);

	// close file
	fclose(file);
}