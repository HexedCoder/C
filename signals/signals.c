/**
 * @file signals.c
 * @brief function definition for establishing signal handlers
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "signals.h"

int establish_handler(int signum, handler_func func)
{
	if (!signum || !func) {
		return 1;
	}
	struct sigaction sa = {
		.sa_handler = func,
		.sa_flags = 0,
	};

	int result = 0;

	if (signum == SIGWINCH) {
		sa.sa_flags = SA_RESTART;
	}

	if (-1 == sigaction(signum, &sa, NULL)) {
		perror("sigaction");
		errno = 0;
		result = 1;
	}

	return result;
}				/* establish_handler() */

void block_signals(int num_signals, ...)
{
	if (num_signals < 1) {
		return;
	}

	sigset_t mask;

	sigemptyset(&mask);

	va_list signal_list;
	va_start(signal_list, num_signals);

	for (int i = 0; i < num_signals; i++) {
		int signum = va_arg(signal_list, int);

		sigaddset(&mask, signum);
	}

	va_end(signal_list);

	sigprocmask(SIG_BLOCK, &mask, NULL);
}				/* block_signals() */

void unblock_signals(int num_signals, ...)
{
	if (num_signals < 1) {
		return;
	}

	sigset_t mask;
	sigemptyset(&mask);

	va_list signal_list;
	va_start(signal_list, num_signals);

	for (int i = 0; i < num_signals; i++) {
		int signum = va_arg(signal_list, int);
		sigaddset(&mask, signum);
	}

	va_end(signal_list);

	sigprocmask(SIG_UNBLOCK, &mask, NULL)
}				/* unblock_signals() */

void block_all_signals(void)
{
	sigset_t mask;
	sigfillset(&mask);

	sigprocmask(SIG_BLOCK, &mask, NULL);
}				/* block_all_signals() */

void unblock_all_signals(void)
{
	sigset_t mask;
	sigemptyset(&mask);

	sigprocmask(SIG_UNBLOCK, &mask, NULL);
}				/* unblock_all_signals() */

/* end of file */
