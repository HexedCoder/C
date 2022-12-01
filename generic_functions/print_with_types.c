#include <stdarg.h>
#include <stdio.h>
#include <string.h>
//
// Exercise 1 - Lesson 9.
// my_print function that prints characters, strings or integers.
// main, accepts a format string containing data types along with data to be printed.
//

void my_print(char * format,...) {
    /* ap represents the arguments passed to program */
    va_list ap;
    int i;
    int arg_cnt;

    arg_cnt = strlen(format);
    /* Initialize the argument list. */
    va_start(ap, format);

    // Loop through arguments

    for (i = 0; i < arg_cnt; i++) {
        switch (format[i]) {
            case 's':
                printf("string %s \n", va_arg(ap, char*));
                break;
            case 'd':
                printf("int %d \n", va_arg(ap, int));
                break;
            case 'c':
                printf("char %c \n ", va_arg(ap, int));
                break;
            default:
                printf("error\n");
                break;
        }
    }

    /* Clean up. */
    va_end(ap);
}


int main (void)
{
    my_print("sdc", "hi there", 25, 'x');
}