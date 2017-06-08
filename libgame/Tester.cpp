#include "libgame.h"
#include "games.h"

// 70 symbols per screen

struct TesterData
{
    uint8_t symb;
    unsigned char buf[8];
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
    sprintf((char*)data->buf, "Page %d", data->symb / 70 + 1);
}

void Tester_render()
{
    int i = 0;
    for (unsigned char c = data->symb; (c < data->symb + 70) && (c < 255); ++c)
    {
        int x = (i % 10) * 6;
        int y = (i / 10 + 1) * 8;
        game_draw_char(c, x, y, WHITE);
        i++;
    }
    game_draw_text(data->buf, 0, 0, RED);
}

void Tester_update(unsigned long delta)
{
    data->cur_time += delta;
    if (data->cur_time - data->last_upd >= 5000)
    {
        data->last_upd = data->cur_time;
        if (data->symb >= 186)
            data->symb = 0;
        else
            data->symb += 70;
        sprintf((char*)data->buf, "Page %d", data->symb / 70 + 1);
    }
}

game_instance Tester = {
    "Tester",
    Tester_prepare,
    Tester_render,
    Tester_update,
    2,
    sizeof(TesterData),
    (void**)(&data)
};
