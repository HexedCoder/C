/** @file bfs.c
*
* @brief Implements a breadth first search
*
* @par
* COPYRIGHT NOTICE: (c) 2022 Jacob Hitchcox
*/

#include "file_io.h"
#define MAX_LINE_LENGTH 1024

int get_rows_cols(FILE *file, int *rows, int *cols)
{
	int row = 0;
	int col = 0;
	char curr = '\0';
	while ((curr = fgetc(file)) != EOF) {
		switch (curr) {
		case '\n':
			++row;
		default:
			++col;
		}
	}
	col = (col - row) / (row + 1);
	++row;

	fseek(file, 0, SEEK_SET);

	if (col && row) {
		*rows = row;
		*cols = col;
		return 0;
	}

	return 1;
}

int bfs(char **matrix, char *start, char *end, int start_row, int start_col)
{
	if (!*matrix || !start || !end) {
		return 1;
	}
	printf("%d", start_col);
	printf("%d", start_row);

	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 8; ++j) {
			printf("%c", matrix[i][j]);
		}
		printf("\n");
	}

	return 0;
}

int matrix_destroy(char **matrix, int rows)
{
	if (!*matrix) {
		return 1;
	}

	for (int i = 0; i < rows; ++i) {
		free(matrix[i]);
	}
	free(matrix);

	return 0;
}

int populate_matrix(FILE *file, char **matrix, char **start, char **end,
		    int *start_row, int *start_col)
{
	if (!file || !*matrix) {
		return 1;
	}

	char curr = '\0';
	int col = 0;
	int row = 0;

	while ((curr = fgetc(file)) != EOF) {
		if (curr != '\n') {
			matrix[row][col] = curr;
			if ('S' == curr) {
				*start = &matrix[row][col];
				*start_col = col;
				*start_row = row;
			} else if ('E' == curr) {
				*end = &matrix[row][col];
			}
			col++;
		} else {
			++row;
			col = 0;
		}
	}

	return 0;
}

int main(void)
{
	const char *file_name = "input";
	int start_col = 0;
	int start_row = 0;

	int cols = 0;
	int rows = 0;

	// reads file and returns NULL if unable to find or the file is empty
	FILE *file = read_file(file_name);

	if (!file) {
		exit(1);
	}

	int ret = get_rows_cols(file, &rows, &cols);

	if (ret) {
		exit(1);
	}

	char **matrix = calloc(rows * cols, sizeof(char));
	for (int i = 0; i < rows; ++i) {
		matrix[i] = calloc(cols + 1, sizeof(char));
		if (!matrix[i]) {
			exit(1);
		}
	}
	char *start = 0;
	char *end = 0;

	ret = populate_matrix(file, matrix, &start, &end, &start_row,
			      &start_col);
	if (ret) {
		exit(1);
	}

	ret = bfs(matrix, start, end, start_row, start_col);

	matrix_destroy(matrix, rows);
	fclose(file);
}
