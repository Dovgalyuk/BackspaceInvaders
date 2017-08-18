#pragma GCC optimize ("-O3")

#include "libgame.h"
#include "font.h"
#include "graphics.h"
#include "storage.h"
#include "controls.h"

void setup()
{
    graphics_setup();
    controls_setup();
    storage_init();
    game_set_ups(60);
}

void loop()
{
    controls_update();
}

