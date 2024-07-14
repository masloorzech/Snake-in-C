#include "snake.h"
#include <stdio.h>



struct point point_substract(struct point f, struct point s){
    return (struct point) {f.x - s.x, f.y - s.y};
}


void snake_init(struct snake* n_snake ){

    if (n_snake == NULL){
        printf("ERROR: n_snake == NULL in snake_init");
        return;
    }
    struct point begin;
    begin.x = (uint16_t) MAP_WIDTH/2+1;
    begin.y =MAP_HEIGHT/2;
    n_snake->snake_array[0] = begin;
    n_snake->snake_array[1] = (struct point) {begin.x-1, begin.y};
    n_snake->snake_array[2] = (struct point) {begin.x-2, begin.y};
    n_snake->head_orientation = STOP;
    n_snake->lenght =3;
    n_snake->dir=STOP;
    n_snake->next_dir =STOP;
    n_snake->game_over = 0;
    n_snake->extend=0;
}

void generate_fruit(struct snake* n_snake){
    if (n_snake==NULL){
        printf("ERROR: n_snake = NULL in generate_fruit");
        return;
    }
    uint8_t flag =0;
    while (1)
    {
        flag=0;
        n_snake->apple.x = 1+rand()%(MAP_WIDTH-1);
        n_snake->apple.y = 1+rand()%(MAP_HEIGHT-1);
        for (int i = 0; i < n_snake->lenght; i++)
        {
            if(n_snake->apple.x == n_snake->snake_array[i].x && n_snake->apple.y == n_snake->snake_array[i].y){
                flag =1;
            }
        }
        if (flag==0){
            return;
        }
    }
}

void update_head_relation(struct snake* new_snake){

    if (new_snake == NULL){
        printf("ERROR: new_sanke == NULL in update_head_relation");
        return;
    }

    struct point head_relation =  point_substract (new_snake->snake_array[1], new_snake->snake_array[0]);

    if (head_relation.x == 1 && head_relation.y ==0 ){
        new_snake->head_orientation = LEFT;
    }else
    if (head_relation.x == -1 && head_relation.y ==0 ){
            new_snake->head_orientation = RIGHT;
    }else
    if (head_relation.x == 0 && head_relation.y ==1 ){
            new_snake->head_orientation = UP;
    }else
    if (head_relation.x == 0 && head_relation.y ==-1 ){
            new_snake->head_orientation = DOWN;
    }else{
        new_snake->head_orientation = new_snake->dir;
    }
}

void update_tail_relation(struct snake* new_snake){

    if (new_snake == NULL){
        printf("ERROR: new_sanke == NULL in update_tail_relation");
        return;
    }

    struct point head_relation =  point_substract (new_snake->snake_array[new_snake->lenght-2], new_snake->snake_array[new_snake->lenght-1]);

    if (head_relation.x == 1 && head_relation.y ==0 ){
        new_snake->tail_orientation = LEFT;
    }else
    if (head_relation.x == -1 && head_relation.y ==0 ){
         new_snake->tail_orientation = RIGHT;
    }else
    if (head_relation.x == 0 && head_relation.y ==1 ){
         new_snake->tail_orientation = UP;
    }else
    if (head_relation.x == 0 && head_relation.y ==-1 ){
         new_snake->tail_orientation = DOWN;
    }else{
        new_snake->tail_orientation = LEFT;
    }
}

void print_snake_cordes(struct snake snake){
    for (int i = 0; i<snake.lenght; i++){
        printf("%d %d %d\n", i, snake.snake_array[i].x,snake.snake_array[i].y);
    }
}

void move_snake(struct snake* new_snake) {

    if (new_snake == NULL){
        printf("ERROR: new_sanke == NULL in move_sanake");
        return;
    }

    if (new_snake->dir == STOP) {
        return;
    }
    if (new_snake->extend==1){
        new_snake->lenght += 1;
        new_snake->extend=0;
    }

    for (int i = new_snake->lenght - 1; i > 0; i--) {
        new_snake->snake_array[i] = new_snake->snake_array[i - 1];
    }

    if (new_snake->dir == LEFT) {
        move_left(new_snake);
    } else if (new_snake->dir == RIGHT) {
        move_right(new_snake);
    } else if (new_snake->dir == UP) {
        move_up(new_snake);
    } else if (new_snake->dir == DOWN) {
        move_down(new_snake);
    }


    // Check if apple is eaten
    if (new_snake->snake_array[0].x == new_snake->apple.x && new_snake->snake_array[0].y == new_snake->apple.y) {
        new_snake->extend = 1;
        generate_fruit(new_snake);
    }

    // Check for self-collision
    for (int i = 1; i < new_snake->lenght-1; i++) {
        if (new_snake->snake_array[0].x == new_snake->snake_array[i].x && new_snake->snake_array[0].y == new_snake->snake_array[i].y) {
            new_snake->game_over = 1;
            return;
        }
    }
}



void move_up(struct snake* new_snake){
    new_snake->snake_array[0].y-=1;

    if (new_snake->snake_array[0].y < 0){
        new_snake->snake_array[0].y = MAP_HEIGHT-1;
    }

}

void move_down(struct snake* new_snake){
    new_snake->snake_array[0].y+=1;

    if (new_snake->snake_array[0].y >MAP_HEIGHT-1){
        new_snake->snake_array[0].y = 0;
    }

}

void move_left(struct snake* new_snake){
    new_snake->snake_array[0].x-=1;

    if (new_snake->snake_array[0].x < 0){
        new_snake->snake_array[0].x = MAP_WIDTH-1;
    }

}
void move_right(struct snake* new_snake){
    new_snake->snake_array[0].x+=1;

    if (new_snake->snake_array[0].x > MAP_WIDTH-1){
        new_snake->snake_array[0].x = 0;
    }

}
