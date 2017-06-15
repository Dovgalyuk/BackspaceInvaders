#include "libgame.h"
#include "font.h"
#include "sprite.h"
#include "storage.h"
#include "EEPROM.h"

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
    *btnSWport, *btnNWport, *btnSEport, *btnNEport;
static uint8_t
    sclkpin, latpin, oepin, addrapin, addrbpin, addrcpin, addrdpin,
    btnSWpin, btnNWpin, btnSEpin, btnNEpin;

void game_set_ups(int ups)
{
    if (ups)
        ticks = 1000 / ups;
    else
        ticks = 1;
}

void game_setup()
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

//////////////////////////
// Storage
//////////////////////////

storage_file open_files[MAX_OPEN_FILES];
uint8_t occupied_sectors[SECTOR_SIZE];
uint8_t first_entry;

bool sector_is_free(uint8_t sector)
{
    return !((EEPROM.read(SECTOR_OFFSET + (sector >> 3)) >> (sector & 7)) & 1);
}

void storage_init()
{
    if ((EEPROM.read(0) != STORAGE_SIG0) ||
        (EEPROM.read(1) != STORAGE_SIG1) ||
        (EEPROM.read(2) != STORAGE_SIG2)) storage_format();
    first_entry = EEPROM.read(3);
    for (uint8_t i = 0; i < SECTOR_SIZE; ++i)
    {
        occupied_sectors[i] = EEPROM.read(SECTOR_OFFSET + i);
    }
    for (uint8_t i = 0; i < MAX_OPEN_FILES; ++i)
    {
        open_files[i].mode = STORAGE_CLOSED;
    }
}

void storage_flush_sectors()
{
    for (uint8_t i = 0; i < SECTOR_SIZE; ++i)
    {
        EEPROM.update(SECTOR_OFFSET + i, occupied_sectors[i]);
    }
}

void storage_format()
{
    // Write header

    EEPROM.update(0, STORAGE_SIG0);
    EEPROM.update(1, STORAGE_SIG1);
    EEPROM.update(2, STORAGE_SIG2);
    EEPROM.update(3, 0);

    // Format sector map
    EEPROM.update(SECTOR_OFFSET, 1);
    for (uint8_t i = SECTOR_OFFSET + 1; i < SECTOR_OFFSET + SECTOR_SIZE; ++i)
    {
        EEPROM.update(i, 0);
    }
}

uint8_t storage_open(const char name[FILENAME_LENGTH], uint8_t mode)
{
    uint8_t entry = first_entry;
    uint8_t prev_entry = 0;
    while (entry)
    {
        char entry_name[FILENAME_LENGTH];
        for (uint8_t x = 0; x < FILENAME_LENGTH; ++x)
            entry_name[x] = EEPROM.read(SECTOR_OFFSET + SECTOR_SIZE * entry + x);
        if (!strncmp(name, entry_name, FILENAME_LENGTH))
        {
            // this is our entry
            for (uint8_t i = 0; i < MAX_OPEN_FILES; ++i)
            {
                if (open_files[i].mode == STORAGE_CLOSED)
                {
                    open_files[i].prev_entry = prev_entry;
                    open_files[i].entry = entry;
                    open_files[i].cur_pos = 0;
                    open_files[i].cur_sector = EEPROM.read(SECTOR_OFFSET + SECTOR_SIZE * entry + FILENAME_LENGTH);
                    open_files[i].mode = mode;

                    if (mode == STORAGE_READ)
                    {
                        return i;
                    }

                    if (mode == STORAGE_WRITE)
                    {
                        uint8_t cur_data = open_files[i].cur_sector;
                        while (cur_data)
                        {
                            // mark sector as empty
                            occupied_sectors[cur_data >> 3] &= ~(1 << (cur_data & 7));
                            cur_data = EEPROM.read(SECTOR_OFFSET + SECTOR_SIZE * cur_data + SECTOR_SIZE - 1);
                        }
                        storage_flush_sectors();
                        return i;
                    }
                }
            }
            return STORAGE_FAILED;
        }
        prev_entry = entry;
        entry = EEPROM.read(SECTOR_OFFSET + SECTOR_SIZE * entry + FILENAME_LENGTH + 1);
    }
    if (mode == STORAGE_WRITE)
    {
        for (uint8_t i = 0; i < MAX_OPEN_FILES; ++i)
        {
            if (open_files[i].mode == STORAGE_CLOSED)
            {
                open_files[i].prev_entry = prev_entry;
                open_files[i].cur_pos = 0;
                open_files[i].cur_sector = 0;
                open_files[i].mode = STORAGE_WRITE;

                // now we need to find an empty sector for entry
                for (uint8_t sector = 0; sector < SECTORS; ++sector)
                {
                    if (sector_is_free(sector))
                    {
                        open_files[i].entry = sector;
                        occupied_sectors[sector >> 3] |= 1 << (sector & 7);
                        for (uint8_t x = 0; x < FILENAME_LENGTH; ++x)
                            EEPROM.update(SECTOR_OFFSET + SECTOR_SIZE * sector + x, name[x]);
                        EEPROM.update(SECTOR_OFFSET + SECTOR_SIZE * sector + FILENAME_LENGTH, 0);  // data
                        EEPROM.update(SECTOR_OFFSET + SECTOR_SIZE * sector + FILENAME_LENGTH + 1, 0);  // next
                        if (prev_entry == 0)
                            EEPROM.update(SECTOR_OFFSET - 1, sector); // prev->next 
                        else
                            EEPROM.update(SECTOR_OFFSET + SECTOR_SIZE * prev_entry + FILENAME_LENGTH + 1, sector); // prev->next 
                        storage_flush_sectors();
                        return i;
                    }
                }

                return STORAGE_FAILED;
            }
        }
    }
    return STORAGE_FAILED;
}

size_t storage_read(uint8_t sd, void* _buffer, size_t size)
{
    if ((open_files[sd].mode != STORAGE_READ) || (open_files[sd].cur_pos == STORAGE_EOF))
    {
        return 0;
    }
    uint8_t* buffer = (uint8_t*)_buffer;
    size_t r = 0;
    while (r < size)
    {
        *buffer = EEPROM.read(SECTOR_OFFSET + open_files[sd].cur_sector * SECTOR_SIZE + open_files[sd].cur_pos);
        buffer++;
        r++;
        open_files[sd].cur_pos++;
        if (open_files[sd].cur_pos >= SECTOR_SIZE - 1)
        {
            // next sector
            uint8_t ns = EEPROM.read(SECTOR_OFFSET + open_files[sd].cur_sector * SECTOR_SIZE + SECTOR_SIZE - 1);
            if (ns == 0)
            {
                open_files[sd].cur_pos = STORAGE_EOF;
                break;
            }
            else
            {
                open_files[sd].cur_pos = 0;
                open_files[sd].cur_sector = ns;
            }
        }
    }
    return r;
}

uint8_t storage_read_byte(uint8_t sd)
{
    uint8_t r = 0;
    storage_read(sd, (void*)&r, sizeof(r));
    return r;
}

uint16_t storage_read_word(uint8_t sd)
{
    uint16_t r = 0;
    storage_read(sd, (void*)&r, sizeof(r));
    return r;
}

uint32_t storage_read_dword(uint8_t sd)
{
    uint32_t r = 0;
    storage_read(sd, (void*)&r, sizeof(r));
    return r;
}

size_t storage_write(uint8_t sd, void* _buffer, size_t size)
{
    if (open_files[sd].mode != STORAGE_WRITE)
    {
        return 0;
    }
    uint8_t* buffer = (uint8_t*)_buffer;
    size_t w = 0;
    while (w < size)
    {
        if ((open_files[sd].cur_pos >= SECTOR_SIZE - 1) || (open_files[sd].cur_sector == 0))
        {
            // allocate next sector
            bool allocated = false;
            for (uint8_t sector = 0; sector < SECTORS; ++sector)
            {
                if (sector_is_free(sector))
                {
                    allocated = true;
                    if (open_files[sd].cur_sector == 0)
                        EEPROM.update(SECTOR_OFFSET + open_files[sd].entry * SECTOR_SIZE + FILENAME_LENGTH, sector);
                    else
                        EEPROM.update(SECTOR_OFFSET + open_files[sd].cur_sector * SECTOR_SIZE + SECTOR_SIZE - 1, sector);
                    open_files[sd].cur_pos = 0;
                    open_files[sd].cur_sector = sector;
                    occupied_sectors[sector >> 3] |= 1 << (sector & 7);
                    storage_flush_sectors();
                    break;
                }
            }
            if (!allocated) break;
        }
        EEPROM.update(SECTOR_OFFSET + open_files[sd].cur_sector * SECTOR_SIZE + open_files[sd].cur_pos, *buffer);
        buffer++;
        w++;
        open_files[sd].cur_pos++;
    }
    return w;
}

void storage_write_byte(uint8_t sd, uint8_t value)
{
    storage_write(sd, (void*)&value, sizeof(value));
}

void storage_write_word(uint8_t sd, uint16_t value)
{
    storage_write(sd, (void*)&value, sizeof(value));
}

void storage_write_dword(uint8_t sd, uint32_t value)
{
    storage_write(sd, (void*)&value, sizeof(value));
}

void storage_close(uint8_t sd)
{
    open_files[sd].mode = STORAGE_CLOSED;
}

void storage_delete(uint8_t sd)
{
    uint8_t next = EEPROM.read(SECTOR_OFFSET + SECTOR_SIZE * open_files[sd].entry + FILENAME_LENGTH + 1);
    if (open_files[sd].prev_entry == 0)
        EEPROM.update(SECTOR_OFFSET - 1, next); 
    else
        EEPROM.update(SECTOR_OFFSET + SECTOR_SIZE * open_files[sd].prev_entry + FILENAME_LENGTH + 1, next);
    open_files[sd].mode = STORAGE_CLOSED;
    uint8_t entry = open_files[sd].entry;
    uint8_t cur_data = EEPROM.read(SECTOR_OFFSET + SECTOR_SIZE * entry + FILENAME_LENGTH);
    occupied_sectors[entry >> 3] &= ~(1 << (entry & 7));
    while (cur_data)
    {
        // mark sector as empty
        occupied_sectors[cur_data >> 3] &= ~(1 << (cur_data & 7));
        cur_data = EEPROM.read(SECTOR_OFFSET + SECTOR_SIZE * cur_data + SECTOR_SIZE - 1);
    }
    storage_flush_sectors();
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
    volatile uint8_t *port;
    uint8_t pin;
    switch (button)
    {
    case BUTTON_SW: pin = btnSWpin; port = btnSWport; break;
    case BUTTON_NW: pin = btnNWpin; port = btnNWport; break;
    case BUTTON_SE: pin = btnSEpin; port = btnSEport; break;
    case BUTTON_NE: pin = btnNEpin; port = btnNEport; break;
    default: return false;
    }

    return ((*port & pin) == 0);
}

void game_render_line(uint8_t *buf, int y)
{
    for (int i = 0; i < WIDTH * 4; ++i)
        buf[i] = 0;

    game_render_buf = buf;
    game_render_y = y;

    // call user render()
    render();
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
    #if defined(COLOR_6BIT) && COLOR_6BIT
    if (step == 0)
        color_channel = (color_channel + 1) % 3;
    #endif

    *oeport &= ~oepin;

    uint8_t lines[4][WIDTH];
    game_render_line((uint8_t*)lines, step);

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

    if ((cur_time - last_update >= ticks) && step == 0)
    {
        update(cur_time - last_update);
        last_update = cur_time;
    }
}
