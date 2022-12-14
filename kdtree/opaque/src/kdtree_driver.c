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

#include "kdtree.h"
#include "file_io.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <getopt.h>
#include <float.h>
#include <math.h>

typedef double (*compare)(void *, void *, int);

typedef void (*action)(void *);

typedef void (*destroy)(void *);

void coord_action(void *data)
{
	coordinate *coord = (coordinate *) data;
	printf("(%lf), ", coord->x_coord);
	printf("(%lf)\n", coord->y_coord);
}

void destroy_action(void *data)
{
	coordinate *coord = (coordinate *) data;
}

double coord_compare(void *coordinate_1, void *coordinate_2, int method)
{
	coordinate *tmp_1 = (coordinate *) coordinate_1;
	coordinate *tmp_2 = (coordinate *) coordinate_2;

	double coord_1 = 0;
	double coord_2 = 0;

	if (0 == method) {
		coord_1 = tmp_1->x_coord;
		coord_2 = tmp_2->x_coord;
	} else {
		coord_1 = tmp_1->y_coord;
		coord_2 = tmp_2->y_coord;
	}

	return coord_2 - coord_1;
}

int main(int argc, char *argv[])
{
	int opt;
	double x_coord = 0;
	double y_coord = 0;
	char *broken = NULL;

	// Long option implementation adapted from Mead's Guide
	// https://azrael.digipen.edu/~mmead/www/Courses/CS180/getopt.html
	int option_index = 0;
	static struct option long_options[] = {
		{"x_coord", required_argument, NULL, 'x'},
		{"y_coord", required_argument, NULL, 'y'},
	};
	while ((opt = getopt_long(argc, argv, "x:y:", long_options,
				  &option_index)) != -1) {
		switch (opt) {
		case 'x':
			x_coord = strtod(optarg, &broken);
			if (*broken) {
				exit(1);
			}
			break;
		case 'y':
			y_coord = strtod(optarg, &broken);;
			if (*broken) {
				exit(1);
			}
			break;
		default:
			printf("Unknown operator found\n");
			exit(1);
		}
	}

	if (fabs(x_coord) < DBL_EPSILON || fabs(y_coord) < DBL_EPSILON) {
		printf("Mandatory Arg:\n");
		printf("\t-x <arg>: x-coord double not 0\n");
		printf("\t-y <arg>: y-coord double not 0\n");
		exit(1);
	}

	char *file_name = "input";
	FILE *file = read_file(file_name);

	tree *tree = tree_create(coord_compare, coord_action, free);
	int ret;
	int count = 1;
	char buffer[1024];

	while (fgets(buffer, 1023, file)) {
		coordinate *tmp = calloc(1, sizeof(coordinate));

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

		tmp->x_coord = strtod(fields[0], &broken);
		if (*broken) {
			printf("broken: %d\n", *broken);
			fprintf(stderr, "%s broke me\n", buffer);
			exit(1);
		}
		tmp->y_coord = strtod(fields[1], &broken);

		if (*broken) {
			printf("broken: %d\n", *broken);
			fprintf(stderr, "%s broke me\n", buffer);
			exit(1);
		}

		int median = find_median(file);
		ret = tree_insert(tree, tmp, median);
		if (!ret) {
			free(tmp);
			exit(1);
		}
	}
	fclose(file);

	coordinate *neighbor = tree_search(tree, x_coord, y_coord);
	coordinate *neighbor = find_neighbor(x_coord, y_coord);

	print_visual(tree);
	tree_destroy(&tree);
}
