
#include <stdint.h>
#include <string.h>
#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif


#include <stdio.h>
#include <GLFW/glfw3.h>
#include <time.h>
#include <stdlib.h>

#include "snake.h"
#include "assets_loader.h"
#include "color.h"

#define BORDER_THICKNESS 5
#define PIXEL_SIZE 20
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define BACKGROUND_COLOR (color){0.1f, 0.1f, 0.1f}

boolean game_started=0;
uint32_t last_game_points =0;
GLuint snake_textures[14];

/*
Ustawianie rzutu orgtogonalnego
Ustawiamy środek ukąłdu wspólrzednych na lewy górny róg ekranu
Zmieniamy wspólrzedne z -0.0f na int
*/
void set_orthogonial(void){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
//funkcja rysujaca prostokąt

void draw_rectangle(float x, float y, float width, float height,color rec_color){

    GLfloat previous_color[4];
    glGetFloatv(GL_CURRENT_COLOR, previous_color);

    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
        glColor3f(rec_color.r,rec_color.g,rec_color.b);
        glVertex2f(x,y);
        glVertex2f(x+width,y);
        glVertex2f(x+width,y+height);
        glVertex2f(x,y+height);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glColor4fv(previous_color);
}

void draw_rectangle_frame(float x, float y, float width, float height, float border_width, color border_color, color background_color){

    GLfloat previous_color[4];
    glGetFloatv(GL_CURRENT_COLOR, previous_color);

    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);

        glColor3f(border_color.r,border_color.g,border_color.b);
        glVertex2f(x,y);
        glVertex2f(x+width,y);
        glVertex2f(x+width,y+height);
        glVertex2f(x,y+height);

        glColor3f(background_color.r,background_color.g,background_color.b);
        glVertex2f(x+border_width,y+border_width);
        glVertex2f(x+width-border_width,y+border_width);
        glVertex2f(x+width-border_width,y+height-border_width);
        glVertex2f(x+border_width,y+height-border_width);

    glEnd();
    glDisable(GL_TEXTURE_2D);

    glColor4fv(previous_color);

}

void drawStartScreen(font* font){

    int border_left_corner_x = SCREEN_WIDTH/2 - (MAP_WIDTH/2)*PIXEL_SIZE -BORDER_THICKNESS;
    int border_left_corner_y = SCREEN_HEIGHT/2 - (MAP_HEIGHT/2)*PIXEL_SIZE - BORDER_THICKNESS;
    int width =MAP_WIDTH*PIXEL_SIZE + 2*BORDER_THICKNESS;
    draw_rectangle_frame(border_left_corner_x,border_left_corner_y,width,MAP_HEIGHT/2*PIXEL_SIZE +2*BORDER_THICKNESS,BORDER_THICKNESS,(color){0.902, 0.722, 0.384},BACKGROUND_COLOR);

    font_format font_config = {
        16,32,-7,BACKGROUND_COLOR,(color){0.902, 0.722, 0.384}
    };

    int string_width = 14*font_config.width;

    int string_offset = (width - string_width)/2;


    RenderString(font, "SNAKE THE GAME", border_left_corner_x + string_offset, border_left_corner_y + BORDER_THICKNESS, &font_config);

    font_format lower_font_config = {
        14,16,-7,BACKGROUND_COLOR,(color){0.902, 0.722, 0.384}
    };

    string_width = strlen("PRESS ANY KEY TO BEGIN") * lower_font_config.width;
    string_offset = (width - string_width)/2;

    RenderString(font, "PRESS ANY KEY TO BEGIN", border_left_corner_x + string_offset, border_left_corner_y + 2 * BORDER_THICKNESS + 2*font_config.height , &lower_font_config);

    string_width = strlen("MADE BY") * lower_font_config.width;
    string_offset = (width - string_width)/2;

    RenderString(font, "MADE BY", border_left_corner_x + string_offset, border_left_corner_y + 2 * BORDER_THICKNESS + 3*font_config.height , &lower_font_config);

    string_width = strlen( "MASLOORZECH") * lower_font_config.width;
    string_offset = (width - string_width)/2;

    RenderString(font, "MASLOORZECH", border_left_corner_x + string_offset, border_left_corner_y + 2 * BORDER_THICKNESS +4* font_config.height , &lower_font_config);


    char points_message[50] ={0};
    sprintf(points_message, "LAST GAME POINTS: %d", last_game_points);
    string_width = strlen(points_message) * lower_font_config.width;
    string_offset = (width - string_width)/2;

    RenderString(font, points_message, border_left_corner_x + string_offset, border_left_corner_y + 2 * BORDER_THICKNESS + 5*font_config.height , &lower_font_config);


}

void load_assets(void){
    BMPImage* asset = LoadBMP24("assets/head_left.bmp");
    if (asset == NULL){
        printf("Asset head_left error");
        return;
    }
    BMPConvertPixels(asset, COLOR_WHITE,BACKGROUND_COLOR);

    snake_textures[0] = CreateTextureFromBMP(asset);

    freeBMPImage(asset);
    /*==========================================*/
    asset = LoadBMP24("assets/head_right.bmp");
    if (asset == NULL){
        printf("Asset head_right error");
        return;
    }
    BMPConvertPixels(asset, COLOR_WHITE,BACKGROUND_COLOR);

    snake_textures[1] = CreateTextureFromBMP(asset);

    freeBMPImage(asset);
     /*==========================================*/
    asset = LoadBMP24("assets/head_up.bmp");
    if (asset == NULL){
        printf("Asset head_up error");
        return;
    }
    BMPConvertPixels(asset, COLOR_WHITE,BACKGROUND_COLOR);

    snake_textures[2] = CreateTextureFromBMP(asset);

    freeBMPImage(asset);
     /*==========================================*/
    asset = LoadBMP24("assets/head_down.bmp");
    if (asset == NULL){
        printf("Asset head_down error");
        return;
    }
    BMPConvertPixels(asset, COLOR_WHITE,BACKGROUND_COLOR);

    snake_textures[3] = CreateTextureFromBMP(asset);

    freeBMPImage(asset);

    /*==========================================*/

    asset = LoadBMP24("assets/tail_left.bmp");
    if (asset == NULL){
        printf("Asset tail_left error");
        return;
    }
    BMPConvertPixels(asset, COLOR_WHITE,BACKGROUND_COLOR);

    snake_textures[4] = CreateTextureFromBMP(asset);

    freeBMPImage(asset);
    /*==========================================*/

    asset = LoadBMP24("assets/tail_right.bmp");
    if (asset == NULL){
        printf("Asset tail_right error");
        return;
    }
    BMPConvertPixels(asset, COLOR_WHITE,BACKGROUND_COLOR);

    snake_textures[5] = CreateTextureFromBMP(asset);

    freeBMPImage(asset);
    /*==========================================*/

    asset = LoadBMP24("assets/tail_up.bmp");
    if (asset == NULL){
        printf("Asset tail_up error");
        return;
    }
    BMPConvertPixels(asset, COLOR_WHITE,BACKGROUND_COLOR);

    snake_textures[6] = CreateTextureFromBMP(asset);

    freeBMPImage(asset);
    /*==========================================*/

    asset = LoadBMP24("assets/tail_down.bmp");
    if (asset == NULL){
        printf("Asset tail_down error");
        return;
    }
    BMPConvertPixels(asset, COLOR_WHITE,BACKGROUND_COLOR);

    snake_textures[7] = CreateTextureFromBMP(asset);

    freeBMPImage(asset);

    /*==========================================*/

    asset = LoadBMP24("assets/body_horizontal.bmp");
    if (asset == NULL){
        printf("Asset body_horizontal error");
        return;
    }
    BMPConvertPixels(asset, COLOR_WHITE,BACKGROUND_COLOR);

    snake_textures[8] = CreateTextureFromBMP(asset);

    freeBMPImage(asset);

    /*==========================================*/

    asset = LoadBMP24("assets/body_vertical.bmp");
    if (asset == NULL){
        printf("Asset body_vertical error");
        return;
    }

    BMPConvertPixels(asset, COLOR_WHITE,BACKGROUND_COLOR);

    snake_textures[9] = CreateTextureFromBMP(asset);

    freeBMPImage(asset);
    /*==========================================*/

    asset = LoadBMP24("assets/body_topleft.bmp");
    if (asset == NULL){
        printf("Asset body_topleft error");
        return;
    }

    BMPConvertPixels(asset, COLOR_WHITE,BACKGROUND_COLOR);

    snake_textures[10] = CreateTextureFromBMP(asset);

    freeBMPImage(asset);
    /*==========================================*/

    asset = LoadBMP24("assets/body_topright.bmp");
    if (asset == NULL){
        printf("Asset body_topright error");
        return;
    }

    BMPConvertPixels(asset, COLOR_WHITE,BACKGROUND_COLOR);

    snake_textures[11] = CreateTextureFromBMP(asset);

    freeBMPImage(asset);
    /*==========================================*/

    asset = LoadBMP24("assets/body_bottomleft.bmp");
    if (asset == NULL){
        printf("Asset body_bottomleft error");
        return;
    }

    BMPConvertPixels(asset, COLOR_WHITE,BACKGROUND_COLOR);

    snake_textures[12] = CreateTextureFromBMP(asset);

    freeBMPImage(asset);
    /*==========================================*/

    asset = LoadBMP24("assets/body_bottomright.bmp");
    if (asset == NULL){
        printf("Asset body_bottomright error");
        return;
    }

    BMPConvertPixels(asset, COLOR_WHITE,BACKGROUND_COLOR);

    snake_textures[13] = CreateTextureFromBMP(asset);

    freeBMPImage(asset);
}


void draw_map_border(void){

    int border_left_corner_x = SCREEN_WIDTH/2 - (MAP_WIDTH/2)*PIXEL_SIZE -BORDER_THICKNESS;
    int border_left_corner_y = SCREEN_HEIGHT/2 - (MAP_HEIGHT/2)*PIXEL_SIZE - BORDER_THICKNESS;

    draw_rectangle_frame(border_left_corner_x,border_left_corner_y,MAP_WIDTH*PIXEL_SIZE + 2*BORDER_THICKNESS,MAP_HEIGHT*PIXEL_SIZE +2*BORDER_THICKNESS,BORDER_THICKNESS,(color){0.902, 0.722, 0.384},BACKGROUND_COLOR);

}


void draw_snake(struct snake new_snake, GLuint apple_texture){

    struct point pivot = {SCREEN_WIDTH / 2 - (MAP_WIDTH / 2) * PIXEL_SIZE, SCREEN_HEIGHT / 2 - (MAP_HEIGHT / 2) * PIXEL_SIZE};

    RenderTexturedQuad(apple_texture, pivot.x + new_snake.apple.x * PIXEL_SIZE, pivot.y + new_snake.apple.y * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE);

    for (uint32_t i = 0; i < new_snake.lenght; i++){
        int x_coord = pivot.x + new_snake.snake_array[i].x * PIXEL_SIZE;
        int y_coord = pivot.y + new_snake.snake_array[i].y * PIXEL_SIZE;
        if (i == 0){
            update_head_relation(&new_snake);
            if (new_snake.head_orientation == LEFT){
                RenderTexturedQuad(snake_textures[0], x_coord, y_coord , PIXEL_SIZE, PIXEL_SIZE);
            }
            if (new_snake.head_orientation == RIGHT){
                RenderTexturedQuad(snake_textures[1], x_coord, y_coord , PIXEL_SIZE, PIXEL_SIZE);
            }
            if (new_snake.head_orientation == UP){
               RenderTexturedQuad(snake_textures[2], x_coord, y_coord , PIXEL_SIZE, PIXEL_SIZE);
            }
            if (new_snake.head_orientation == DOWN){
               RenderTexturedQuad(snake_textures[3], x_coord, y_coord , PIXEL_SIZE, PIXEL_SIZE);
            }
        }else if (i==new_snake.lenght-1){
            update_tail_relation(&new_snake);

            if (new_snake.tail_orientation == LEFT){
                RenderTexturedQuad(snake_textures[4], x_coord, y_coord , PIXEL_SIZE, PIXEL_SIZE);
            }
            if (new_snake.tail_orientation == RIGHT){
                RenderTexturedQuad(snake_textures[5], x_coord, y_coord , PIXEL_SIZE, PIXEL_SIZE);
            }
            if (new_snake.tail_orientation == UP){
                RenderTexturedQuad(snake_textures[6], x_coord, y_coord , PIXEL_SIZE, PIXEL_SIZE);
            }
            if (new_snake.tail_orientation == DOWN){
                RenderTexturedQuad(snake_textures[7], x_coord, y_coord , PIXEL_SIZE, PIXEL_SIZE);
            }
        }else
        {
            struct point previous_block = point_substract(new_snake.snake_array[i-1], new_snake.snake_array[i]);
            struct point next_block = point_substract(new_snake.snake_array[i+1], new_snake.snake_array[i]);
            if (previous_block.x == next_block.x){
                RenderTexturedQuad(snake_textures[9], x_coord, y_coord , PIXEL_SIZE, PIXEL_SIZE);
            }else if (previous_block.y == next_block.y){
                RenderTexturedQuad(snake_textures[8], x_coord, y_coord , PIXEL_SIZE, PIXEL_SIZE);
            }else{
                if ((previous_block.x == -1 && next_block.y == -1) || (previous_block.y == -1 && next_block.x == -1 )){
                    RenderTexturedQuad(snake_textures[10], x_coord, y_coord , PIXEL_SIZE, PIXEL_SIZE);
                }else if ((previous_block.x == -1 && next_block.y == 1) || (previous_block.y == 1 && next_block.x == -1 )){
                    RenderTexturedQuad(snake_textures[12], x_coord, y_coord , PIXEL_SIZE, PIXEL_SIZE);
                }else if ((previous_block.x == 1 && next_block.y == -1) || (previous_block.y == -1 && next_block.x == 1 )){
                    RenderTexturedQuad(snake_textures[11], x_coord, y_coord , PIXEL_SIZE, PIXEL_SIZE);
                }else if ((previous_block.x == 1 && next_block.y == 1) || (previous_block.y == 1 && next_block.x == 1 )){
                    RenderTexturedQuad(snake_textures[13], x_coord, y_coord , PIXEL_SIZE, PIXEL_SIZE);
                }
            }
        }
    }
}


void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods){

    struct snake* new_snake = (struct snake*)glfwGetWindowUserPointer(window);

   if (action == GLFW_PRESS) {
       game_started=1;
        if ((key == GLFW_KEY_W || key == GLFW_KEY_UP) && new_snake->dir != DOWN) {
            new_snake->next_dir = UP;
        }
        else if ((key == GLFW_KEY_S || key == GLFW_KEY_DOWN) && new_snake->dir != UP) {
            new_snake->next_dir = DOWN;
        }
        else if ((key == GLFW_KEY_A || key == GLFW_KEY_LEFT) && new_snake->dir != RIGHT) {
            new_snake->next_dir = LEFT;
        }
        else if ((key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) && new_snake->dir != LEFT) {
            new_snake->next_dir = RIGHT;
        }
        else if (key == GLFW_KEY_P) {
            new_snake->next_dir = STOP;
        }
    }
}


void init_game(struct snake* n_snake){

    srand(time(NULL));

    snake_init(n_snake);

    generate_fruit(n_snake);

}


int main(int argc, char** argv) {

    if (!glfwInit()) {
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake the game", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);


    BMPImage* bmpImage = LoadBMP24("assets/font.bmp");
    if (!bmpImage) {
        return -1;
    }

    load_assets();
    font font1_font;
    fontInit(&font1_font,bmpImage, 7,9,6,18,(size_t)' ');

    freeBMPImage(bmpImage);

    BMPImage *BmpImage_apple = LoadBMP24("assets/apple.bmp");

    BMPConvertPixels(BmpImage_apple, COLOR_BLACK, BACKGROUND_COLOR);

    GLuint apple_texture = CreateTextureFromBMP(BmpImage_apple);
    freeBMPImage(BmpImage_apple);
    struct snake new_snake;
    init_game(&new_snake);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glfwSetWindowUserPointer(window,&new_snake);
    glfwSetKeyCallback(window,key_callback);

    font_format font1_config = {
        8,16,-5,COLOR_BLACK,COLOR_WHITE
    };
    char points_string[30];
    double last_time = glfwGetTime();
    const double initial_delay = 0.1;
    const double minimum_delay = 0.001;
    struct point string_pivot = {SCREEN_WIDTH / 2 - (MAP_WIDTH / 2) * PIXEL_SIZE, SCREEN_HEIGHT / 2 - (MAP_HEIGHT / 2) * PIXEL_SIZE - font1_config.height-BORDER_THICKNESS};
    double game_start_time = 0.0;
    while (!glfwWindowShouldClose(window)) {

        glClear(GL_COLOR_BUFFER_BIT);
        if (game_started ==0){
            drawStartScreen(&font1_font);
        }else{
            if (new_snake.lenght == 3 && game_start_time == 0.0) {
                game_start_time = glfwGetTime();
            }
            draw_map_border();
            draw_snake(new_snake, apple_texture);
            sprintf(points_string, "Points: %d", new_snake.lenght*10-30);
            RenderString(&font1_font, points_string, string_pivot.x, string_pivot.y,&font1_config);
            double current_time = glfwGetTime();
            double elapsed_time = current_time - game_start_time;

            int hours = (int)(elapsed_time / 3600);
            int minutes = (int)((elapsed_time - (hours * 3600)) / 60);
            int seconds = (int)(elapsed_time - (hours * 3600) - (minutes * 60));

            char time_string[30];
            sprintf(time_string, "%02d:%02d:%02d", hours, minutes, seconds);
            RenderString(&font1_font, time_string, string_pivot.x+ MAP_WIDTH*PIXEL_SIZE- (strlen(time_string)+1)*font1_config.width - BORDER_THICKNESS,
            string_pivot.y, &font1_config);

        }

        double current_time = glfwGetTime();
        double elapsed = current_time - last_time;
        double delay = initial_delay - initial_delay * (new_snake.lenght - 1)/100; // Decrease delay based on snake length
        if (delay < minimum_delay) {
            delay = minimum_delay;
        }


        if (elapsed > delay) {
            if (new_snake.game_over) {
                last_game_points = new_snake.lenght*10-30;
                init_game(&new_snake);
                game_start_time = 0.0;
                game_started=0;
            } else {
                new_snake.dir = new_snake.next_dir;
                move_snake(&new_snake);
            }
            last_time = current_time;
        }

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    freeFont(&font1_font);

    return 0;
}
