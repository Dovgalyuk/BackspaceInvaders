#pragma GCC optimize ("-O3")

#include "libgame.h"
#include "storage.h"
#include "graphics.h"
#include "controls.h"
#include "random.h"

#define _OUTPUT_INSTANCES
#include "games.h"
#undef _OUTPUT_INSTANCES

#define UP BITMASK(BUTTON_NE) | BITMASK(BUTTON_UP)
#define DOWN BITMASK(BUTTON_SE) | BITMASK(BUTTON_DOWN)
#define SELECT BITMASK(BUTTON_SW) | BITMASK(BUTTON_START)

// need some space for stack and system variables
#define AVAIL_SPACE 1024

#define N_GAMES (sizeof(instances) / sizeof(game_instance))

static uint8_t sel = 0;

static uint8_t memory[AVAIL_SPACE];

static unsigned long cur_time = 0;

static unsigned long btn_timeout = 0;
#define BUTTON_DELAY 200

static bool btn_pressed = false;
static game_instance* ptr;

void update(unsigned long delta)
{
    cur_time += delta;
    if (!ptr)
    {
        if (cur_time < btn_timeout) return;
        if ((game_is_any_button_pressed(DOWN)) && sel < (N_GAMES - 1))
        {
            sel++;
            btn_timeout = cur_time + BUTTON_DELAY;
        }
        if ((game_is_any_button_pressed(UP)) && sel > 0)
        {
            sel--;
            btn_timeout = cur_time + BUTTON_DELAY;
        }
        if (game_is_any_button_pressed(SELECT)) btn_pressed = true;
        if (!game_is_any_button_pressed(SELECT) && btn_pressed)
        {
            random_setup();
            // run game
            ptr = instances + sel;
            *(ptr->data) = memory;
            ptr->prepare();
        }
    }
    else
    {
        ptr->update(delta);
    }
}

void render()
{
    if (!ptr)
    {
        for (uint8_t iter = 0; iter < N_GAMES; ++iter)
        {
            if (instances[iter].name)
            {
                game_draw_text(instances[iter].name, 0, 8 * iter, (iter == sel) ? RED : WHITE);
            }
        }
    }
    else
    {
        ptr->render();
    }
}

//////////////////////////
// Storage
//////////////////////////

void game_save(const void *buf, uint16_t size)
{
    storage_write((const char*)ptr->name, buf, size);
}

void game_load(void *buf, uint16_t size)
{
    storage_read((const char*)ptr->name, buf, size);
}

