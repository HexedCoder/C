/** @file file_io_check.c
 *
 * @brief file_io_check.c tests public functions from file_io.h
 *
 * COPYRIGHT NOTICE: (c) 2022 Jacob Hitchcox
 *
 */

#ifndef FILE_IO_CHECK_H
#define FILE_IO_CHECK_H

#include <check.h>
#include "../src/file_io.h"

START_TEST(test_valid_file_inputs)
        {
        printf ("\n-----FILE_IO----\n");

        printf ("Start: test_valid_file_inputs\n");

        char valid_string[] = "test/sample_valid_input";

        FILE *file = read_file(valid_string);
        long ret = get_length(file);
        fclose(file);

        // file size is 654
        ck_assert(ret == 654);

        // Empty file input should display
        char valid_string2[] = "test/sample_valid_input2";
        file = read_file(valid_string2);
        ret = get_length(file);

        fclose(file);

        // file size is 1005148
        ck_assert(ret == 1005148);
        printf ("End: test_valid_file_inputs\n");

        }

END_TEST START_TEST(test_invalid_file_inputs)
{
    printf ("Start: test_invalid_file_inputs\n");

    char invalid_string[] = "test/sample_empty_input";

    // returns NULL due to being an empty file
    ck_assert(read_file(invalid_string) == NULL);
    char invalid_string2[] = "test/sample_invalid_input";

    FILE *file = read_file(invalid_string2);
    char line[MAX_LINE_LENGTH];
    fgets(line, MAX_LINE_LENGTH - 1, file);
    fclose(file);
    printf ("End: test_invalid_file_inputs\n");

    printf ("-----END FILE_IO----\n\n");
}

END_TEST

#endif //FILE_IO_CHECK_H