/** @file file_io.h
 *
 * @brief This module serves as the template for functions using io commands
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2022 Jacob Hitchcox
 */
#ifndef C_CODES_FILE_IO_H
#define C_CODES_FILE_IO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * @brief Check cmd line args for program
 *
 * @param argc The number of args passed in
 * @param argv The args passed in to the program
 *
 * @return file_name on success, NULL on fail
 */
bool get_args(int argc, char *argv[]);

/**
 * @brief Open the file_name provided in the method provided
 *
 * @param file_name The name of the file which is opened
 * @param method The method used when opening the file
 *
 * @return FILE * pointer to file on success, NULL on fail
 */
long get_length(FILE *file_name);

/**
 * @brief Get length of input file. The function returns the file pointer to
 * the original location
 *
 * @param file_name File to get the length
 *
 * @return long length of file on success, -1 on fail
 */
FILE *open_file(char *file_name, char *method);

#endif // C_CODES_FILE_IO_H