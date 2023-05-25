#include <stdio.h>
#include <stdlib.h>

#include "hashtable.h"

#define movie_list "Movies.txt"

// https://benhoyt.com/writings/hash-table-in-c/

void print_person(void *data);
void destroy_person(void *data);

void print_movie(void *data);
void destroy_movie(void *data);

typedef struct person_t {
	int age;
	char *name;

} person_t;

typedef struct movie_t {
	char *title;
	int year;
} movie_t;

int main(void)
{
	hashtable_t *hashtable = create_table(print_movie, destroy_movie);
	if (!hashtable) {
		printf("Unable to create hashtable\n");
		exit(EXIT_FAILURE);
	}

	FILE *fp = fopen(movie_list, "r");
	// TODO error handling

	char line_buff[256] = { 0 };

	while (fgets(line_buff, 255, fp) != NULL) {

		line_buff[strcspn(line_buff, "\n") - 1] = 0;

		movie_t *mov = calloc(1, sizeof(movie_t));

		if (!mov) {
			continue ;
		}

		char * year_ptr = strrchr(line_buff, ' ');

		if (!year_ptr) {
//			fprintf(stderr, "Skipped: %s\n", line_buff);
			free(mov);
			continue;
		}
		*year_ptr = 0;
		mov->year = atoi(year_ptr + 1);
		mov->title = strdup(line_buff);
		*year_ptr = ' ';

		hashtable_insert(hashtable, line_buff, mov);
	}

	fclose(fp);
//	hashtable_display(hashtable);
	movie_t *mov = hashtable_lookup(hashtable, "Skippy 1931");
	if (mov) {
		printf("\n%s: ", mov->title);
		printf("%d\n\n", mov->year);
	}
	hashtable_destroy(&hashtable);
	return 0;
}

void print_person(void *data)
{
	if (!data) {
		return;
	}

	person_t *p = (person_t *) data;
	printf("%s is %d\n", p->name, p->age);
}

void destroy_person(void *data)
{
	person_t *p = (person_t *) data;
	free(p->name);
	free(p);
	p = NULL;
}

void print_movie(void *data)
{
	if (!data) {
		return;
	}

	movie_t *p = (movie_t *) data;
	printf("%s: ", p->title);
	printf("%d\n", p->year);
}

void destroy_movie(void *data)
{
	movie_t *p = (movie_t *) data;
	free(p->title);
	free(p);
	p = NULL;
}
