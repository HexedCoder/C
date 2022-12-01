#include <stdio.h>

int pwr_2(int num) {
    int pwr_two = 2;
    while (pwr_two < num)
        pwr_two *= 2;

    return pwr_two == num;
}

int main(void) {
    int nums[] = {2, 15, 4, 99, 101, 256};
    size_t num_elems = sizeof(nums) / sizeof(int);
    for (int idx = 0; idx < num_elems; ++idx) {
        int i = nums[idx];
        printf("%4d mod 8         = %d\n", i, i % 8);
        printf("%4d mod 8 bit     = %d\n", i, i & 7);

        printf("%4d times 33 	   = %d\n", i, i * 33);
        printf("%4d times 33 bit  = %d\n", i, (i << 5) + i);

        printf("%4d %s a power of 2\n",
               i, (pwr_2(i)) ? "is" : "is not");
        printf("%4d %s a power of 2 (bits)\n\n",
               i, i & (i - 1) ? "is not" : "is");

    }
}