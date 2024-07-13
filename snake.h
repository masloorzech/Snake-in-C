#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <stdio.h>
#include "color.h"
#include <GLFW/glfw3.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>


#define MAX_SNAKE_SEGMENTS 100
#define MAP_WIDTH  20
#define MAP_HEIGHT 20

typedef uint8_t boolean;

enum direction{
    STOP,
    LEFT,
    RIGHT,
    UP,
    DOWN
};


struct point{
    int16_t x;
    int16_t y;
};

struct point point_substract(struct point f, struct point s);

struct snake{
    struct point snake_array[MAX_SNAKE_SEGMENTS];
    enum direction head_orientation;
    enum direction tail_orientation;
    struct point apple;
    enum direction dir;
    enum direction next_dir;
    uint32_t lenght;
    boolean game_over;
    boolean extend;
};

void snake_init(struct snake* n_snake );
void generate_fruit(struct snake* n_snake);
void move_snake(struct snake* new_snake);
void move_up(struct snake* new_snake);
void move_down(struct snake* new_snake);
void move_left(struct snake* new_snake);
void move_right(struct snake* new_snake);
void update_head_relation(struct snake* new_snake);
void update_tail_relation(struct snake* new_snake);
