#ifndef HUMAN_H
#define HUMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct human_t human_t;

human_t *human_init(const char *fname, const char *lname);

void human_print(human_t *human);

int human_rename(human_t *human, const char *fname, const char *lname);

int human_departs(human_t **human);

#endif
