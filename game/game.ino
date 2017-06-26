#pragma GCC optimize ("-O3")

#include "libgame.h"
#include "font.h"
#include "sprite.h"
#include "storage.h"

// Arduino configuration

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

// Button input
#define CLOCK 11
#define LATCHB 12
#define LATCHJ 13
#define DATA A4
#define BUTTONS 16 // total buttons

uint16_t buttons; // buttons currently pressed

// THIS CAN DEGRADE PERFORMANCE 
#define COLOR_6BIT 0

#define DATAPORT PORTD
#define DATADIR  DDRD
#define SCLKPORT PORTB

static unsigned long ticks;
static unsigned long last_update = 0;
static uint8_t step = 0;

// RENDER VARS
static int game_render_y;
static uint8_t *game_render_buf;
#if defined(COLOR_6BIT) && COLOR_6BIT
static uint8_t color_channel = 0;
#endif

// IMPLEMENT THIS IN YOUR GAME
void render(); // render sprites
void update(unsigned long delta); // update logic (ms since last update), enforced by ups / ticks
void prepare(); // prepare execution

static volatile uint8_t
    *latport, *oeport, *addraport, *addrbport, *addrcport, *addrdport,
    *clockport, *latchbport, *latchjport, *dataport;
static uint8_t
    sclkpin, latpin, oepin, addrapin, addrbpin, addrcpin, addrdpin,
    clockpin, latchbpin, latchjpin, datapin;

void game_set_ups(int ups)
{
    if (ups)
        ticks = 1000 / ups;
    else
        ticks = 1;
}

void game_setup()
{
    buttons = 0;
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

    clockport = portOutputRegister(digitalPinToPort(CLOCK));
    clockpin  = digitalPinToBitMask(CLOCK); 
    latchbport = portOutputRegister(digitalPinToPort(LATCHB));
    latchbpin  = digitalPinToBitMask(LATCHB); 
    latchjport = portOutputRegister(digitalPinToPort(LATCHJ));
    latchjpin  = digitalPinToBitMask(LATCHJ); 
    dataport = portInputRegister(digitalPinToPort(DATA));
    datapin  = digitalPinToBitMask(DATA);

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

    pinMode(CLOCK, OUTPUT);
    pinMode(LATCHB, OUTPUT);
    pinMode(LATCHJ, OUTPUT);
    pinMode(DATA, INPUT);

    SCLKPORT   |= sclkpin;
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
    
    digitalWrite(CLOCK, HIGH);
    digitalWrite(LATCHB, HIGH);
    digitalWrite(LATCHJ, LOW);
}

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
                game_render_buf[xx + ((ry & 0xf0) << 2)] = color;
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
    uint8_t ry = game_render_y + (y & 0xf0);
    if (ry < y) ry += 0x10;
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
    if (game_render_y == (y & 0xf))
    {
        game_render_buf[x + ((y & 0xf0) << 2)] = game_make_color(color);
    }
}

void game_draw_text(const uint8_t *s, int x, int y, uint8_t color)
{
    if (game_render_y < (y & 0xf) || game_render_y >= (y & 0xf) + FONT_HEIGHT)
        return;
    for (const uint8_t *c = s; *c; ++c)
    {
        game_draw_char(*c, x, y, color);
        x += FONT_WIDTH + 1;
    }
}

void game_draw_char(uint8_t c, int x, int y, uint8_t color)
{
    if (game_render_y < (y & 0xf) || game_render_y >= (y & 0xf) + FONT_HEIGHT)
        return;
    int pos = (int)c * FONT_HEIGHT + (game_render_y - (y & 0xf));
    uint8_t d = pgm_read_byte_near(font_data + pos);
    for (int i = 0; i < FONT_WIDTH; ++i)
    {
        if ((d >> (FONT_WIDTH - 1 - i)) & 1)
        {
            game_render_buf[x + i + ((y & 0xf0) << 2)] = game_make_color(color);
        }
    }
}

bool game_is_button_pressed(uint8_t button)
{
    return (buttons >> button) & 1;
}

bool game_is_any_button_pressed(uint16_t bitmask)
{
    return (buttons & bitmask);
}

void game_render_line(uint8_t *buf, int y)
{
    for (int i = 0; i < WIDTH * 4; ++i)
        buf[i] = 0;

    game_render_buf = buf;
    game_render_y = y;

    // call user render()
    render();

    if (y == WIDTH / 4 - 1) // fix broken led
        buf[WIDTH * 4 - 1] = 0;
}

void setup()
{
    prepare();
}

void loop()
{
    int tock;
    int tick;
    tock = SCLKPORT;
    tick = tock & ~sclkpin;

    uint8_t lines[4 * WIDTH];
    game_render_line((uint8_t*)lines, step);

    uint8_t *line1 = &lines[3 * WIDTH];
    uint8_t *line2 = &lines[2 * WIDTH];
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

    *latport |= latpin;
    *latport &= ~latpin;
    *oeport &= ~oepin;
    
    unsigned long cur_time = millis();

    if ((cur_time - last_update >= ticks) && step == 0)
    {
        // update button state
        *latchjport |= latchjpin;
        *latchjport &= ~latchjpin;
        *latchbport &= ~latchbpin;
        *latchbport |= latchbpin;
        for (uint8_t button = 0; button < BUTTONS; ++button)
        {
            *clockport &= ~clockpin;
            *clockport |= clockpin;
            if (*dataport & datapin)
            {
                buttons &= ~(1 << button);
            }
            else
            {
                buttons |= (1 << button);
            }
        }
        update(cur_time - last_update);
        last_update = cur_time;
    }

    step = (step + 1) & 0xf;
    #if defined(COLOR_6BIT) && COLOR_6BIT
    if (step == 0)
        color_channel = (color_channel + 1) % 3;
    #endif


}
