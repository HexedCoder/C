/** @file kdtree_driver.c
*
* @brief This module opens supplied file to build a radix of all valid strings
* found in file. The radix can be printed with -p with a prefix for all words
* to print. -d allows user to delete a word. -s matches a full word and
* prints whether the word is in the file
*
* @par
* COPYRIGHT NOTICE: (c) 2022 Jacob Hitchcox
*/

#include "kdtree_funcs.h"
#include "file_io.h"
#include "pqueue.h"

#include <string.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <getopt.h>
#include <float.h>
#include <math.h>

#define STARTING_CAP 20

struct trunk {
	struct trunk *prev;
	const char *str;
};

int find_median(FILE * file)
{
	if (!file) {
		return INT32_MIN;
	}
	double min = INT32_MAX;
	double max = INT32_MIN;

	long curr_location = ftell(file);
	char buffer[1024];

	while (fgets(buffer, 1023, file)) {
		char *fields[2];
		char *ptr = buffer;

		for (int i = 0; i < 2; i++) {
			// save the pointer to the start of the field
			fields[i] = ptr;
			// skip to the end of the field
			ptr += strcspn(ptr, "\t\n");
			if (*ptr != '\0') {
				// set a null terminator for the field and skip to the next one
				*ptr++ = '\0';
			}
		}

		double res = strtod(fields[0], NULL);

		if (res > max) {
			max = res;
		} else if (res < min) {
			min = res;
		}

		if (0 == (int)res) {
			printf("Invalid argument found: '%s'\n", buffer);
			fclose(file);
			exit(1);
		}
	}

	fseek(file, curr_location, SEEK_SET);

	return 2;
}				/* find_median() */

static void print_trunks(struct trunk *p)
{
	if (!p) {
		return;
	}
	print_trunks(p->prev);
	printf("%s", p->str);
}

static void print_recursive(tree * root, struct trunk *prev, int is_left)
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

	print_trunks(&this_disp);
	printf("(%lf, %lf) \n", root->x_coord, root->y_coord);	// whatever custom print you need

	if (prev) {
		prev->str = prev_str;
	}
	this_disp.str = "    |";

	print_recursive(root->left, &this_disp, 0);
	if (!prev) {
		puts("");
	}
}

void print_visual(tree * root)
{
	if (!root) {
		return;
	}
	print_recursive(root, NULL, 0);
}

void node_print(void *value)
{
	if (!value) {
		return;
	}

	tree *node = (tree *) value;

	printf("(%lf, %lf, %lf\n", node->x_coord, node->x_coord,
	       node->distance);
}

int main(int argc, char *argv[])
{
	int opt;
	double x_coord = DBL_MAX;
	double y_coord = DBL_MAX;
	char *broken = NULL;
	const char *file_name = "input";
	int num_neighbors = 1;

	// Long option implementation adapted from Mead's Guide
	// https://azrael.digipen.edu/~mmead/www/Courses/CS180/getopt.html
	int option_index = 0;
	static struct option long_options[] = {
		{"x_coord", required_argument, NULL, 'x'},
		{"y_coord", required_argument, NULL, 'y'},
		{"file", required_argument, NULL, 'f'},
		{"knn", required_argument, NULL, 'k'},
		{"help", required_argument, NULL, 'k'},
	};
	while ((opt = getopt_long(argc, argv, "x:y:f:k:h", long_options,
				  &option_index)) != -1) {
		switch (opt) {
		case 'x':
			x_coord = strtod(optarg, &broken);
			if (*broken) {
				printf("x: Invalid x_coord provided\n");
				exit(1);
			}
			break;
		case 'y':
			y_coord = strtod(optarg, &broken);
			if (*broken) {
				printf("y: Invalid y_coord provided\n");
				exit(1);
			}
			break;
		case 'f':
			file_name = optarg;
			break;
		case 'k':
			num_neighbors = strtod(optarg, &broken);
			if (*broken) {
				printf("K: Invalid integer provided\n");
				exit(1);
			}
			break;
		case 'h':
		default:
			printf
			    ("Usage ./driver -f input_file -x <x_coord> -y <y_coord>\n");
			printf
			    ("\t-f | --file <arg>: file to input (input, by default)\n");
			printf
			    ("\t-x | --x_coord <arg>: x-coord between -180 and 180)\n");
			printf
			    ("\t-y | --y_coord <arg>: y-coord between -180 and 180)\n");
			exit(1);
		}
	}

	if (fabs(x_coord) > 180 || fabs(y_coord) > 180) {
		printf("Mandatory Arg:\n");
		printf
		    ("\t-x | --x_coord <arg>: x-coord between -180 and 180)\n");
		printf
		    ("\t-y | --y_coord <arg>: y-coord between -180 and 180)\n");
		exit(1);
	}

	FILE *file = read_file(file_name);

	if (!file) {
		exit(1);
	}

	int line_no = 0;
	int count = 1;
	char buffer[1024];
	tree *bst = NULL;

	while (fgets(buffer, 1023, file)) {
		char *fields[2];
		char *ptr = buffer;

		for (int i = 0; i < 2; i++) {
			// save the pointer to the start of the field
			fields[i] = ptr;
			// skip to the end of the field
			ptr += strcspn(ptr, ",\n");
			if (*ptr != '\0') {
				// set a null terminator for the field and skip to the next one
				*ptr++ = '\0';
			}
		}

		double tmp_x_coord = strtod(fields[0], &broken);
		if (*broken || fields[0] == broken) {
			if (0 == line_no++) {
				continue;
			} else {
				printf("broken: %d\n", *broken);
				fprintf(stderr, "%s broke me\n", buffer);
				fclose(file);
				tree_delete(&bst);
				exit(1);
			}
		}
		double tmp_y_coord = strtod(fields[1], &broken);

		if (*broken || fields[1] == broken) {
			printf("broken: %d\n", *broken);
			fprintf(stderr, "%s broke me\n", buffer);
			fclose(file);
			tree_delete(&bst);
			exit(1);
		}

		if (fabs(tmp_x_coord) > 180 || fabs(tmp_y_coord) > 180) {
			printf("Coordinate must be between -180 and 180 "
			       "degrees\n");
			fclose(file);
			tree_delete(&bst);
			exit(1);
		}

		if (1 == count++) {
			bst = create_tree_node(tmp_x_coord, tmp_y_coord);

			if (!bst) {
				exit(1);
			}
		} else {
			int ret = kd_insert(bst,
					    create_tree_node(tmp_x_coord,
							     tmp_y_coord),
					    0);
			if (!ret) {
				fprintf(stderr, "Unable to insert into tree\n");
				fclose(file);
				tree_delete(&bst);
				exit(1);
			}
		}
	}

	pqueue_t *queue = pqueue_create(100, NULL);

	if (!queue) {
		fprintf(stderr, "Unable to allocate memory\n");
		exit(1);
	}

	nearest_neighbor(bst, x_coord, y_coord, 1, 0, queue);

	for (int i = 0; i < num_neighbors; ++i) {
		if (!pqueue_is_empty(queue)) {
			pqueue_print(queue);
			pqueue_extract(queue);
		} else {
			break;
		}
	}

	fclose(file);

	tree_delete(&bst);
	pqueue_delete(queue);
}
