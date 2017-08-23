#ifndef GRAPHICS_H
#define GRAPHICS_H

// COLORS
// 
// are defined as 6-bit integer: RRGGBB

#define BLACK 0
#define BLUE 3
#define GREEN 12
#define RED 48
#define CYAN (BLUE + GREEN)
#define PURPLE (RED + BLUE)
#define YELLOW (RED + GREEN)
#define WHITE 63

// SPRITE

struct game_sprite
{
    uint8_t width;
    uint8_t height;
    uint8_t lineSize;
    const uint8_t *lines;
};

// Setup functions

void game_enable_frame_buffer();

// Sprite functions

uint8_t game_sprite_width(const struct game_sprite *s);
uint8_t game_sprite_height(const struct game_sprite *s);

// DRAWING FUNCTIONS

void game_draw_pixel(int x, int y, uint8_t color);
void game_draw_vline(int x, int y1, int y2, uint8_t color); // vertical line
void game_draw_text(const uint8_t *s, int x, int y, uint8_t color); // text is drawn with default 5x7 font
void game_draw_char(uint8_t c, int x, int y, uint8_t color); // char is drawn with default 5x7 font
void game_draw_sprite(const struct game_sprite *s, int x, int y, uint8_t color);
void game_draw_digits(uint16_t num, int len, int x, int y, uint8_t color);

bool game_is_drawing_lines(int y, int height);

// Internal functions
// Called only by libgame

void graphics_setup();
void graphics_render_frame();

#endif

