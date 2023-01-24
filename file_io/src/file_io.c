/** @file file_io.c
*
* @brief This module implements the functions in file_io.h
* @par
* COPYRIGHT NOTICE: (c) 2022 Jacob Hitchcox
*/

#include "file_io.h"

#include <getopt.h>

int get_args(int argc, char *argv[])
{
	if (!argc || !argv) {
		return 0;
	}

	int opt;
	char *file_name = NULL;
	const char *msg = NULL;

	// Long option implementation adapted from Mead's Guide
	// https://azrael.digipen.edu/~mmead/www/Courses/CS180/getopt.html
	int option_index = 0;
	static struct option long_options[] = {
		{ "prefix", required_argument, NULL, 'p' },
		{ "search", required_argument, NULL, 's' },
		{ "file", required_argument, NULL, 'f' },
		{ "help", no_argument, NULL, 'h' },
	};
	while ((opt = getopt_long(argc, argv, "d:f:p:s:h", long_options,
				  &option_index)) != -1) {
		switch (opt) {
		case 'd':
			break;
		case 'f':
			file_name = optarg;
			break;
		case 'p':
			break;
		case 's':
			break;
		case 'h':
		default: // intentional fall-through
			msg = "Max line size: 255 characters\n"
			      "Mandatory Arg:\n"
			      "\t-f | --file <arg>: file name to read for inputs\n"
			      "Arg Options (must use one):\n"
			      "\t-s | --search <arg>: find exact match for arg in radix\n"
			      "\t-p | --prefix <arg>: arg is word prefix to search to print\n"
			      "\t-d | --delete <arg>: delete arg from list (not implemented)\n";

			printf("%s", msg);
			return 1;
		}
	}

	if (!*file_name) {
		msg = "File name required\n"
		      "\t-f <arg>: file name to read for inputs\n";
		printf("%s", msg);

		return 1;
	}

	return 0;
} /* get_args() */

FILE *read_file(const char *file_name)
{
	if (!file_name) {
		return NULL;
	}

	struct stat buf;
	stat(file_name, &buf);

	// check for directory argument
	if (S_ISDIR(buf.st_mode)) {
		printf("Directory provided\n");
		return NULL;
	}

	FILE *file = fopen(file_name, "r");

	if (!file) {
		perror("Fatal - Unable to open");
	}

	// ensure file is not empty
	if (!get_length(file)) {
		fclose(file);
		printf("Empty file input\n");
		file = NULL;
	}

	return file;
} /* read_file() */

long get_length(FILE *file)
{
	if (!file) {
		printf("Invalid file input\n");
		return -1;
	}

	// save location of file pointer
	long curr_location = ftell(file);
	fseek(file, 0, SEEK_END);
	long length = ftell(file);

	// reset file pointer location
	fseek(file, curr_location, SEEK_SET);

	return length;
} /* get_length() */
