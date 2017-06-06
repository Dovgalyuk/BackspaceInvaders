#include <avr/pgmspace.h>
#include <EEPROM.h>
#include "libgame.h"

// Arduino configuration

#if !defined(__INT_MAX__) || (__INT_MAX__ > 0xFFFF)
 #define pgm_read_pointer(addr) ((void *)pgm_read_dword(addr))
#else
 #define pgm_read_pointer(addr) ((void *)pgm_read_word(addr))
#endif

#define IR1 2
#define IG1 3
#define IB1 4
#define IR2 5
#define IG2 6
#define IB2 7
#define CLK 8  // MUST be on PORTB! (Use pin 11 on Mega)
#define LAT 10
#define OE  9
#define A   A0
#define B   A1
#define C   A2
#define D   A3


#define DATAPORT PORTD
#define DATADIR  DDRD
#define SCLKPORT PORTB

unsigned long ticks;
unsigned long last_update = 0;
uint8_t step = 0;

// RENDER VARS
int game_render_y;
uint8_t *game_render_buf;

// IMPLEMENT THIS IN YOUR GAME
void render(); // render sprites
void update(unsigned long delta); // update logic (ms since last update), enforced by ups / ticks


volatile uint8_t
    *latport, *oeport, *addraport, *addrbport, *addrcport, *addrdport,
    *btnSWport, *btnNWport, *btnSEport, *btnNEport;
uint8_t
    sclkpin, latpin, oepin, addrapin, addrbpin, addrcpin, addrdpin,
    btnSWpin, btnNWpin, btnSEpin, btnNEpin;


void game_setup(int ups)
{
    ticks     = 1000 / ups;
    sclkpin   = digitalPinToBitMask(CLK);
    latport   = portOutputRegister(digitalPinToPort(LAT));
    latpin    = digitalPinToBitMask(LAT);
    oeport    = portOutputRegister(digitalPinToPort(OE));
    oepin     = digitalPinToBitMask(OE);
    addraport = portOutputRegister(digitalPinToPort(A));
    addrapin  = digitalPinToBitMask(A);
    addrbport = portOutputRegister(digitalPinToPort(B));
    addrbpin  = digitalPinToBitMask(B);
    addrcport = portOutputRegister(digitalPinToPort(C));
    addrcpin  = digitalPinToBitMask(C); 
    addrdport = portOutputRegister(digitalPinToPort(D));
    addrdpin  = digitalPinToBitMask(D); 

    btnSWport = portInputRegister(digitalPinToPort(BUTTON_SW));
    btnSWpin  = digitalPinToBitMask(BUTTON_SW); 
    btnNWport = portInputRegister(digitalPinToPort(BUTTON_NW));
    btnNWpin  = digitalPinToBitMask(BUTTON_NW); 
    btnSEport = portInputRegister(digitalPinToPort(BUTTON_SE));
    btnSEpin  = digitalPinToBitMask(BUTTON_SE); 
    btnNEport = portInputRegister(digitalPinToPort(BUTTON_NE));
    btnNEpin  = digitalPinToBitMask(BUTTON_NE); 

    pinMode(CLK , OUTPUT);
    pinMode(LAT, OUTPUT);
    pinMode(OE, OUTPUT);
    pinMode(A, OUTPUT);
    pinMode(B, OUTPUT);
    pinMode(C, OUTPUT);
    pinMode(D, OUTPUT);

    pinMode(IR1, OUTPUT);
    pinMode(IG1, OUTPUT);
    pinMode(IB1, OUTPUT);
    pinMode(IR2, OUTPUT);
    pinMode(IG2, OUTPUT);
    pinMode(IB2, OUTPUT);

    pinMode(BUTTON_SW, INPUT);
    pinMode(BUTTON_NW, INPUT);
    pinMode(BUTTON_SE, INPUT);
    pinMode(BUTTON_NE, INPUT);

    SCLKPORT   &= ~sclkpin;
    *latport   &= ~latpin;
    *oeport    &= ~oepin;
    *addraport &= ~addrapin;
    *addrbport &= ~addrbpin;
    *addrcport &= ~addrcpin;
    *addrdport &= ~addrdpin;

    digitalWrite(IR1, HIGH);
    digitalWrite(IG1, HIGH);
    digitalWrite(IB1, HIGH);
    digitalWrite(IR2, HIGH);
    digitalWrite(IG2, HIGH);
    digitalWrite(IB2, HIGH);
}


uint8_t game_sprite_width(const struct game_sprite *s)
{
    return pgm_read_byte(&s->width);
}

uint8_t game_sprite_height(const struct game_sprite *s)
{
    return pgm_read_byte(&s->height);
}

const uint8_t *game_sprite_line(const struct game_sprite *s, int line)
{
    return pgm_read_byte(&s->lineSize) * line + (const uint8_t*)pgm_read_pointer(&s->lines);
}

void game_sprite_render_line(const struct game_sprite *s, int x, int line, uint8_t *buf, uint8_t color)
{
    uint8_t mask = 0x80;
    uint8_t width = game_sprite_width(s);
    const uint8_t *ptr = game_sprite_line(s, line);
    uint8_t spr = pgm_read_byte(ptr);
    for (uint8_t dx = 0; dx < width; ++dx)
    {
        int xx = x + dx;
        if (xx >= 0 && xx < WIDTH)
        {
            if (spr & mask)
            {
                buf[xx] = color;
            }
        }
        mask >>= 1;
        if (!mask)
        {
            mask = 0x80;
            ++ptr;
            spr = pgm_read_byte(ptr);
        }
    }
}

void game_sprite_render_impl(const struct game_sprite *s, int x, int y, uint8_t *buf, int line, uint8_t color)
{
    uint8_t height = game_sprite_height(s);
    if (line >= y && line < y + height)
    {
        game_sprite_render_line(s, x, line - y, buf, color);
    }
}

void game_draw_sprite(const struct game_sprite *s, int x, int y, uint8_t color)
{
    game_sprite_render_impl(s, x, y, game_render_buf, game_render_y, color);
}

void game_render_line(uint8_t *buf, int y)
{
    for (int i = 0; i < WIDTH; ++i)
        buf[i] = 0;

    game_render_buf = buf;
    game_render_y = y;

    // call user render()
    render();

    // fix broken LED
    if (y == 63)
        buf[63] = 0;
}

void game_run()
{
    while (1)
    {
        int tock;
        int tick;
        tock = SCLKPORT;
        tick = tock | sclkpin;

        *oeport |= oepin;
        *latport   |= latpin;
        *latport   &= ~latpin;

        if (step & 1)
            *addraport |= addrapin;
        else
            *addraport &= ~addrapin;
        if (step & 2)
            *addrbport |= addrbpin;
        else
            *addrbport &= ~addrbpin;
        if (step & 4)
            *addrcport |= addrcpin;
        else
            *addrcport &= ~addrcpin;
        if (step & 8)
            *addrdport |= addrdpin;
        else
            *addrdport &= ~addrdpin;
        step = (step + 1) & 0xf;

        *oeport &= ~oepin;

        int y1 = step; 
        int y2 = (step + 16); 
        int y3 = (step + 32); 
        int y4 = (step + 48);

        uint8_t lines[4][WIDTH];
        game_render_line(lines[0], y1);
        game_render_line(lines[1], y2);
        game_render_line(lines[2], y3);
        game_render_line(lines[3], y4);

        for (int i = 0 ; i < WIDTH ; ++i)
        {
            DATAPORT = (lines[3][i] << 5) | (lines[2][i] << 2);
            SCLKPORT = tick; // Clock lo
            SCLKPORT = tock; // Clock hi
        }

        for (int i = 0 ; i < WIDTH ; ++i)
        {
            DATAPORT = (lines[1][i] << 5) | (lines[0][i] << 2);
            SCLKPORT = tick; // Clock lo
            SCLKPORT = tock; // Clock hi
        }

        unsigned long cur_time = millis();

        if (cur_time - last_update >= ticks)
        {
            update(cur_time - last_update);
            last_update = cur_time;
        }
    }
}
