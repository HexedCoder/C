/** @file signals.h
*
* @brief This module serves as the template for functions in signals.c
*
* @par
* COPYRIGHT NOTICE: (c) 2023 Jacob Hitchcox
*/

#ifndef SIGNALS_H
#define SIGNALS_H

#include <signal.h>

typedef void (*handler_func)(int);

/**
 * Establishes a signal handler for the specified signal number.
 *
 * @param signum The signal number to establish the handler for.
 * @param func The function pointer to the signal handler.
 *
 * @return 0 if the signal handler is established successfully, 1 otherwise.
 */
int establish_handler(int sigsum, handler_func func);

/**
 * Blocks the specified signals, adding them to the current signal mask.
 *
 * @param num_signals The number of signals to block.
 * @param ... The variable list of signal numbers to block.
 */
int block_signals(int num_signals, ...);

/**
 * Unblocks the specified signals in the current process.
 *
 * @param num_signals The number of signals to unblock.
 * @param ... Variable number of signal numbers to unblock.
 */
int unblock_signals(int num_signals, ...);

/**
 * Block all signals in the current process.
 */
int block_all_signals(void);

/**
 * Unblock all signals in the current process.
 */
int unblock_all_signals(void);

#endif				/* SIGNALS_H */

/*** end of file ***/
