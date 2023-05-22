#include <stdio.h>
#include <stdlib.h>

#include "colors.h"

struct color_t red = {
    "\033[31m"   // Start escape sequence for red color
};

struct color_t green = {
    "\033[32m"   // Start escape sequence for green color
};

struct color_t blue = {
    "\033[34m"   // Start escape sequence for blue color
};

struct color_t cyan = {
    "\033[36m"   // Start escape sequence for cyan color
};

struct color_t yellow = {
    "\033[33m"   // Start escape sequence for yellow color
};

struct color_t magenta = {
    "\033[35m"   // Start escape sequence for magenta color
};

struct color_t white = {
    "\033[37m"   // Start escape sequence for white color
};

struct color_t black = {
    "\033[30m"   // Start escape sequence for black color
};

struct color_t gray = {
    "\033[90m"   // Start escape sequence for gray color
};

struct color_t brightRed = {
    "\033[91m"   // Start escape sequence for bright red color
};

struct color_t brightGreen = {
    "\033[92m"   // Start escape sequence for bright green color
};

struct color_t brightYellow = {
    "\033[93m"   // Start escape sequence for bright yellow color
};

struct color_t brightBlue = {
    "\033[94m"   // Start escape sequence for bright blue color
};

struct color_t brightMagenta = {
    "\033[95m"   // Start escape sequence for bright magenta color
};

struct color_t brightCyan = {
    "\033[96m"   // Start escape sequence for bright cyan color
};

struct color_t brightWhite = {
    "\033[97m"   // Start escape sequence for bright white color
};

struct font_t bold = {
    "\033[1m"    // Start escape sequence for bold attribute
};

struct font_t clear_screen = {
    "\033[2J"    // Escape sequence to clear the screen
};



void change_color(color_t color) 
{
    printf("%s", color.color);
}

void change_font(font_t option) 
{
    printf("%s", option.font);
}

void reset_color(void)
{
    printf("\033[0m");
}
