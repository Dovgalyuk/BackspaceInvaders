#ifndef LIBGAME_H
#define LIBGAME_H

#ifdef EMULATED
#define DATA
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <stdint.h>
#include <string.h>
using namespace std;
#else
#include <Arduino.h>
#include <avr/pgmspace.h>
#define DATA PROGMEM
#endif

#ifdef EMULATED
#define pgm_read_byte(addr) (*addr)
#define pgm_read_word(addr) (*addr)
#define pgm_read_dword(addr) (*addr)
#define pgm_read_pointer(addr) (*addr)
#else
#if !defined(__INT_MAX__) || (__INT_MAX__ > 0xFFFF)
#define pgm_read_pointer(addr) ((void *)pgm_read_dword(addr))
#else
#define pgm_read_pointer(addr) ((void *)pgm_read_word(addr))
#endif
#endif

#define WIDTH  64
#define HEIGHT 64

// BUTTONS

#ifdef EMULATED
#define BUTTON_SW 1
#define BUTTON_NW 2
#define BUTTON_SE 3
#define BUTTON_NE 4
#else
#define BUTTON_SW 11
#define BUTTON_NW A4
#define BUTTON_SE 12
#define BUTTON_NE 13
#endif

// COLORS
// 
// are defined as 6-bit integer: RRGGBB

#define BLACK 0
#define BLUE 3
#define GREEN 12
#define RED 48
#define CYAN (BLUE + GREEN)
#define PURPLE (RED + BLUE)
#define BROWN (RED + GREEN)
#define WHITE 63

typedef void(*game_function_prepare)();
typedef void(*game_function_render)();
typedef void(*game_function_update)(unsigned long);

struct game_instance
{
    unsigned char name[11];
    game_function_prepare prepare;
    game_function_render render;
    game_function_update update;
    uint16_t eeprom_size;
    uint16_t data_size;
    void** data;
};

// 8 game instances leave 1983 bytes in RAM

struct game_sprite
{
    uint8_t width;
    uint8_t height;
    uint8_t lineSize;
    const uint8_t *lines;
};


uint8_t game_sprite_width(const struct game_sprite *s);
uint8_t game_sprite_height(const struct game_sprite *s);
void game_draw_sprite(const struct game_sprite *s, int x, int y, uint8_t color);
void game_draw_pixel(int x, int y, uint8_t color);
void game_draw_text(const uint8_t *s, int x, int y, uint8_t color); // text is drawn with default 5x7 font
void game_draw_char(uint8_t c, int x, int y, uint8_t color); // char is drawn with default 5x7 font

bool game_is_button_pressed(uint8_t button);

void game_setup(int ups);

#endif 
