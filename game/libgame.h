#ifndef LIBGAME_H
#define LIBGAME_H

#include "config.h"

#define WIDTH  64
#define HEIGHT 64

typedef void(*game_function_prepare)();
typedef void(*game_function_render)();
typedef void(*game_function_update)(unsigned long);

struct game_instance
{
    unsigned char name[10];
    game_function_prepare prepare;
    game_function_render render;
    game_function_update update;
    uint16_t data_size;
    void** data;
};

void game_set_ups(int ups);
void game_setup();

void game_save(const void *buf, uint16_t size);
void game_load(void *buf, uint16_t size);

#endif 
