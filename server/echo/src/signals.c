/** @file signals.h
*
* @brief This module implements the functions in signals.h
*
* @par
* COPYRIGHT NOTICE: (c) 2023 Jacob Hitchcox
*/

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "signals.h"

int establish_handler(int signum, handler_func func)
{
	struct sigaction sa = {
		.sa_handler = func,
		.sa_flags = 0,
	};

	int result = 0;

	if (-1 == sigaction(signum, &sa, NULL)) {
		perror("sigaction");
		errno = 0;
		result = 1;
	}

	return result;
}				/* establish_handler */

int block_signals(int num_signals, ...)
{
	sigset_t mask;

	if (-1 == sigemptyset(&mask)) {
		perror("sigemptyset");
		errno = 0;
		return 1;
	}

	va_list signal_list;
	va_start(signal_list, num_signals);

	for (int i = 0; i < num_signals; i++) {
		int signum = va_arg(signal_list, int);

		if (-1 == sigaddset(&mask, signum)) {
			perror("sigaddset");
			errno = 0;
			va_end(signal_list);
			return 1;
		}
	}

	va_end(signal_list);

	if (-1 == sigprocmask(SIG_BLOCK, &mask, NULL)) {
		perror("sigpromask");
		errno = 0;
		return 1;
	}

	return 0;
}				/* block_signals */

int unblock_signals(int num_signals, ...)
{
	sigset_t mask;
	if (-1 == sigemptyset(&mask)) {
		perror("sigemptyset");
		errno = 0;
		return 1;
	}

	va_list signal_list;
	va_start(signal_list, num_signals);

	for (int i = 0; i < num_signals; i++) {
		int signum = va_arg(signal_list, int);
		if (-1 == sigaddset(&mask, signum)) {
			perror("sigaddset");
			errno = 0;
			va_end(signal_list);
			return 1;
		}
	}

	va_end(signal_list);

	if (-1 == sigprocmask(SIG_UNBLOCK, &mask, NULL)) {
		perror("sigprocmask");
		errno = 0;
		return 1;
	}

	return 0;
}				/* unblock_signals */

int block_all_signals(void)
{
	sigset_t mask;
	if (sigfillset(&mask) == -1) {
		perror("sigfillset");
		return 1;
	}

	if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1) {
		perror("sigprocmask");
		return 1;
	}

	return 0;
}				/* block_all_signals */

int unblock_all_signals(void)
{
	sigset_t mask;
	if (sigemptyset(&mask) == -1) {
		perror("sigemptyset");
		return 1;
	}

	if (sigprocmask(SIG_UNBLOCK, &mask, NULL) == -1) {
		perror("sigprocmask");
		return 1;
	}

	return 0;
}				/* unblock_all_signals */

/* end of file */
