// Exercise 1, lesson 9: Code a 'printf' type function that prints characters, strings or integers

#include <stdio.h>
#include <stdarg.h>


void my_printf(char *fmt, ...)
{
    va_list ap;
    int d;
    char c;
    char *s;

    va_start(ap, fmt);
    while (*fmt)
        switch (*fmt++) {
            case 's':              /* string */
                s = va_arg(ap, char *);
                printf("string '%s'\n", s);
                break;
            case 'd':              /* int */
                d = va_arg(ap, int);
                printf("int %d\n", d);
                break;

            case '%':              /* int */
                d = va_arg(ap, int);
                printf("int %d\n", d);
                break;


            case 'c':              /* char */
                /* need a cast here since va_arg only
                   takes fully promoted types */
                c = va_arg(ap, int);
                printf("char %c\n", c);
                break;
        }
    va_end(ap);
}

int main(void)
{
    my_printf("%s%dc", 10, "hi there", 5, 25, 'x');
}

/*

----
string '   hi there'
int 00025
char x
----
*/