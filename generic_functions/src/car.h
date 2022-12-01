#ifndef CAR_H
#define CAR_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct car_t car_t;

car_t *car_init(const char *brand, const char *type, uint64_t amount);

void car_print(car_t *car);

int car_destroy(car_t **car);

#endif
