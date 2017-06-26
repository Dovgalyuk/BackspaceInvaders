#ifndef SPRITE_H
#define SPRITE_H

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

#endif
