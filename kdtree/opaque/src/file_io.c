/** @file file_io.c
*
* @brief This module implements the functions in file_io.h
* @par
* COPYRIGHT NOTICE: (c) 2022 Jacob Hitchcox
*/

#include "file_io.h"

#include <getopt.h>

bool get_args(int argc, char *argv[], char **delete, char **file_name,
	      char **prefix, char **search)
{
	if (!argc || !argv || !file_name) {
		return false;
	}

	int opt;

	// Long option implementation adapted from Mead's Guide
	// https://azrael.digipen.edu/~mmead/www/Courses/CS180/getopt.html
	int option_index = 0;
	static struct option long_options[] = {
		{"prefix", required_argument, NULL, 'p'},
		{"search", required_argument, NULL, 's'},
		{"file", required_argument, NULL, 'f'},
		{"help", no_argument, NULL, 'h'},
	};
	while ((opt = getopt_long(argc, argv, "d:f:p:s:h", long_options,
				  &option_index)) != -1) {
		switch (opt) {
		case 'd':
			*delete = optarg;
			break;
		case 'f':
			*file_name = optarg;
			break;
		case 'p':
			*prefix = optarg;
			break;
		case 's':
			*search = optarg;
			break;
		case 'h':
		default:	// intentional fall-through
			printf("Max line size: 255 characters\n");
			printf("Mandatory Arg:\n");
			printf("\t-f <arg>: file name to read for inputs\n");
			printf("Arg Options (must use one):\n");
			printf("\t-s <arg>: find exact match for arg in "
			       "radix\n");
			printf("\t-p <arg>: word prefix to search to print\n");
			printf("\t-d <arg>: delete arg from list (not "
			       "implemented)\n");
			return true;
		}
	}

	if (!*file_name) {
		printf("File name required\n");
		printf("\t-f <arg>: file name to read for inputs\n");
		return true;
	} else if (*delete && !*prefix && !*search) {
		printf("Delete requires either a search or prefix\n");
		printf("\t-p <arg>: word prefix to search to print\n");
		printf("\t-s <arg>: find exact match for arg in radix\n");
		return true;
	} else if ((*prefix && *search) || (!*prefix && !*search)) {
		printf("Must use either a prefix lookup or search\n");
		printf("\t-p <arg>: arg is word prefix to search to print\n");
		printf("\t-s <arg>: find exact arg for arg in radix\n");
		return true;
	}

	return false;
}

FILE *read_file(char *file_name)
{
	if (!file_name) {
		return NULL;
	}

	FILE *file = fopen(file_name, "r");

	if (!file) {
		perror("Fatal - Unable to open");
	}

	if (!get_length(file)) {
		fclose(file);
		printf("Empty file input\n");
		file = NULL;
	}

	return file;
}

long get_length(FILE * file)
{
	if (!file) {
		printf("Invalid file input\n");
		return -1;
	}

	long curr_location = ftell(file);
	fseek(file, 0, SEEK_END);
	long length = ftell(file);

	fseek(file, curr_location, SEEK_SET);

	return length;
}
