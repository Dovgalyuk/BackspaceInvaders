#include "libgame.h"

#define _OUTPUT_INSTANCES
#include "games.h"
#undef _OUTPUT_INSTANCES

#define AVAIL_SPACE 1400

uint8_t N_GAMES;

uint8_t sel = 0;

uint8_t memory[AVAIL_SPACE];

unsigned long cur_time = 0;

unsigned long btn_timeout = 0;
#define BUTTON_DELAY 200

bool game_running = false;
bool btn_pressed = false;
game_instance* ptr;
void prepare()
{
    N_GAMES = sizeof(instances) / sizeof(game_instance);
    game_setup(30);
}

void update(unsigned long delta)
{
    cur_time += delta;
    if (!game_running)
    {
        if (cur_time < btn_timeout) return;
        if ((game_is_button_pressed(BUTTON_SE)) && sel < (N_GAMES - 1) && instances[sel + 1].name)
        {
            sel++;
            btn_timeout = cur_time + BUTTON_DELAY;
        }
        if ((game_is_button_pressed(BUTTON_NE)) && sel > 0 && instances[sel - 1].name)
        {
            sel--;
            btn_timeout = cur_time + BUTTON_DELAY;
        }
        if (game_is_button_pressed(BUTTON_SW)) btn_pressed = true;
        if (!game_is_button_pressed(BUTTON_SW) && btn_pressed)
        {
            // run game
            ptr = instances + sel;
            game_running = true;
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
    if (!game_running)
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
