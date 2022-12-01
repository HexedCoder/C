#include "file_io.h"

#include <getopt.h>

bool get_args(int argc, char *argv[])
{
	int opt;
	bool display_holes = false;

	while ((opt = getopt(argc, argv, "c")) != -1) {
		switch (opt) {
		case 'c':
			printf("Displaying all holes\n");
			display_holes = true;
		}
	}

	return display_holes;
}

FILE *open_file(char *file_name, char *method)
{
	FILE *file;

	file = fopen(file_name, method);

	if (!file) {
		char diag[64];
		sprintf(diag, "Fatal - Unable to open '%s'", file_name);
		perror(diag);
		return file;
	}

	return file;
}

long get_length(FILE *file)
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