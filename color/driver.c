#include <stdio.h>
#include <stdlib.h>

#include "colors.h"

int main(void)
{
	printf("Hello World\n");

	change_color(COLOR_RED);
	printf("Hello World\n");

	change_color(COLOR_GREEN);
	printf("Hello World\n");

	change_color(COLOR_BLUE);
	printf("Hello World\n");

	change_color(COLOR_CYAN);
	printf("Hello World\n");

	change_color(COLOR_YELLOW);
	printf("Hello World\n");

	change_color(COLOR_MAGENTA);
	printf("Hello World\n");

	change_color(COLOR_WHITE);
	printf("Hello World\n");

	change_color(COLOR_BLACK);
	printf("Hello World\n");

	change_color(COLOR_GRAY);
	printf("Hello World\n");

	change_color(COLOR_BRIGHT_RED);
	printf("Hello World\n");

	change_color(COLOR_BRIGHT_BLUE);
	printf("Hello World\n");

	change_color(COLOR_BRIGHT_GREEN);
	printf("Hello World\n");

	change_color(COLOR_BRIGHT_YELLOW);
	printf("Hello World\n");

	change_color(COLOR_BRIGHT_MAGENTA);
	printf("Hello World\n");

	change_color(COLOR_BRIGHT_CYAN);
	printf("Hello World\n");

	change_color(COLOR_BRIGHT_WHITE);
	printf("Hello World\n");

	reset_color();

	change_font(FONT_BOLD);

	printf("\nHello World\n");

	reset_color();

	printf("Hello World\n");

	return 0;
}