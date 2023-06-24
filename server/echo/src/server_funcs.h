#ifndef TIMED_FUNCS_H
#define TIMED_FUNCS_H

#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>

#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>

#define MAX_CLIENTS 5
#define BUFF_LEN 1024

typedef struct client_struct {
	int client_fd;
} client_struct_t;

extern pthread_t tcp_thread;
extern pthread_t udp_thread;

/**
 * @brief Handles signals sent to the process by invoking the appropriate
 * 		  signal handler function for each signal.
 *
 * @param sig The signal number that triggered the function. This parameter
 *            is unused, but required for the signal handler function.
 */
void main_signal_handler(int sig);

/**
 * @brief This function checks if a given port number is valid, ensuring that it falls within the
 * valid port range (1-65535).
 *
 * @param user_port The string to parse for port
 * @param port ptr to store the result
 *
 * @return true if the port number is valid, false otherwise.
 */
bool valid_port(const char *user_port, int *port);

/**
 * @brief Handles SIGINT signal (CTRL + C) to interrupt program.
 *
 * This function prints a message to console indicating the program was
 * interrupted by the user.
 *
 * @param sig The signal number that triggered the function. This parameter
 *            is unused, but required for the signal handler function.
 */
void main_set_shutdown(int);

/**
 * @brief TCP handler function that sends the current time to the client.
 * This function is responsible for handling TCP connections from clients. It accepts client connections,
 * and echoes client messages to the client.
 *
 * @param arg A void pointer to the client_struct_t argument containing the client file descriptor.
 *
 * @return arg on success, otherwise NULL
 */
void *tcp_handler(void *arg);

/**
 * @brief UDP handler function for processing UDP client requests.
 * This file contains the implementation of the UDP handler function, which is responsible for
 * processing UDP client requests and echoing client messages to the client.
 *
 * @param arg A void pointer to the client_struct_t argument containing the client file descriptor.
 *
 * @return arg on success, otherwise NULL
 */
void *udp_handler(void *arg);

#endif				/* TIMED_FUNCS_H */

/* end of file */
