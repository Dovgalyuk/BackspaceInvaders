#ifndef LIBGAME_H
#define LIBGAME_H

#ifdef EMULATED
#define PROGMEM 
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <stdint.h>
#include <string.h>
using namespace std;
#else
#include <Arduino.h>
#include <avr/pgmspace.h>
#endif

#ifdef EMULATED
#define pgm_read_byte(addr) (*(addr))
#define pgm_read_word(addr) (*(addr))
#define pgm_read_dword(addr) (*(addr))
#define pgm_read_byte_near(addr) (*((uint8_t*)(addr)))
#define pgm_read_word_near(addr) (*((uint16_t*)(addr)))
#define pgm_read_dword_near(addr) (*((uint32_t*)(addr)))
#define pgm_read_pointer(addr) (*(addr))
#else
#if !defined(__INT_MAX__) || (__INT_MAX__ > 0xFFFF)
#define pgm_read_pointer(addr) ((void *)pgm_read_dword(addr))
#else
#define pgm_read_pointer(addr) ((void *)pgm_read_word(addr))
#endif
#endif

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
