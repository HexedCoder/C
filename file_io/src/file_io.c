/** @file file_io.c
*
* @brief This module implements the functions in file_io.h
* @par
* COPYRIGHT NOTICE: (c) 2023 Jacob Hitchcox
*/
#define _GNU_SOURCE

#include "file_io.h"

#include <getopt.h>
#include <errno.h>
#include <signal.h>

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
		{"help", no_argument, NULL, 'h'},
		{0, 0, 0, 0}

	};
	while ((opt = getopt_long(argc, argv, "h", long_options,
				  &option_index)) != -1) {
		switch (opt) {
		case 'h':
			break;
		default:
			msg = "Modify\n";
			printf("%s", msg);
			return 1;
		}
	}

	if (!*file_name) {
		msg = "File name required\n\t"
		    "-f <arg>: file name to read for inputs\n";
		printf("%s", msg);

		return 1;
	}

	return 0;
}				/* get_args() */

FILE *read_file(const char *file_name)
{
	if (!file_name) {
		return NULL;
	}

	struct stat buf;
	stat(file_name, &buf);

	// check for directory argument
	if (S_ISDIR(buf.st_mode)) {
		fprintf(stderr, "Directory provided\n");
		return NULL;
	}

	FILE *file = fopen(file_name, "r");

	if (!file) {
		perror("Fatal - Unable to open");
	}
	// ensure file is not empty
	if (!f_get_length(file)) {
		fclose(file);
		fprintf(stderr, "Empty file input\n");
		file = NULL;
	}

	return file;
}				/* read_file() */

long f_get_length(FILE * file)
{
	if (!file) {
		fprintf(stderr, "Invalid file input\n");
		return -1;
	}

	struct stat st_struct;
	if (fstat(fileno(file), &st_struct) != 0) {
		perror("Failed to get file status");
		return -1;
	}

	return st_struct.st_size;

}				/* f_get_length() */

long fd_get_length(int fd)
{
	if (!fd) {
		fprintf(stderr, "Invalid file descriptor provided.\n");
		return -1;
	}

	struct stat st_struct;
	if (fstat(fd, &st_struct) != 0) {
		perror("Failed to get file status");
		return -1;
	}

	return st_struct.st_size;

}				/* fd_get_length() */

ssize_t get_input(char *input_buffer, const char *msg)
{
	char *current_dir = get_current_dir_name();
	printf("\033[1m%s\033[0m %s ", msg, current_dir);
	free(current_dir);

	char *ptr = fgets(input_buffer, 4096, stdin);

	if (!ptr || ' ' == ptr[0]) {
		fprintf(stderr, "Invalid\n");
		return 0;
	}

	if (feof(stdin) || ferror(stdin)) {
		puts("");
		kill(0, SIGINT);
	} else if (errno) {
		perror(" fgets");
		fprintf(stderr, "Error code: %d\n", errno);

		errno = 0;
		return -1;
	}

	strrchr(input_buffer, '\n')[0] = '\0';

	return (ssize_t) strlen(input_buffer);

}				/* get_input() */
