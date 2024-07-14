#include <GLFW/glfw3.h>
#include "color.h"

#define MAXNUMBEROFCHARACTERINFONT 128

typedef struct font_formatter_t{
    uint8_t width;
    uint8_t height;
    int letter_spacing;
    color background_color;
    color font_color;
}font_format;

typedef struct font_t{
    int width;
    int height;
    int firstasci_character;
    GLuint letters[MAXNUMBEROFCHARACTERINFONT];
}font;
