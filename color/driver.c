#include <stdio.h>
#include <stdlib.h>

#include "colors.h"

int main(void) {

    printf("Hello World\n");
    
    change_color(red);
    printf("Hello World\n");
    
    change_color(green);
    printf("Hello World\n");
    
    change_color(blue);
    printf("Hello World\n");
    
    change_color(cyan);
    printf("Hello World\n");
    
    change_color(yellow);
    printf("Hello World\n");

    change_color(magenta);
    printf("Hello World\n");

    change_color(white);
    printf("Hello World\n");

    change_color(black);
    printf("Hello World\n");

    change_color(gray);
    printf("Hello World\n");

    change_color(brightRed);
    printf("Hello World\n");

    change_color(brightBlue);
    printf("Hello World\n");

    change_color(brightGreen);
    printf("Hello World\n");

    change_color(brightYellow);
    printf("Hello World\n");

    change_color(brightMagenta);
    printf("Hello World\n");

    change_color(brightCyan);
    printf("Hello World\n");

    change_color(brightWhite);
    printf("Hello World\n");

    reset_color();

    change_font(bold);

    printf("\nHello World\n");

    reset_color();

    printf("Hello World\n");

    
    return 0;
}