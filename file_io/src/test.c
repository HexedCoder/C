#include "file_io.h"

int main(int argc, char *argv[])
{
	FILE *file;

	if (argc != 2) {
		exit(1);
	}

	char *file_name = argv[1];
	long length;

	file = read_file(file_name);
	if (!file)
		exit(1);

	length = get_length(file);
	if (-1 == length) {
		exit(1);
	}

	printf("File length: %ld\n", length);
}
