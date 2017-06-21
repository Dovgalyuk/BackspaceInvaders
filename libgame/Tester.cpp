#include "libgame.h"
#include "font.h"

#define PER_PAGE 64
#define COLS 8
#define ROWS 8
#define TOTAL 256

#define PREV BITMASK(BUTTON_NE) | BITMASK(BUTTON_LEFT)
#define NEXT BITMASK(BUTTON_SE) | BITMASK(BUTTON_RIGHT)

struct TesterData
{
    uint8_t symb;
    bool pressed;
    unsigned long last_upd;
    unsigned long cur_time;
};
static TesterData* data;

void Tester_prepare()
{
    data->pressed = false;
    data->symb = 0;
    data->last_upd = 0;
    data->cur_time = 0;
}

unsigned char to_hex(uint8_t n)
{
    if (n < 10) return '0' + n;
    return 'A' + n - 10;
}

void Tester_render()
{
    int i = 0;
    for (unsigned char c = data->symb; i < PER_PAGE; ++c)
    {
        int x = (i % COLS + 2) * (FONT_WIDTH + 1);
        int y = (i / ROWS) * (FONT_HEIGHT + 1);
        game_draw_char(c, x, y, WHITE);
        i++;
    }
    for (uint8_t l = 0; l < ROWS; ++l)
    {
        unsigned char first = data->symb + COLS * l;
        game_draw_char(to_hex(first >> 4), 0, l * (FONT_HEIGHT + 1), RED);
        game_draw_char(to_hex(first & 0xf), FONT_WIDTH + 1, l * (FONT_HEIGHT + 1), RED);
    }
}

void Tester_update(unsigned long delta)
{
    if (!data->pressed)
    {
        if (game_is_any_button_pressed(NEXT))
        {
            data->symb = (data->symb + PER_PAGE) % TOTAL;
        }
        if (game_is_any_button_pressed(PREV))
        {
            data->symb = (data->symb - PER_PAGE + TOTAL) % TOTAL;
        }
    }
    data->pressed = game_is_any_button_pressed(NEXT | PREV);
}

game_instance Tester = {
    "Font",
    Tester_prepare,
    Tester_render,
    Tester_update,
    2,
    sizeof(TesterData),
    (void**)(&data)
};
