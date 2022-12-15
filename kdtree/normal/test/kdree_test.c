/** @file test.c
*
* @brief test.c tests public functions from file_io.h
*
* COPYRIGHT NOTICE: (c) 2022 Jacob Hitchcox
*
*/

#include "../src/file_io.h"
#include <check.h>

#define MAX_LINE_LENGTH 1024

START_TEST(test_valid_file_inputs)
{
	char valid_string[] = "test/sample_valid_input";

	FILE *file = read_file(valid_string);

	int ret = get_length(file);
	fclose(file);
	printf("ret: %d\n", ret);

	// file size is 19
	ck_assert(ret == 19);

	char valid_string2[] = "test/sample_valid_input2";
	file = read_file(valid_string2);
	ret = get_length(file);
	fclose(file);

	// file size is 23
	ck_assert(ret == 23);
}

END_TEST START_TEST(test_invalid_file_inputs)
{
	char invalid_string[] = "test/sample_empty_input";

	// returns NULL due to being an empty file
	ck_assert(read_file(invalid_string) == NULL);

	char invalid_string2[] = "/dev/null";

	// returns NULL due to being an empty file
	ck_assert(read_file(invalid_string2) == NULL);

	char invalid_string3[] = "/dev/urandom";

	// returns NULL due to being an empty file
	ck_assert(read_file(invalid_string3) == NULL);

	char invalid_string4[] = "/dev/zero";

	// returns NULL due to being an empty file
	ck_assert(read_file(invalid_string4) == NULL);

	char invalid_string5[] = "/src";

	// returns NULL due to being a directory
	ck_assert(read_file(invalid_string5) == NULL);

	char invalid_string6[] = "";
	ck_assert(read_file(invalid_string6) == NULL);
}

END_TEST Suite *file_io_check(void)
{
	Suite *suite;
	TCase *tc_core;

	suite = suite_create("file_io_tests");

	tc_core = tcase_create("Core");
	tcase_add_test(tc_core, test_valid_file_inputs);
	tcase_add_test(tc_core, test_invalid_file_inputs);

	suite_add_tcase(suite, tc_core);

	return suite;
}

int main()
{
	Suite *suite = file_io_check();
	SRunner *runner = srunner_create(suite);

	srunner_set_fork_status(runner, CK_NOFORK);

	srunner_run_all(runner, CK_VERBOSE);

	int no_failed = srunner_ntests_failed(runner);

	srunner_free(runner);
	return (no_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

/*** end of file***/
