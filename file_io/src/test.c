#include "file_io.h"

int main(int argc, char *argv[])
{

	FILE *file;
	char *file_name = argv[1];
	char *method = "r";
	long length;

	file = open_file(file_name, method);
	if (!file)
		return 1;

	fseek(file, 2, SEEK_SET);
	printf("%ld\n", ftell(file));
	length = get_length(NULL);
	printf("%ld\n", ftell(file));

	printf("%ld\n", length);
}
