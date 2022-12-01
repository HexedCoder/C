#include "car.h"

#define MAX_LEN 64

struct car_t {
  char *brand;
  char *type;
  uint64_t amt;
};

car_t *car_init(const char *brand, const char *type, uint64_t amount) {
  car_t *car = NULL;
  if (!brand || !type || 0 == amount) {
    fprintf(stderr, "[ERR!] Invalid input\n");
    goto CAR_INIT_FINISHED;
  }

  if (MAX_LEN < strnlen(brand, MAX_LEN + 1)) {
    fprintf(stderr, "[ERR!] %s exceeds %d chars\n", brand, MAX_LEN);
    goto CAR_INIT_FINISHED;
  }

  if (MAX_LEN < strnlen(type, MAX_LEN + 1)) {
    fprintf(stderr, "[ERR!] %s exceeds %d chars\n", type, MAX_LEN);
    goto CAR_INIT_FINISHED;
  }

  car = calloc(1, sizeof(*car));
  if (!car) {
    fprintf(stderr, "[ERR!] Cannot allocate\n");
    goto CAR_INIT_FINISHED;
  }

  // No need for strndup since we already checked bounds
  //
  car->brand = strdup(brand);
  if (!car->brand) {
    fprintf(stderr, "[ERR!] Cannot allocate for brand\n");
    goto CAR_INIT_FINISHED;
  }

  car->type = strdup(type);
  if (!car->type) {
    fprintf(stderr, "[ERR!] Cannot allocate for type\n");
    goto CAR_INIT_FINISHED;
  }

  car->amt = amount;

CAR_INIT_FINISHED:
  return car;
}

void car_print(car_t *car) {
  if (!car) {
    return;
  }

  printf("%02lu: %s %s\n", car->amt, car->brand, car->type);
}

int car_destroy(car_t **car) {
  int ret = 0;
  if (!car || !*car) {
    fprintf(stderr, "[ERR!] Cannot dereference\n");
    goto CAR_DESTROY;
  }

  free((*car)->brand);
  free((*car)->type);
  free(*car);
  *car = NULL;
  ret = 1;

CAR_DESTROY:
  return ret;
}
