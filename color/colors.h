#ifndef COLOR_H
#define COLOR_H


typedef struct color_t {
    const char * color;

} color_t;

typedef struct font_t {
    const char * font;

} font_t;

extern struct color_t red;
extern struct color_t green;
extern struct color_t blue;
extern struct color_t cyan;
extern struct color_t yellow;
extern struct color_t magenta;
extern struct color_t white;
extern struct color_t black;
extern struct color_t gray;
extern struct color_t brightRed;
extern struct color_t brightGreen;
extern struct color_t brightBlue;
extern struct color_t brightYellow;
extern struct color_t brightMagenta;
extern struct color_t brightCyan;
extern struct color_t brightWhite;

extern struct font_t bold;
extern struct font_t clear_screen;


void change_color(color_t color);

void change_font(font_t option);

void reset_color(void);


#endif /* COLOR_H */