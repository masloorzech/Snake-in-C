#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "color.h"
#include <GLFW/glfw3.h>

#pragma pack(push, 1)
typedef struct {
    unsigned short type;
    unsigned int size;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int offset;
} BMPFileHeader;

typedef struct {
    unsigned int size;
    int width, height;
    unsigned short planes;
    unsigned short bits;
    unsigned int compression;
    unsigned int imageSize;
    int xResolution, yResolution;
    unsigned int nColours;
    unsigned int importantColours;
} BMPInfoHeader;

typedef struct{
    uint8_t r;
    uint8_t g;
    uint8_t b;
}BMPPixel;

typedef struct {
    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;
    BMPPixel** data;
} BMPImage;

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
    GLuint letters[128];
}font;

#pragma pack(pop)

BMPImage* LoadBMP24(const char* filePath);
void freeBMPImage(BMPImage* image);
void freeFont(font* font_t);
void MirrorBMPHorizontally(BMPPixel* pixelData,unsigned int width,unsigned int height);
void RotateBMP180(BMPPixel* pixelData,unsigned int width, unsigned int height);
GLuint CreateTextureFromBMP(BMPImage* image);
GLuint CreateTextureFromBMPSegment(BMPImage* image, int x, int y, int width, int height);
int fontInit(font* font_n,BMPImage* image,uint16_t letter_width, uint16_t letter_height,uint16_t rows,uint16_t columns,size_t first_asci_character);
void RenderTexturedQuad(GLuint texture, float x, float y, float width, float height);
int nearestGreaterOrEqualPowerOfTwo(int num);
void RenderChar(font* font_n, char c, int x, int y, font_format* font_configuration);
void RenderString(font* font_t, char* string, int x, int y, font_format* font_configuration);
void debugPrintBMPInTerminal(BMPImage* image);
void BMPConvertPixels(BMPImage * image, color primary_color, color new_color);
