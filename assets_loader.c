#include "assets_loader.h"

BMPImage* LoadBMP24(const char* filePath) {
    FILE* file = fopen(filePath, "rb");

    if (!file) {
        printf("Failed to open file %s\n", filePath);
        return NULL;
    }

    BMPFileHeader fileHeader;
    fread(&fileHeader, sizeof(BMPFileHeader), 1, file);

    if (fileHeader.type != 0x4D42) {
        printf("Not a BMP file: %s\n", filePath);
        fclose(file);
        return NULL;
    }

    BMPInfoHeader infoHeader;
    fread(&infoHeader, sizeof(BMPInfoHeader), 1, file);

    if (infoHeader.bits != 24) {
        printf("Only 24-bit BMP files are supported: %s\n", filePath);
        fclose(file);
        return NULL;
    }

    if (infoHeader.imageSize == 0) {
        infoHeader.imageSize = infoHeader.width * infoHeader.height * (infoHeader.bits / 8);
    }

    BMPImage* image = (BMPImage*)malloc(sizeof(BMPImage));
    if (!image) {
        printf("Failed to allocate memory for BMPImage\n");
        fclose(file);
        return NULL;
    }

    image->fileHeader = fileHeader;
    image->infoHeader = infoHeader;

    image->data = (BMPPixel**)malloc(sizeof(BMPPixel*) * infoHeader.height);
    if (!image->data) {
        printf("Failed to allocate memory for BMPImage data\n");
        free(image);
        fclose(file);
        return NULL;
    }

    for (int i = 0; i < infoHeader.height; i++) {
        image->data[i] = (BMPPixel*)malloc(infoHeader.width * sizeof(BMPPixel));
        if (!image->data[i]) {
            printf("Failed to allocate memory for BMPImage row %d\n", i);
            for (int j = 0; j < i; j++) {
                free(image->data[j]);
            }
            free(image->data);
            free(image);
            fclose(file);
            return NULL;
        }
    }

    fseek(file, fileHeader.offset, SEEK_SET);

    for (int i = infoHeader.height - 1; i >= 0; i--) {
        fread(image->data[i], sizeof(BMPPixel), infoHeader.width, file);
    }

    fclose(file);

    return image;
}

void freeBMPImage(BMPImage* image) {
    if (image) {
        if (image->data) {
            for (int i = 0; i < image->infoHeader.height; i++) {
                free(image->data[i]);
            }
            free(image->data);
        }
        free(image);
    }
}

void freeFont(font* font_t) {
    if (font_t) {
        for (int i = 0; i < 128; i++) {
            glDeleteTextures(1, &font_t->letters[i]);
        }
    }
}
void debugPrintBMPInTerminal(BMPImage* image){

    for (int i = 0; i < image->infoHeader.height; i++) {
        printf("\n");
        for (int j = 0; j < image->infoHeader.width; j++) {
            if (image->data[i][j].r == 0 && image->data[i][j].g == 0 && image->data[i][j].b == 0) {
                printf("1");
            } else {
                printf(" ");
            }
        }
    }

}

void MirrorBMPHorizontally(BMPPixel* pixelData, int width, int height) {
    BMPPixel temp;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width / 2; x++) {
            temp = pixelData[y * width + x];
            pixelData[y * width + x] = pixelData[y * width + (width - 1 - x)];
            pixelData[y * width + (width - 1 - x)] = temp;
        }
    }
}


void RotateBMP180(BMPPixel* pixelData, int width, int height) {
    BMPPixel* temp = (BMPPixel*)malloc(width * height * sizeof(BMPPixel));
    if (!temp) {
        printf("Failed to allocate memory for rotation\n");
        return;
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            temp[(height - 1 - y) * width + (width - 1 - x)] = pixelData[y * width + x];
        }
    }

    memcpy(pixelData, temp, width * height * sizeof(BMPPixel));
    free(temp);
}

void BMPConvertPixels(BMPImage * image, color primary_color, color new_color){

    for(size_t i =0; i<image->infoHeader.height; i++){
        for(size_t j =0; j<image->infoHeader.width; j++){
            if (image->data[i][j].r == primary_color.r *255 && image->data[i][j].r == primary_color.g *255 && image->data[i][j].r == primary_color.b *255){
                image->data[i][j].r = new_color.r *255;
                image->data[i][j].g = new_color.g *255;
                image->data[i][j].b = new_color.b *255;
            }
        }
    }

}

GLuint CreateTextureFromBMP(BMPImage* image) {

    if (image==NULL){
        printf("Error. CreateTextureFromBMP with NULL arguments");
        return 1;
    }
    /*
    Przekształcamy tablice dwuwymairową na jedno wymiarową aby móc stowrzyć z niej teksturę i móc z niej kozystać
    */

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    int width = image->infoHeader.width;
    int height = image->infoHeader.height;

    BMPPixel* pixelData = (BMPPixel*)malloc(width * height * sizeof(BMPPixel));
    if (!pixelData) {
        printf("Failed to allocate memory for pixel data\n");
        glBindTexture(GL_TEXTURE_2D, 0);
        return 0;
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            pixelData[(height - 1 - y) * width + x] = image->data[y][x];
        }
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, pixelData);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    free(pixelData);

    return texture;
}

GLuint CreateTextureFromBMPSegment(BMPImage* image, int x, int y, int width, int height){

    if (x < 0 || x >= image->infoHeader.width) {
        printf("Error: x coordinate out of bounds\n");
        return -1;
    }
    if (y < 0 || y >= image->infoHeader.height) {
        printf("Error: y coordinate out of bounds\n");
        return -2;
    }
    if (x + width > image->infoHeader.width) {
        printf("Error: segment width exceeds image width\n");
        return -3;
    }
    if (y + height > image->infoHeader.height) {
        printf("Error: segment height exceeds image height\n");
        return -4;
    }
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    BMPPixel* pixelData = (BMPPixel*)malloc(width * height * sizeof(BMPPixel));
    if (!pixelData) {
        printf("Failed to allocate memory for pixel data\n");
        glBindTexture(GL_TEXTURE_2D, 0);
        return 0;
    }

    size_t index=0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            pixelData[index] = image->data[y + i][x + j];
            index++;
        }
    }

    if ((width & (width - 1)) != 0 || (height & (height - 1)) != 0) {
            RotateBMP180(pixelData, width, height);
            MirrorBMPHorizontally(pixelData, width, height);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, pixelData);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);

    free(pixelData);

    return texture;
}



int fontInit(font* font_n,BMPImage* image,uint16_t letter_width, uint16_t letter_height,uint16_t rows,uint16_t columns,size_t first_asci_character){

    if (image==NULL){
        printf("Error sending NULL image fontInit font\n");
        return -1;
    }

    font_n->height = letter_height;
    font_n->width = letter_width;
    font_n->firstasci_character = first_asci_character;

    size_t index=0;

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < columns; j++) {
            int x = j * letter_width;
            int y = i * letter_height;
            font_n->letters[index] = CreateTextureFromBMPSegment(image, x, y, letter_width, letter_height);
            index++;
        }
    }
    return 0;
}




void RenderTexturedQuad(GLuint texture, float x, float y, float width, float height) {

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);


    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(x, y);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(x + width, y);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(x + width, y + height);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y + height);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

}

int nearestGreaterOrEqualPowerOfTwo(int num) {
    int power = 1;
    while (power < num) {
        power *= 2;
    }
    return power;
}

void renderOrginalChar(font* font_n, char c, int x, int y, font_format* font_configuration){
    RenderTexturedQuad(font_n->letters[(int)c - font_n->firstasci_character], x, y, font_configuration->width, font_configuration->height);
}

void RenderChar(font* font_n, char c, int x, int y, font_format* font_configuration) {
    if (font_n == NULL || font_configuration == NULL){
        printf("Sending Null font or null font_config to renderChar");
        return;
    }

    if (font_configuration->font_color.r == 1 && font_configuration->font_color.g == 1 && font_configuration->font_color.b == 1 &&
        font_configuration->background_color.r == 0 && font_configuration->background_color.g == 0 && font_configuration->background_color.b == 0 ){
            renderOrginalChar(font_n, c,  x,  y, font_configuration);
            return;
    }
    int width = nearestGreaterOrEqualPowerOfTwo(font_n->width);
    int height = font_n->height;

    GLuint originalTexture = font_n->letters[(int)c - font_n->firstasci_character];

    if (originalTexture == 0) {
            printf("Invalid original texture for character %c\n", c);
            return;
        }

    BMPPixel* pixelData = (BMPPixel*)malloc(8 * 9 *  sizeof(BMPPixel));
    if (!pixelData) {
        printf("Failed to allocate memory for pixel data\n");
        glBindTexture(GL_TEXTURE_2D, 0);
        return;
    }

    glBindTexture(GL_TEXTURE_2D, originalTexture);

    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, pixelData);

    for (int i = 0; i < width * height; i++) {
        if (pixelData[i].r == 0  && pixelData[i].g == 0 && pixelData[i].b == 0) {
            pixelData[i].r = font_configuration->background_color.r * 255;
            pixelData[i].g = font_configuration->background_color.g * 255;
            pixelData[i].b = font_configuration->background_color.b * 255;
        }else{
            pixelData[i].r = font_configuration->font_color.r * 255;
            pixelData[i].g = font_configuration->font_color.g * 255;
            pixelData[i].b = font_configuration->font_color.b * 255;
        }
    }


    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint tempTexture;

    glGenTextures(1,&tempTexture);
    glBindTexture(GL_TEXTURE_2D,tempTexture);
    glBindTexture(GL_TEXTURE_2D, tempTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixelData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    RenderTexturedQuad(tempTexture, x, y, font_configuration->width, font_configuration->height);

    glDeleteTextures(1, &tempTexture);

    free(pixelData);
}

void RenderString(font* font_t, char* string, int x, int y, font_format* font_configuration){

    int spacing  = x;
    int index =0;
    while(string[index]!='\0'){
        RenderChar(font_t,string[index],spacing,y,font_configuration);
        spacing += font_t->width+(font_configuration->width)+font_configuration->letter_spacing;
        index++;
    }

}
