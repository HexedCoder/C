#include <stdio.h>

#include "colors.h"

void change_color(char *color)
{
	printf("%s", color);
}

void change_font(char *option)
{
	printf("%s", option);
}

void reset_color(void)
{
    printf("\033[0m");
}
