#include "libgame.h"
#include "sprite.h"

#define MAXLEN 64

#define LEFT BUTTON_NE
#define RIGHT BUTTON_SE

#define GAMEOVER_X 16
#define GAMEOVER_Y 24

const uint8_t gameOverLines[] DATA = {
    0x3e, 0x38, 0xc6, 0xfe,
    0x60, 0x6c, 0xee, 0xc0,
    0xc0, 0xc6, 0xfe, 0xc0,
    0xce, 0xc6, 0xfe, 0xfc,
    0xc6, 0xfe, 0xd6, 0xc0,  
    0x66, 0xc6, 0xc6, 0xc0,
    0x3e, 0xc6, 0xc6, 0xfe,
    0x00, 0x00, 0x00, 0x00,
    0x7c, 0xc6, 0xfe, 0xfc,
    0xc6, 0xc6, 0xc0, 0xc6,
    0xc6, 0xc6, 0xc0, 0xc6,
    0xc6, 0xc6, 0xfc, 0xce,
    0xc6, 0x6c, 0xc0, 0xf8,
    0xc6, 0x38, 0xc0, 0xdc,
    0x7c, 0x10, 0xfe, 0xce
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
    uint8_t snakeLen;
    uint8_t velX;
    uint8_t velY;
    uint8_t foodX;
    uint8_t foodY;
    bool leftPressed;
    bool rightPressed;
    bool half;
    uint8_t collision[128];

};
static SnakeData* data;

void generateFood()
{
    while (1)
    {
        bool ok = true;
        data->foodX = rand() % 32;
        data->foodY = rand() % 32;
        for (int i = 0; i < data->snakeLen; ++i)
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
    data->snakeLen = 3;
    data->snakeX[0] = 17;
    data->snakeY[0] = 16;
    data->snakeX[1] = 16;
    data->snakeY[1] = 16;
    data->snakeX[2] = 15;
    data->snakeY[2] = 16;
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
    for (int i = 0; i < data->snakeLen; ++i)
    {
        int x = data->snakeX[i] * 2 + WIDTH;
        int y = data->snakeY[i] * 2 + HEIGHT;
        if (data->half && i == data->snakeLen - 1)
        {
            x += velsign((data->snakeX[i - 1] - data->snakeX[i] + 64) % 32);
            y += velsign((data->snakeY[i - 1] - data->snakeY[i] + 64) % 32);
            game_draw_pixel(x % 64, y % 64, GREEN);
            game_draw_pixel((x + 1) % 64, y % 64, GREEN);
            game_draw_pixel(x % 64, (y + 1) % 64, GREEN);
            game_draw_pixel((x + 1) % 64, (y + 1) % 64, GREEN);
            continue;
        }
        game_draw_pixel(x % 64, y % 64, GREEN);
        game_draw_pixel((x + 1) % 64, y % 64, GREEN);
        game_draw_pixel(x % 64, (y + 1) % 64, GREEN);
        game_draw_pixel((x + 1) % 64, (y + 1) % 64, GREEN);
        if (data->half && i == 0)
        {
            x += data->velX;
            y += data->velY;
            game_draw_pixel(x % 64, y % 64, GREEN);
            game_draw_pixel((x + 1) % 64, y % 64, GREEN);
            game_draw_pixel(x % 64, (y + 1) % 64, GREEN);
            game_draw_pixel((x + 1) % 64, (y + 1) % 64, GREEN);
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
        for (int i = 0; i < 128; ++i)
            data->collision[i] = 0;
        for (int i = 0; i < data->snakeLen; ++i)
        {
            int bit = data->snakeX[i] * 32 + data->snakeY[i];
            data->collision[bit / 8] |= (1 << (bit % 8));
        }
        // move snake forward
        for (int i = data->snakeLen; i >= 1; --i)
        {
            data->snakeX[i] = data->snakeX[i - 1];
            data->snakeY[i] = data->snakeY[i - 1];
        }
        data->snakeX[0] = (data->snakeX[0] + data->velX) % 32;
        data->snakeY[0] = (data->snakeY[0] + data->velY) % 32;
        if (data->snakeX[0] == data->foodX && data->snakeY[0] == data->foodY)
        {
            if (data->snakeLen < MAXLEN)
            {
                data->snakeLen++;
            }
            generateFood();
        }
        for (int i = 0; i < 128; ++i)
            data->collision[i] = 0;
        for (int i = 0; i < data->snakeLen; ++i)
        {
            int bit = data->snakeX[i] * 32 + data->snakeY[i];
            if (data->collision[bit / 8] & (1 << (bit % 8)))
            {
                // game over
                data->phase = PHASE_GAMEOVER;
            }
            data->collision[bit / 8] |= (1 << (bit % 8));
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
    "Змейка",
    Snake_prepare,
    Snake_render,
    Snake_update,
    2,
    sizeof(SnakeData),
    (void**)(&data)
};
