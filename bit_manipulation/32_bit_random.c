#include <stdio.h>
#include <stdlib.h>

void print_random() {
    printf("%lu\n", (unsigned long) rand() << 32 | (unsigned long) rand());
}

int main() {
    for (int i = 0; i < 20; i++)
        print_random();
}