#ifndef LIBGAME_H
#define LIBGAME_H

// BUTTONS

#define BUTTON_SW 11
#define BUTTON_NW A4
#define BUTTON_SE 12
#define BUTTON_NE 13

#define WIDTH 64
                 
// COLORS           BGR

#define BLACK  0 // 000
#define RED    1 // 001
#define GREEN  2 // 010
#define BROWN  3 // 011
#define BLUE   4 // 100
#define PURPLE 5 // 101
#define CYAN   6 // 110
#define WHITE  7 // 111

struct game_sprite
{
    uint8_t width;
    uint8_t height;
    uint8_t lineSize;
    const uint8_t *lines;
};


uint8_t game_sprite_width(const struct game_sprite *s);
uint8_t game_sprite_height(const struct game_sprite *s);
const uint8_t *game_sprite_line(const struct game_sprite *s, int line);
void game_draw_sprite(const struct game_sprite *s, int x, int y, uint8_t color);

void game_setup(int ups);
void game_run();

#endif 
