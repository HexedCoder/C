/** @file pretty_print_tree.c
*
* @brief This module defines the functions in pretty_print_tree.h
*
* @par
* COPYRIGHT NOTICE: (c) 2022 Jacob Hitchcox
*/

#include "file_io.h"
#include "stdio.h"

struct trunk {
	struct trunk *prev;
	const char *str;
};

void print_trunks(struct trunk *p)
{
	if (!p) {
		return;
	}
	print_trunks(p->prev);
	printf("%s", p->str);
} /* print_trunks() */

void print_recursive(node_t *root, struct trunk *prev, int is_left)
{
	if (!root) {
		return;
	}

	struct trunk this_disp = { prev, "     " };
	const char *prev_str = this_disp.str;
	print_recursive(root->right, &this_disp, 1);

	if (!prev) {
		this_disp.str = "---";
	} else if (is_left) {
		this_disp.str = ".--";
		prev_str = "    |";
	} else {
		this_disp.str = "`--";
		prev->str = prev_str;
	}

	movie_t *movie = root->data;
	print_trunks(&this_disp);
	printf("%s (%d)\n", movie->title,
	       movie->year); // whatever custom print you need

	if (prev) {
		prev->str = prev_str;
	}
	this_disp.str = "    |";

	print_recursive(root->left, &this_disp, 0);
	if (!prev) {
		puts("");
	}
} /* print_recursive() */

void print_visual(avl_t *root)
{
	if (!root) {
		return;
	}
	print_recursive(root->root, NULL, 0);
} /* print_visual() */

/*** END OF FILE ***/
