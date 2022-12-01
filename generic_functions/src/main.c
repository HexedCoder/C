#include "car.h"
#include "human.h"

void print_array(void **array, int arr_sz, void (*printer)(void *element));

int main() {
  car_t *cars[5] = {NULL};
  // Should have verified these init are successful but avoided
  // to keep on one page
  cars[0] = car_init("Subaru", "Outback", 5);
  cars[1] = car_init("BMW", "840i", 1);
  cars[2] = car_init("Toyota", "Supra", 2);
  cars[3] = car_init("Audi", "RS5", 1);
  cars[4] = car_init("Kia", "Sorento", 4);

  print_array((void **)cars, 5, (void (*)(void *))car_print);

  for (int i = 0; i < 5; ++i) {
    car_destroy(&(cars[i]));
  }

  printf("\n");
  human_t *humans[5] = {NULL};
  humans[0] = human_init("Kyle", "Matuszczak");
  humans[1] = human_init("Timmy", "Smith");
  humans[2] = human_init("Kimmy", "Johnson");
  humans[3] = human_init("Jimmy", "Martinez");
  humans[4] = human_init("Kevin", "Minion");

  print_array((void **)humans, 5, (void (*)(void *))human_print);

  for (int i = 0; i < 5; ++i) { `
    human_departs(&(humans[i]));
  }
}

void print_array(void **array, int arr_sz, void (*printer)(void *element)) {
  if (!array) {
    return;
  }

  for (int i = 0; i < arr_sz; ++i) {
    printer(array[i]);
  }
}
