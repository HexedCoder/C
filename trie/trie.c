#include "trie.h"
#include <stdlib.h>
#include <stdio.h>



node * createnode() {
	node *new_node = calloc(1, sizeof(*new_node)); // create mem for new node

	// loop through each char for string and add space for children
	for (int i = 0; i < NUM_CHARS; ++i) {
		new_node->children[i] = NULL;
	}

	new_node->end_node = false; // setting end to false when creating space

	return new_node;
}

bool insert_node(node **root, char *string_to_insert) {
	// if tree is empty, create new node and make it root
	if(*root == NULL) {
		*root = createnode();
	}

	unsigned char *string = (unsigned char *)string_to_insert;
	node *temp = *root; // temp pointer to root for traversing tree
	int length =  strlen(string_to_insert); // len of string sent into func

	for (int i = 0; i < length; i++) {
		if (temp->children[string[i]] == NULL) {
			// create new node for letter
			temp->children[string[i]] = createnode();
		}

		temp = temp->children[string[i]]; // moves temp to current's child
	}

	if(temp->end_node) {
		return false;
	}
	else {
		temp->end_node = true;
		return true;
	}
}

void print_dictionary_recursive(node * node, unsigned char * prefix, int length) {

	unsigned char new_prefix[length + 2]; // +2 for new char and null terminator
	memcpy(new_prefix, prefix, length); // copies old prefix to new with new len
	new_prefix[length + 1] = 0; // null terminates new prefix

	if(node->end_node) {
		// if terminal, print the actual word
		printf("Dict word: %s\n", prefix);
	}

	for (int i = 0; i < NUM_CHARS; i++) {
		if (node->children[i] != NULL) {
			new_prefix[length] = i;
			print_dictionary_recursive(node->children[i], new_prefix, length + 1);
		}
	}
}

void print_dictionary(node * root)
{
	if(root == NULL) {
		printf("Dictionary is empty!\n");
		return;
	}
	// root, no prefix yet, # of characters seen so far
	print_dictionary_recursive(root, NULL, 0);
}