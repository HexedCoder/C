// Exercise 3, Lesson 8: Make new structs that are copies of existing structs but require less memory.
#include <stdio.h>
#include <string.h>

#define BITS_IN_INT 32

void get_string(unsigned int number, char *word) {
    for (int bit = BITS_IN_INT - 1; bit >= 0; bit--) {

        if ((1 << bit) & number)
            strcat(word, "1");
        else
            strcat(word, "0");
    }
}

int print_bits(unsigned int first, unsigned int second) {
    int flips = 0;

    char first_str[BITS_IN_INT + 1] = "\0";
    get_string(first, first_str);
    printf("\nString value of first:  %s\n", first_str);

    char second_str[BITS_IN_INT + 1] = "\0";
    get_string(second, second_str);

    printf("String value of second: %s\n", second_str);

    unsigned int x_or = first ^ second;

    char x_or_str[BITS_IN_INT + 1] = "\0";
    get_string(x_or, x_or_str);

    printf("\nString value of x_or:   %s\n\n", x_or_str);

    for (int i = 0; i < BITS_IN_INT; i++) {
        if (x_or_str[i] == '1')
            flips += 1;
    }

    return flips;
}

int main(void) {
    unsigned int first;
    unsigned int second;
    printf("Enter the first number ==> \n");
    scanf("%ud", &first);

    printf("Enter the second number ==> \n");
    scanf("%ud", &second);

    int flips = print_bits(first, second);

    printf("Must flip %d bits to change %d to %d\n", flips, first, second);
}