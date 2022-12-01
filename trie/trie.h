/** @file trie.h
*
* @brief This module provides declarations for trie.c
 * Adopted by Jacob Sorber https://www.youtube.com/watch?v=NDfAYZCHstI
*
* @par
* COPYRIGHT NOTICE: (c) 2022 Jacob Hitchcox
*/

#ifndef TRIE_H
#define TRIE_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define NUM_CHARS 256 // enough to house string

typedef struct node {
	struct node *children[NUM_CHARS]; // array for children
	bool end_node; // checks if it's the end of a word
} node;

node *create_node();

bool insert_node(node **root, char *string_to_insert);

void print_dictionary_recursive(node *node, unsigned char *prefix, int length);

void print_dictionary(node *root);

#endif /* TRIE_H */