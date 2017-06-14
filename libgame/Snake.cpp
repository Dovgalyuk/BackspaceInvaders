#include "libgame.h"
#include "sprite.h"

#define MAXLEN 64

#define LEFT BUTTON_NE
#define RIGHT BUTTON_SE

#define GAMEOVER_X 16
#define GAMEOVER_Y 24

#define FIELD_WIDTH 32
#define FIELD_HEIGHT 32

const uint8_t gameOverLines[] DATA = {
    B00111110, B00111000, B11000110, B11111110,
    B01100000, B01101100, B11101110, B11000000,
    B11000000, B11000110, B11111110, B11000000,
    B11001110, B11000110, B11111110, B11111100,
    B11000110, B11111110, B11010110, B11000000,  
    B01100110, B11000110, B11000110, B11000000,
    B00111110, B11000110, B11000110, B11111110,
    B00000000, B00000000, B00000000, B00000000,
    B01111100, B11000110, B11111110, B11111100,
    B11000110, B11000110, B11000000, B11000110,
    B11000110, B11000110, B11000000, B11000110,
    B11000110, B11000110, B11111100, B11001110,
    B11000110, B01101100, B11000000, B11111000,
    B11000110, B00111000, B11000000, B11011100,
    B01111100, B00010000, B11111110, B11001110
};

const game_sprite gameOver DATA = {
    31, 15, 4, gameOverLines
};

enum Phases
{
    PHASE_GAME,
    PHASE_GAMEOVER
};

struct SnakeData
{
    uint8_t phase;
    uint8_t snakeX[MAXLEN + 1];
    uint8_t snakeY[MAXLEN + 1];
    uint8_t velX;
    uint8_t velY;
    uint8_t foodX;
    uint8_t foodY;
    bool leftPressed;
    bool rightPressed;
    bool half;
    uint8_t snakeBegin;
    uint8_t snakeEnd;

};
static SnakeData* data;

void generateFood()
{
    while (1)
    {
        bool ok = true;
        data->foodX = rand() % FIELD_WIDTH;
        data->foodY = rand() % FIELD_HEIGHT;
        for (int i = data->snakeBegin; i != data->snakeEnd; i = (i + 1) % MAXLEN)
        {
            if (data->snakeX[i] == data->foodX && data->snakeY[i] == data->foodY)
            {
                ok = false;
                break;
            }
        }
        if (ok)
            return;
    }
}

void Snake_prepare()
{
    game_set_ups(60);
    data->phase = PHASE_GAME;
    data->snakeX[0] = 17;
    data->snakeY[0] = 16;
    data->snakeX[1] = 16;
    data->snakeY[1] = 16;
    data->snakeX[2] = 15;
    data->snakeY[2] = 16;
    data->snakeBegin = 0;
    data->snakeEnd = 3;
    data->velX = 1;
    data->velY = 0;
    data->leftPressed = false;
    data->rightPressed = false;
    data->half = false;
    generateFood();
}

int velsign(int x)
{
    if (x == 0) return 0;
    if (x == 1) return 1;
    return -1;
}

void Snake_render()
{
    game_draw_pixel(data->foodX * 2, data->foodY * 2, RED);
    game_draw_pixel(data->foodX * 2 + 1, data->foodY * 2, RED);
    game_draw_pixel(data->foodX * 2, data->foodY * 2 + 1, RED);
    game_draw_pixel(data->foodX * 2 + 1, data->foodY * 2 + 1, RED);
    for (int i = data->snakeBegin; i != data->snakeEnd; i = (i + 1) % MAXLEN)
    {
        int x = data->snakeX[i] * 2 + WIDTH;
        int y = data->snakeY[i] * 2 + HEIGHT;
        if (data->half && (i + 1) % MAXLEN == data->snakeEnd)
        {
            x += velsign((data->snakeX[i - 1] - data->snakeX[i] + WIDTH) % FIELD_WIDTH);
            y += velsign((data->snakeY[i - 1] - data->snakeY[i] + HEIGHT) % FIELD_HEIGHT);
            game_draw_pixel(x % WIDTH, y % HEIGHT, GREEN);
            game_draw_pixel((x + 1) % WIDTH, y % HEIGHT, GREEN);
            game_draw_pixel(x % WIDTH, (y + 1) % HEIGHT, GREEN);
            game_draw_pixel((x + 1) % WIDTH, (y + 1) % HEIGHT, GREEN);
            continue;
        }
        game_draw_pixel(x % WIDTH, y % HEIGHT, GREEN);
        game_draw_pixel((x + 1) % WIDTH, y % HEIGHT, GREEN);
        game_draw_pixel(x % WIDTH, (y + 1) % HEIGHT, GREEN);
        game_draw_pixel((x + 1) % WIDTH, (y + 1) % HEIGHT, GREEN);
        if (data->half && i == data->snakeBegin)
        {
            x += data->velX;
            y += data->velY;
            game_draw_pixel(x % WIDTH, y % HEIGHT, GREEN);
            game_draw_pixel((x + 1) % WIDTH, y % HEIGHT, GREEN);
            game_draw_pixel(x % WIDTH, (y + 1) % HEIGHT, GREEN);
            game_draw_pixel((x + 1) % WIDTH, (y + 1) % HEIGHT, GREEN);
        }
    }

    if (data->phase == PHASE_GAMEOVER)
    {
        game_draw_sprite(&gameOver, GAMEOVER_X, GAMEOVER_Y, WHITE);
    }
}

void Snake_update(unsigned long delta)
{
    if (data->phase == PHASE_GAME) data->half = !data->half; else data->half = false;
    if (data->phase == PHASE_GAME && !data->half)
    {
        // move snake forward
        int newX = (data->snakeX[data->snakeBegin] + data->velX) % FIELD_WIDTH;
        int newY = (data->snakeY[data->snakeBegin] + data->velY) % FIELD_HEIGHT;
        data->snakeBegin = (data->snakeBegin + MAXLEN - 1) % MAXLEN;
        data->snakeEnd = (data->snakeEnd + MAXLEN - 1) % MAXLEN;
        data->snakeX[data->snakeBegin] = newX;
        data->snakeY[data->snakeBegin] = newY; 
        if (newX == data->foodX && newY == data->foodY)
        {
            if ((data->snakeEnd + 1) % MAXLEN != data->snakeBegin)
                data->snakeEnd = (data->snakeEnd + 1) % MAXLEN;
            generateFood();
        }
        for (int i = (data->snakeBegin + 1) % MAXLEN; i != data->snakeEnd; i = (i + 1) % MAXLEN)
        {
            if (newX == data->snakeX[i] && newY == data->snakeY[i])
            {
                // game over
                data->phase = PHASE_GAMEOVER;
                break;
            }
        }
    }
    if (data->phase == PHASE_GAME)
    {
        if (game_is_button_pressed(LEFT) && !data->leftPressed)
        {
            int newVelX = -data->velY;
            int newVelY = data->velX;
            data->velX = newVelX;
            data->velY = newVelY;
        }

        if (game_is_button_pressed(RIGHT) && !data->rightPressed)
        {
            int newVelX = data->velY;
            int newVelY = -data->velX;
            data->velX = newVelX;
            data->velY = newVelY;
        }
    }
    if (data->phase == PHASE_GAMEOVER)
    {
        if (game_is_button_pressed(BUTTON_NW) ||
            game_is_button_pressed(BUTTON_SW) ||
            game_is_button_pressed(BUTTON_NE) ||
            game_is_button_pressed(BUTTON_SE)) Snake_prepare(); 
    }
    data->leftPressed = game_is_button_pressed(LEFT);
    data->rightPressed = game_is_button_pressed(RIGHT);
}

game_instance Snake = {
    "Snake",
    Snake_prepare,
    Snake_render,
    Snake_update,
    2,
    sizeof(SnakeData),
    (void**)(&data)
};
