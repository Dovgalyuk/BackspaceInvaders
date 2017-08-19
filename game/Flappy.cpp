#pragma GCC optimize ("-O3")

#include "libgame.h"
#include "graphics.h"
#include "binary.h"
#include "font.h"
#include "controls.h"

const uint8_t lines_player[] PROGMEM = {
  B00000000, B00011100, //..........xxx
  B00000000, B00111100, // .......xxxxx
  B00000000, B00110000, //xx......xxx 
  B11000111, B11111110, //xx  .xxxxxxxxx.
  B11001111, B11111111, //xx  xxxxxxxxxxx
  B11111001, B00100111, //xxxxx  x  x  xxx
  B01111001, B00100111, // .xxx  x  x  xxx
  B10001111, B11111110, // ...xxxxxxxxxxx.
  B00000111, B11111100, // ....xxxxxxxxx..
};

const game_sprite sprite_player PROGMEM = {
    16, 9, 2, lines_player
};

#define PLAYER_X 12
#define PLAYER_SKIP_Y 3
#define LANDSCAPE_MAX 24
#define BUBBLES 16

struct FlappyData
{
    bool upd_speed;
    int8_t y;
    int8_t speed;
    int8_t up[WIDTH];
    int8_t down[WIDTH];
    int8_t curr;
    uint16_t score;
    uint8_t bubblesX[BUBBLES];
    uint8_t bubblesY[BUBBLES];
    uint8_t bubblesC[BUBBLES];
    uint8_t curr_bubble;
};
static FlappyData* data;

static void Flappy_reset()
{
    data->speed = 0;
    data->y = 0;
    data->score = 0;
    for (int i = 0 ; i < WIDTH ; ++i)
        data->up[i] = data->down[i] = 1;
    for (int i = 0 ; i < BUBBLES ; ++i)
    {
        data->bubblesC[i] = BLACK;
    }
}

static void Flappy_prepare()
{
    game_set_ups(20);
    Flappy_reset();
}

static void Flappy_render()
{
    // draw bubbles
    for (int i = 0 ; i < BUBBLES ; ++i)
        game_draw_pixel(data->bubblesX[i], data->bubblesY[i], data->bubblesC[i]);

    // draw submarine
    game_draw_sprite(&sprite_player, PLAYER_X, data->y, YELLOW);

    // draw landscape
    for (int i = 0 ; i < WIDTH ; ++i)
    {
        int8_t c = (data->curr + i) % WIDTH;
        game_draw_pixel(i, data->up[c] - 1, GREEN);
        game_draw_pixel(i, HEIGHT - data->down[c], GREEN);
    }
    // draw score
    char s[6];
    sprintf(s, "%u", data->score);
    game_draw_text((uint8_t*)s, 0, 0, WHITE);
}

static void Flappy_update(unsigned long delta)
{
    data->y += data->speed;
    if (data->upd_speed)
        data->speed += 1;
    data->upd_speed = !data->upd_speed;

    // any button is pressed
    if (game_is_any_button_pressed(0xffff))
        data->speed = -2;

    // check collision
    int8_t c = data->curr;
    for (int8_t i = PLAYER_X ; i < PLAYER_X + sprite_player.height ; ++i)
    {
        if (data->y + PLAYER_SKIP_Y < data->up[(c + i) % WIDTH]
            || data->y + sprite_player.height > HEIGHT - data->down[(c + i) % WIDTH])
        {
            Flappy_reset();
        }
    }

    // update landscape
    int8_t prev = (data->curr - 1 + WIDTH) % WIDTH;
    data->up[c] = data->up[prev] + (rand() % 5 - 2);
    if (data->up[c] <= 0)
        data->up[c] = 1;
    else if (data->up[c] > LANDSCAPE_MAX)
        data->up[c] = LANDSCAPE_MAX;
    data->down[c] = data->down[prev] + (rand() % 5 - 2);
    if (data->down[c] <= 0)
        data->down[c] = 1;
    else if (data->down[c] > LANDSCAPE_MAX)
        data->down[c] = LANDSCAPE_MAX;
    data->curr = (data->curr + 1) % WIDTH;

    // update bubbles
    for (int i = 0 ; i < BUBBLES ; ++i)
    {
        --data->bubblesY[i];
        data->bubblesX[i] = data->bubblesX[i] - rand() % 2;
    }
    static const uint8_t colors[3] = {BLUE, CYAN, WHITE};
    data->bubblesC[data->curr_bubble] = colors[rand() % 3];
    data->bubblesX[data->curr_bubble] = PLAYER_X;
    data->bubblesY[data->curr_bubble] = data->y + sprite_player.height - 2;
    data->curr_bubble = (data->curr_bubble + 1) % BUBBLES;

    ++data->score;
}

game_instance Flappy = {
    "Flappy",
    Flappy_prepare,
    Flappy_render,
    Flappy_update,
    sizeof(FlappyData),
    (void**)(&data)
};
