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

typedef void (*handler)(int);

int establish_handler(int sigsum, handler func);

int block_signals(int num_signals, ...);

int unblock_signals(int num_signals, ...);

int block_all_signals(void);

int unblock_all_signals(void);

#endif				/* SIGNALS_H */

/* end of file */
