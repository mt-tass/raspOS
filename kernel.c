#include "uart.h"
#include "framebuffer.h"
#include "timer.h"

typedef struct {
    int x;
    int y;
    color c;
} snake_body;
typedef struct {
    int x;
    int y;
} food;
#define SNAKE_SIZE 10
#define MAX_LENGTH 100
#define SPEED_MS 80
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

uint32_t random(void) {
    return timer_get_ticks();
}
void spawn_food(food *f, snake_body *snake, int length) {
    f->x = (random()%(SCREEN_WIDTH/SNAKE_SIZE))*SNAKE_SIZE;
    f->y = (random()%(SCREEN_HEIGHT/SNAKE_SIZE))*SNAKE_SIZE;
    for (int i = 0; i < length; i++){
        if (f->x == snake[i].x&&f->y == snake[i].y){
            spawn_food(f, snake, length);
            return;
        }
    }
}
void kernel_main(void) {
    uart_init();
    uart_print_string("Snake Game\n");
    if (fb_init(SCREEN_WIDTH, SCREEN_HEIGHT, 32) != 0) {
        uart_print_string("ERROR: framebuffer failed!\n");
        while(1);
    }
    fb_fill_screen(COLOR_BLACK);
    snake_body snake[MAX_LENGTH];
    int snake_length = 4;
    snake[0] = (snake_body){100, 240, COLOR_RED};
    snake[1] = (snake_body){90,  240, COLOR_GREEN};
    snake[2] = (snake_body){80,  240, COLOR_CYAN};
    snake[3] = (snake_body){70,  240, COLOR_YELLOW};
    int move_x = 1;
    int move_y = 0;
    food current_food;
    spawn_food(&current_food, snake, snake_length);
    for (int i = 0; i < snake_length; i++) {
        fb_draw_rectangle(snake[i].x,snake[i].y,SNAKE_SIZE,SNAKE_SIZE,snake[i].c);
    }
    fb_draw_rectangle(current_food.x,current_food.y,SNAKE_SIZE,SNAKE_SIZE,COLOR_WHITE);
    fb_draw_string(10, 10, "Length: 4", COLOR_WHITE, COLOR_BLACK);
    delay_ms(1000);
    uart_print_string("Game start\n");
    while(1){
        int will_grow = 0;
        int new_x = snake[0].x + move_x * SNAKE_SIZE;
        int new_y = snake[0].y + move_y * SNAKE_SIZE;
        
        if (new_x == current_food.x && new_y == current_food.y){
            will_grow = 1;
            uart_print_string("New Length: ");
            uart_print_num(snake_length + 1, DECIMAL);
            uart_print_string("\n");
        }
        if (!will_grow){
            fb_draw_rectangle(snake[snake_length - 1].x,snake[snake_length - 1].y,SNAKE_SIZE, SNAKE_SIZE, COLOR_BLACK);
        }
        else{
            snake_length++;
            if (snake_length >= MAX_LENGTH) {
                snake_length = MAX_LENGTH;
            }
            spawn_food(&current_food, snake, snake_length);
            fb_draw_rectangle(current_food.x,current_food.y,SNAKE_SIZE,SNAKE_SIZE,COLOR_WHITE);
            fb_draw_rectangle(10, 10, 150, 10, COLOR_BLACK);
            fb_draw_string(10, 10, "Length: ", COLOR_WHITE, COLOR_BLACK);
            char score_str[4];
            score_str[0] = '0' + (snake_length / 10);
            score_str[1] = '0' + (snake_length % 10);
            score_str[2] = '\0';
            fb_draw_string(80, 10, score_str, COLOR_WHITE, COLOR_BLACK);
        }
        for (int i = snake_length - 1; i >= 1; i--) {
            snake[i].x = snake[i - 1].x;
            snake[i].y = snake[i - 1].y;
        }
        snake[0].x = new_x;
        snake[0].y = new_y;
        if (move_x == 1 && snake[0].x >= SCREEN_WIDTH - SNAKE_SIZE){
            move_x = 0;
            move_y = 1;
        }
        else if (move_y == 1 && snake[0].y >= SCREEN_HEIGHT - SNAKE_SIZE){
            move_x = -1;
            move_y = 0;
        }
        else if (move_x == -1 && snake[0].x <= 0){
            move_x = 0;
            move_y = -1;
        }
        else if (move_y == -1 && snake[0].y <= 0){
            move_x = 1;
            move_y = 0;
        }
        for (int i = 0; i < snake_length; i++) {
            fb_draw_rectangle(snake[i].x, snake[i].y,SNAKE_SIZE,SNAKE_SIZE,COLOR_CYAN);
        }
        delay_ms(SPEED_MS);
    }
}