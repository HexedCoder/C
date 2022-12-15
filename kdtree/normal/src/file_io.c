/** @file file_io.c
*
* @brief This module implements the functions in file_io.h
* @par
* COPYRIGHT NOTICE: (c) 2022 Jacob Hitchcox
*/

#include "file_io.h"

FILE *read_file(const char *file_name)
{
	if (!file_name) {
		return NULL;
	}

	struct stat buf;
	stat(file_name, &buf);

	if (S_ISDIR(buf.st_mode)) {
		printf("Directory provided\n");
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
