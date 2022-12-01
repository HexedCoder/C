/** @file trie.c
*
* @brief This module defines the templates from trie.h
 * Adopted by Jacob Sorber https://www.youtube.com/watch?v=NDfAYZCHstI
*
* @par
* COPYRIGHT NOTICE: (c) 2022 Jacob Hitchcox
*/

#include "trie.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

node *create_node()
{
	node *new_node =
		calloc(1, sizeof(*new_node)); // create mem for new node

	if (!new_node) {
		return NULL;
	}

	// loop through each char for string and add space for children
	for (int i = 0; i < NUM_CHARS; ++i) {
		new_node->children[i] = NULL;
	}

	new_node->end_node = false; // setting end to false when creating space

	return new_node;
}

bool insert_node(node **root, char *string_to_insert)
{
	// if tree is empty, create new node and make it root
	if (NULL == *root) {
		*root = create_node();
	}

	unsigned char *string = (unsigned char *)string_to_insert;
	node *temp = *root; // temp pointer to root for traversing tree
	size_t length =
		strlen(string_to_insert); // len of string sent into func

	for (size_t i = 0; i < length; i++) {
		if (temp->children[string[i]] == NULL) {
			// create new node for letter
			temp->children[string[i]] = create_node();
		}

		temp = temp->children[string[i]]; // moves temp to current's child
	}

	if (temp->end_node) {
		return false;
	} else {
		temp->end_node = true;
		return true;
	}
}

static void print_dictionary_rec(node *node, unsigned char *prefix, int length)
{
	unsigned char
		new_prefix[length + 2]; // +2 for new char and null terminator
	memcpy(new_prefix, prefix,
	       length); // copies old prefix to new with new len
	new_prefix[length + 1] = 0; // null terminates new prefix

	if (node->end_node) {
		// if terminal, print the actual word
		printf("Word: %s\n", prefix);
	}

	for (int i = 0; i < NUM_CHARS; i++) {
		if (node->children[i] != NULL) {
			new_prefix[length] = i;
			print_dictionary_rec(node->children[i], new_prefix,
					     length + 1);
		}
	}
}

void trie_print(node *root)
{
	if (root == NULL) {
		printf("Dictionary is empty!\n");
		return;
	}
	// root, no prefix yet, # of characters seen so far
	print_dictionary_rec(root, NULL, 0);
}

bool trie_search(node *root, const char *signed_text)
{
	unsigned char *text = (unsigned char *)signed_text;
	size_t text_len = strlen(signed_text);
	node *tmp = root;

	for (size_t i = 0; i < text_len; i++) {
		if (NULL == tmp->children[text[i]]) {
			return false;
		}
		tmp = tmp->children[text[i]];
	}
	return tmp->end_node;
}

static bool has_children(node *node)
{
	for (int i = 0; i < NUM_CHARS; i++) {
		if (node->children[i] != NULL) {
			return true;
		}
	}

	return false;
}

static node *delete_dictionary_rec(node *node, unsigned char *text,
				   bool *deleted)
{
	if (!node) {
		return node;
	}

	if (!*text) {
		if (node->end_node) {
			node->end_node = false;
			*deleted = true;

			if (!has_children(node)) {
				free(node);
				node = NULL;
			}
		}
		return node;
	}

	node->children[text[0]] = delete_dictionary_rec(node->children[text[0]],
							text + 1, deleted);
	if (*deleted && !has_children(node) && !node->end_node) {
		free(node);
		node = NULL;
	}
	return node;
}

bool node_delete(node **root, const char *signed_text)
{
	if (!*root || !signed_text) {
		return false;
	}

	unsigned char *text = (unsigned char *)signed_text;
	bool result = false;

	*root = delete_dictionary_rec(*root, text, &result);
	return result;
}

bool trie_delete(node **root)
{
	if (!root) {
		return false;
	}

	node *ptr = *root;
	int i;

	// recursive case (go to end of trie)
	for (i = 0; i < NUM_CHARS; i++) {
		if (ptr->children[i] != NULL) {
			trie_delete(&ptr->children[i]);
		}
	}

	// base case
	free(ptr);

	return true;
}