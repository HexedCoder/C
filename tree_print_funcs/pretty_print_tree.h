/** @file pretty_print_tree.h
*
* @brief This module declares the functions for pretty_print_tree.c
*
* @par
* COPYRIGHT NOTICE: (c) 2022 Jacob Hitchcox
*/

#ifndef PRETTY_PRINT_TREE_H
#define PRETTY_PRINT_TREE_H

#include "avl.h"

struct trunk;

void print_trunks(struct trunk *p);

void print_recursive(node_t * root, struct trunk *prev, int is_left);

void print_visual(avl_t * root);

#endif				//PRETTY_PRINT_TREE_H
