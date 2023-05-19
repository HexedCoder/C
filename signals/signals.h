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

int establish_handler(int sigsum, handler func);

void block_signals(int num_signals, ...);

void unblock_signals(int num_signals, ...);

void block_all_signals(void);

void unblock_all_signals(void);

#endif				/* SIGNALS_H */

/* end of file */
