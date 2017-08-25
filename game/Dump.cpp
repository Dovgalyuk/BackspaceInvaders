#pragma GCC optimize ("-O3")

#ifndef EMULATED
#include "libgame.h"
#include "font.h"
#include "graphics.h"
#include "controls.h"
#include <avr/eeprom.h>

#define COLS 4
#define ROWS 8
#define PER_PAGE (COLS * ROWS)
#define TOTAL 1024

#define PREV BITMASK(BUTTON_NE) | BITMASK(BUTTON_LEFT)
#define NEXT BITMASK(BUTTON_SE) | BITMASK(BUTTON_RIGHT)

struct DumpData
{
    uint16_t addr;
    bool pressed;
    unsigned long last_upd;
    unsigned long cur_time;
};
static DumpData* data;

void Dump_prepare()
{
    data->pressed = false;
    data->addr = 0;
    data->last_upd = 0;
    data->cur_time = 0;
}

static unsigned char to_hex(uint8_t n)
{
    if (n < 10) return '0' + n;
    return 'A' + n - 10;
}

void Dump_render()
{
    int i = 0;
    uint16_t addr = data->addr;
    for (int y = 0 ; y < ROWS * (FONT_HEIGHT + 1) ; y += FONT_HEIGHT + 1)
    {
        if (game_is_drawing_lines(y, FONT_HEIGHT))
        {
            game_draw_char(to_hex(addr >> 8), 0, y, RED);
            game_draw_char(to_hex((addr >> 4) & 0xf), FONT_WIDTH, y, RED);
            game_draw_char(to_hex(addr & 0xf), 2 * FONT_WIDTH, y, RED);
            for (int x = 0 ; x < COLS * 2 * (FONT_WIDTH + 1) ; x += (FONT_WIDTH + 1) * 2, ++addr)
            {
                uint8_t c = eeprom_read_byte((uint8_t*)addr);
                game_draw_char(to_hex(c >> 4), 3 * FONT_WIDTH + 1 + x, y, WHITE);
                game_draw_char(to_hex(c & 0xf), 3 * FONT_WIDTH + 1 + x + FONT_WIDTH + 1, y, WHITE);
            }
        }
        else
        {
            addr += COLS;
        }
    }
}

void Dump_update(unsigned long delta)
{
    if (!data->pressed)
    {
        if (game_is_any_button_pressed(NEXT))
        {
            data->addr = (data->addr + PER_PAGE) % TOTAL;
        }
        if (game_is_any_button_pressed(PREV))
        {
            data->addr = (data->addr - PER_PAGE + TOTAL) % TOTAL;
        }
    }
    data->pressed = game_is_any_button_pressed(NEXT | PREV);
}

game_instance Dump = {
    "EEPROM",
    Dump_prepare,
    Dump_render,
    Dump_update,
    sizeof(DumpData),
    (void**)(&data)
};
#endif
