#ifndef COLOR_H
#define COLOR_H

#define COLOR_WHITE (color){1,1,1}
#define COLOR_BLACK (color){0,0,0}
#define COLOR_RED (color) {1.0f,0.0f,0.0f}
#define COLOR_GREEN (color) {0.0f,1.0f,0.0f}
#define COLOR_BLUE (color) {0.0f,0.0f,1.0f}
#define COLOR_PINK (color) {1.0f,0.0f,1.0f}
#define COLOR_YELLOW (color) {1.0f,1.0f,0.0f}
#define COLOR_CYAN (color) {0.0f,1.0f,1.0f}
#define COLOR_PURPLE (color) {0.522f, 0.0f, 1.0f}

typedef struct color_rgb{
    float r;
    float g;
    float b;
}color;

#endif /* COLOR_H */
