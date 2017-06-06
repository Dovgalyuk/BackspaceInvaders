#ifndef LIBGAME_H
#define LIBGAME_H

#ifdef EMULATED
#define DATA
#include <cstdlib>
#ifndef min
#define min(a,b) (((a)<(b))?(a):(b))
#endif
#ifndef max
#define max(a,b) (((a)>(b))?(a):(b))
#endif
using namespace std;
#else
#ifndef min
#define min(a,b) (((a)<(b))?(a):(b))
#endif
#ifndef max
#define max(a,b) (((a)>(b))?(a):(b))
#endif
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
#define BUTTON_NW 18
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

bool game_is_button_pressed(uint8_t button);

void game_setup(int ups);
void game_run();

int get_rand();

#endif 
