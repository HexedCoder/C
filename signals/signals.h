/**
 * @file signals.h
 * @brief basic library to establish signal handlers
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef SIGNALS_H
#define SIGNALS_H

#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

typedef void (*handler)(int);

/**
 * Establishes a signal handler for the specified signal number.
 *
 * @param signum The signal number to establish the handler for.
 * @param func The function pointer to the signal handler.
 *
 * @return 0 if the signal handler is established successfully, 1 otherwise.
 */
int establish_handler(int sigsum, handler func);

/**
 * Blocks the specified signals, adding them to the current signal mask.
 *
 * @param num_signals The number of signals to block.
 * @param ... The variable list of signal numbers to block.
 */
void block_signals(int num_signals, ...);

/**
 * Unblocks the specified signals in the current process.
 *
 * @param num_signals The number of signals to unblock.
 * @param ... Variable number of signal numbers to unblock.
 */
void unblock_signals(int num_signals, ...);

/**
 * Block all signals in the current process.
 */
void block_all_signals(void);

/**
 * Unblock all signals in the current process.
 */
void unblock_all_signals(void);

#endif				/* SIGNALS_H */

/* end of file */
