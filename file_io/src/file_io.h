/** @file file_io.h
*
* @brief This module serves as the template for functions using io commands
*
* @par
* COPYRIGHT NOTICE: (c) 2023 Jacob Hitchcox
*/
#ifndef C_CODES_FILE_IO_H
#define C_CODES_FILE_IO_H

#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

/**
* @brief Parse command-line arguments and options and return
* the status of the parsing.
*
* @param argc: The number of arguments passed from the command line.
* @param argv: An array of strings containing the arguments passed from the
* command line.
*
* @return 1 on success, 0 on failure.
*/
int get_args(int argc, char *argv[]);

/**
* @brief Returns the size of a file in bytes.
*
* @param file_name: pointer to the file to get the size of.
*
* @return The size of the file in bytes if successful, or -1 on error. If an
* error occurs, an error message is printed to stderr.
*/
long f_get_length(FILE *file_name);

/**
* @brief Returns the size of a file associated with file_descriptor in bytes.
*
* @param fd: file descriptor of the file whose size is being determined.
*
* @return The number of bytes in the file provided or -1 if the provided
* file descriptor is invalid.
*/
long fd_get_length(int fd);

/**
* @brief Reads file_name and returns a pointer to a FILE object representing
* the opened file. If the file cannot be opened, NULL is returned.

*
* @param file_name: The name of the file to open.
*
* @return pointer to the opened file, or NULL if the file could not be opened.
*/
FILE *read_file(const char *file_name);

/**
* @brief  Read user input from the standard input and store it in the
 * provided input buffer.
*
* @param input_buffer: buffer to store the user input.
* @param msg: display message to give the user.
*
* @return The number of characters read from the stdin, or -1 on error
*/
ssize_t get_input(char *input_buffer, const char *msg);

#endif // C_CODES_FILE_IO_H
