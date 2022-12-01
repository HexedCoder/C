#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
    NUM_BITS_INT = 8 * sizeof(int)
};

char *get_binary_string(int n) {
    char *bits = calloc(NUM_BITS_INT, sizeof(char));

    for (int bit = NUM_BITS_INT - 1; bit >= 0; bit--) {
        // this is simply moving the 1 over bit times
        printf("%d\n", 1 << bit);
        if ((1 << bit) & n)
            strcat(bits, "1");
        else
            strcat(bits, "0");
    }
    return bits;
}

void format_binary_str(char *bin_for_num, int num_in_grouping) {
    printf("Grouped by %d bits\n", num_in_grouping);
    for (int bidx = 0; bidx < NUM_BITS_INT; bidx++) {
        if (bidx != 0 && bidx % num_in_grouping == 0)
            printf(" ");
        printf("%c", bin_for_num[bidx]);
    }
    printf("\n");

}

void print_random() {
    printf("%lu\n", (unsigned long) rand() << 32 | (unsigned long) rand());
}

int main() {
    int num;
    printf("Enter an integer number :");
    scanf("%d", &num);
    char *bin_for_num = get_binary_string(num);
    printf("\nBinary value of %d is = %s\n", num, bin_for_num);
    format_binary_str(bin_for_num, 4);
    format_binary_str(bin_for_num, 8);
    format_binary_str(bin_for_num, 16);

    free(bin_for_num);
}