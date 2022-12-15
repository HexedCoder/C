/** @file test.c
*
* @brief test.c tests public functions from file_io.h
*
* COPYRIGHT NOTICE: (c) 2022 Jacob Hitchcox
*
*/

#include "../src/file_io.h"
#include "../src/kdtree_funcs.h"
#include "../src/pqueue.h"
#include <check.h>

#define MAX_LINE_LENGTH 1024

int get_data(FILE *file)
{
	int line_no = 0;
	int count = 1;
	char buffer[1024];
	char *broken = NULL;
	tree *bst = NULL;

	while (fgets(buffer, 1023, file)) {
		char *fields[2];
		char *ptr = buffer;

		for (int i = 0; i < 2; i++) {
			// save the pointer to the start of the field
			fields[i] = ptr;
			// skip to the end of the field
			ptr += strcspn(ptr, ",\n");
			if (*ptr != '\0') {
				// set a null terminator for the field and skip to the next one
				*ptr++ = '\0';
			}
		}

		double tmp_x_coord = strtod(fields[0], &broken);
		if (*broken || fields[0] == broken) {
			if (0 == line_no++) {
				continue;
			} else {
				fprintf(stderr, "%s broke me\n", buffer);
				fclose(file);
				tree_delete(&bst);
				return 1;
			}
		}
		double tmp_y_coord = strtod(fields[1], &broken);

		if (*broken || fields[1] == broken) {
			fprintf(stderr, "%s broke me\n", buffer);
			fclose(file);
			tree_delete(&bst);
			return 2;
		}

		if (fabs(tmp_x_coord) > 180 || fabs(tmp_y_coord) > 180) {
			printf("Coordinate must be between -180 and 180 "
			       "degrees\n");
			fclose(file);
			tree_delete(&bst);
			return 3;
		}

		if (1 == count++) {
			bst = create_tree_node(tmp_x_coord, tmp_y_coord);

			if (!bst) {
				return 1;
			}
		} else {
			int ret = kd_insert(
				bst, create_tree_node(tmp_x_coord, tmp_y_coord),
				0);
			if (!ret) {
				fprintf(stderr, "Unable to insert into tree\n");
				fclose(file);
				tree_delete(&bst);
				return 1;
			}
		}
	}

	return 0;
}

START_TEST(test_valid_kdtree_ops)
{
	char valid_string[] = "test/sample_valid_input";

	FILE *file = read_file(valid_string);

	int ret = get_length(file);
	fclose(file);
	printf("ret: %d\n", ret);

	// file size is 1005148
	ck_assert(ret == 1005148);

	char valid_string2[] = "test/sample_valid_input2";
	file = read_file(valid_string2);
	ret = get_length(file);
	fclose(file);
	printf("ret: %d\n", ret);

	// file size is 349
	ck_assert(ret == 349);

	// points are 3, 4, 5 length triangle
	ck_assert(get_distance(0, 0, 3, 4) == 5);

	// points are the same
	ck_assert(get_distance(0, 0, 0, 0) == 0);
}

END_TEST START_TEST(test_invalid_kdtree_ops)
{
	int ret = 0;
	const char *empty_file_string = "test/sample_empty_input";
	ck_assert(read_file(empty_file_string) == NULL);

	const char *invalid_string = "test/sample_invalid_input";
	FILE *file = read_file(invalid_string);
	ret = get_data(file);

	// Invalid x_coord input
	ck_assert(ret == 1);

	const char *invalid_string2 = "test/sample_invalid_input2";
	file = read_file(invalid_string2);
	ret = get_data(file);

	// Invalid x_coord input
	ck_assert(ret == 3);

	const char *invalid_string3 = "test/sample_invalid_input3";
	file = read_file(invalid_string3);
	ret = get_data(file);

	// Invalid x_coord input
	ck_assert(ret == 3);

	const char *invalid_string4 = "test/sample_invalid_input4";
	file = read_file(invalid_string4);
	ret = get_data(file);

	// Invalid y_coord input
	ck_assert(ret == 3);

	const char *invalid_string5 = "test/sample_invalid_input5";
	file = read_file(invalid_string5);
	ret = get_data(file);

	// Invalid y_coord input
	ck_assert(ret == 3);

	// Null tree input
	ck_assert(tree_size(NULL) == 0);
}

END_TEST Suite *kdtree_check(void)
{
	Suite *suite;
	TCase *tc_core;

	suite = suite_create("kdtree_tests");

	tc_core = tcase_create("Core");
	tcase_add_test(tc_core, test_valid_kdtree_ops);
	tcase_add_test(tc_core, test_invalid_kdtree_ops);

	suite_add_tcase(suite, tc_core);

	return suite;
}

int main()
{
	Suite *suite = kdtree_check();
	SRunner *runner = srunner_create(suite);

	srunner_set_fork_status(runner, CK_NOFORK);

	srunner_run_all(runner, CK_VERBOSE);

	int no_failed = srunner_ntests_failed(runner);

	srunner_free(runner);
	return (no_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

/*** end of file***/
