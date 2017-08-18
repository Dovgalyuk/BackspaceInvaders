#pragma GCC optimize ("-O3")

#include "libgame.h"
#include "graphics.h"
#include "font.h"

///////////////////
// Configuration
///////////////////

#define DATAPORT PORTD
#define DATADIR  DDRD
#define SCLKPORT PORTB

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
// 5-bit address bus support
//#define E   A5

#define COLOR_6BIT 0 /* Whether to use 6-bit color palette */

#ifdef E
#define ADDR_LOW    0x1f
#define ADDR_HIGH   0xe0
#define ADDR_SHIFT  1
#define BUF_LINES   2
#else
#define ADDR_LOW    0xf
#define ADDR_HIGH   0xf0
#define ADDR_SHIFT  2
#define BUF_LINES   4
#endif

static volatile uint8_t
    *latport, *oeport, *addraport, *addrbport, *addrcport, *addrdport
#ifdef E
    , addreport
#endif
;
static uint8_t
    sclkpin, latpin, oepin, addrapin, addrbpin, addrcpin, addrdpin
#ifdef E
    , addrepin
#endif
;

// RENDER VARS
static int game_render_y;
static uint8_t *game_render_buf;
#if defined(COLOR_6BIT) && COLOR_6BIT
static uint8_t color_channel;
#endif
static uint8_t step;

void render(); // render sprites
void prepare(); // prepare execution

//////////////////////////
// Colors
//////////////////////////

#if defined(COLOR_6BIT) && COLOR_6BIT
uint8_t game_make_color_channel(uint8_t channel)
{
    return (color_channel < channel);
}
#endif

uint8_t game_make_color(uint8_t color)
{
    #if defined(COLOR_6BIT) && COLOR_6BIT
    return (game_make_color_channel((color >> 4) & 3)) |
           (game_make_color_channel((color >> 2) & 3) << 1) |
           (game_make_color_channel(color & 3) << 2);
    #else
    return (color >> 5 & 1) |
           (color >> 3 & 1) << 1 |
           (color >> 1 & 1) << 2;
    #endif
}


uint8_t game_sprite_width(const struct game_sprite *s)
{
    return pgm_read_byte(&s->width);
}

uint8_t game_sprite_height(const struct game_sprite *s)
{
    return pgm_read_byte(&s->height);
}

const uint8_t *game_sprite_line(const struct game_sprite *s, uint8_t line)
{
    return pgm_read_byte(&s->lineSize) * line + (const uint8_t*)pgm_read_pointer(&s->lines);
}

void game_sprite_render_line(const struct game_sprite *s, int x, uint8_t y, int8_t color, uint8_t ry)
{
    uint8_t line = ry - y;
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
                game_render_buf[xx + ((ry & ADDR_HIGH) << ADDR_SHIFT)] = color;
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

void game_draw_sprite(const struct game_sprite *s, int x, int y, uint8_t color)
{
    uint8_t height = game_sprite_height(s);
    uint8_t ry = game_render_y + (y & ADDR_HIGH);
    if (ry < y) ry += ADDR_LOW + 1;
    if (ry >= HEIGHT) return;
    if (ry < (y + height))
    {
        game_sprite_render_line(s, x, y, game_make_color(color), ry);
    }
}

void game_draw_pixel(int x, int y, uint8_t color)
{
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
        return;
    if (game_render_y == (y & ADDR_LOW))
    {
        game_render_buf[x + ((y & ADDR_HIGH) << ADDR_SHIFT)] = game_make_color(color);
    }
}

void game_draw_vline(int x, int y1, int y2, uint8_t color)
{
    if (x < 0 || x >= WIDTH || y2 < 0 || y1 >= HEIGHT)
        return;
    for (uint8_t y = 0 ; y < (1 << ADDR_SHIFT) ; ++y)
    {
        uint8_t yy = game_render_y + y * (ADDR_LOW + 1);
        if (yy >= y1 && yy <= y2)
            game_render_buf[x + y * WIDTH] = game_make_color(color);
    }
}

void game_draw_text(const uint8_t *s, int x, int y, uint8_t color)
{
    if (game_render_y < (y & ADDR_LOW) || game_render_y >= (y & ADDR_LOW) + FONT_HEIGHT)
        return;
    for (const uint8_t *c = s; *c; ++c)
    {
        game_draw_char(*c, x, y, color);
        x += FONT_WIDTH + 1;
    }
}

void game_draw_char(uint8_t c, int x, int y, uint8_t color)
{
    if (game_render_y < (y & ADDR_LOW) || game_render_y >= (y & ADDR_LOW) + FONT_HEIGHT)
        return;
    int pos = (int)c * FONT_HEIGHT + (game_render_y - (y & ADDR_LOW));
    uint8_t d = pgm_read_byte_near(font_data + pos);
    for (int i = 0; i < FONT_WIDTH; ++i)
    {
        if ((d >> (FONT_WIDTH - 1 - i)) & 1)
        {
            game_render_buf[x + i + ((y & ADDR_HIGH) << ADDR_SHIFT)] = game_make_color(color);
        }
    }
}

void game_render_line(uint8_t *buf, int y)
{
    for (int i = 0; i < WIDTH * BUF_LINES; ++i)
        buf[i] = 0;

    game_render_buf = buf;
    game_render_y = y;

    // call user render()
    render();
#ifndef E
    if (y == WIDTH / 4 - 1) // fix broken led
        buf[WIDTH * 4 - 1] = 0;
#endif
}

void graphics_setup()
{
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
#ifdef E
    addreport = portOutputRegister(digitalPinToPort(E));
    addrepin  = digitalPinToBitMask(E); 
#endif
  
    pinMode(CLK , OUTPUT);
    pinMode(LAT, OUTPUT);
    pinMode(OE, OUTPUT);
    pinMode(A, OUTPUT);
    pinMode(B, OUTPUT);
    pinMode(C, OUTPUT);
    pinMode(D, OUTPUT);
#ifdef E
    pinMode(E, OUTPUT);
#endif

    pinMode(IR1, OUTPUT);
    pinMode(IG1, OUTPUT);
    pinMode(IB1, OUTPUT);
    pinMode(IR2, OUTPUT);
    pinMode(IG2, OUTPUT);
    pinMode(IB2, OUTPUT);

    SCLKPORT   |= sclkpin;
    *latport   &= ~latpin;
    *oeport    &= ~oepin;
    *addraport &= ~addrapin;
    *addrbport &= ~addrbpin;
    *addrcport &= ~addrcpin;
    *addrdport &= ~addrdpin;
#ifdef E
    *addreport &= ~addrepin;
#endif

    digitalWrite(IR1, HIGH);
    digitalWrite(IG1, HIGH);
    digitalWrite(IB1, HIGH);
    digitalWrite(IR2, HIGH);
    digitalWrite(IG2, HIGH);
    digitalWrite(IB2, HIGH);

  // Set up Timer1 for interrupt:
  TCCR1A  = _BV(WGM11); // Mode 14 (fast PWM), OC1A off
  TCCR1B  = _BV(WGM13) | _BV(WGM12) | _BV(CS10) | _BV(CS11); // Mode 14, div 64
  ICR1    = 0;
  TIMSK1 |= _BV(TOIE1); // Enable Timer1 interrupt
  sei();                // Enable global interrupts
}

void graphics_update()
{
    int tock;
    int tick;
    tock = SCLKPORT;
    tick = tock & ~sclkpin;

    uint8_t lines[BUF_LINES * WIDTH];
    game_render_line((uint8_t*)lines, step);

    uint8_t *line1 = &lines[(BUF_LINES - 1) * WIDTH];
    uint8_t *line2 = &lines[(BUF_LINES - 2) * WIDTH];
    uint8_t const8 = 8;
    uint8_t const4 = 4;
    uint8_t tmp;
    #define pew asm volatile(                 \
      "ld  %[tmp], %a[ptr1]+"       "\n\t"    \
      "mul %[tmp], %[c8]"           "\n\t"    \
      "ld  %[tmp], %a[ptr2]+"       "\n\t"    \
      "or  r0, %[tmp]"              "\n\t"    \
      "mul r0, %[c4]"                "\n\t"   \
      "out %[data]    , r0"          "\n\t"   \
      "out %[clk]     , %[tick]"     "\n\t"   \
      "out %[clk]     , %[tock]"     "\n"     \
      :  [ptr1] "+e" (line1),                 \
         [ptr2] "+e" (line2),                 \
         [tmp] "=r" (tmp),                    \
         [c8] "+r" (const8),                  \
         [c4] "+r" (const4)                   \
      :  [data] "I" (_SFR_IO_ADDR(DATAPORT)), \
         [clk]  "I" (_SFR_IO_ADDR(SCLKPORT)), \
         [tick] "r" (tick),                   \
         [tock] "r" (tock): "r0", "r1");

    pew pew pew pew pew pew pew pew 
    pew pew pew pew pew pew pew pew 
    pew pew pew pew pew pew pew pew 
    pew pew pew pew pew pew pew pew 
    pew pew pew pew pew pew pew pew 
    pew pew pew pew pew pew pew pew 
    pew pew pew pew pew pew pew pew 
    pew pew pew pew pew pew pew pew
#ifndef E
    line1 = &lines[1 * WIDTH];
    line2 = &lines[0 * WIDTH];

    pew pew pew pew pew pew pew pew 
    pew pew pew pew pew pew pew pew 
    pew pew pew pew pew pew pew pew 
    pew pew pew pew pew pew pew pew 
    pew pew pew pew pew pew pew pew 
    pew pew pew pew pew pew pew pew 
    pew pew pew pew pew pew pew pew 
    pew pew pew pew pew pew pew pew 
#endif
    *oeport |= oepin;

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
#ifdef E
    if (step & 16)
        *addreport |= addrepin;
    else
        *addreport &= ~addrepin;
#endif

    *latport |= latpin;
    *latport &= ~latpin;
    *oeport &= ~oepin;

    step = (step + 1) & ADDR_LOW;
#if defined(COLOR_6BIT) && COLOR_6BIT
    if (step == 0)
        color_channel = (color_channel + 1) % 3;
#endif
}

ISR(TIMER1_OVF_vect, ISR_BLOCK) { // ISR_BLOCK important
  graphics_update();              // Call refresh func
  TIFR1 |= TOV1;                  // Clear Timer1 interrupt flag
  ICR1      = 128;        // Set interval for next interrupt
  TCNT1     = 0;        // Restart interrupt timer
}

