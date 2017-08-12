#ifndef ARDUINO
/*
 *  CONTROLS:
 *
 * NW - Q
 * SW - A
 * NE - P
 * SE - L
 *
 * LEFT - <LEFT>
 * RIGHT - <RIGHT>
 * UP - <UP>
 * DOWN - <DOWN>
 * A - X
 * B - Z
 * SELECT - ,
 * START - .
 *
 */




#include <SFML/Graphics.hpp>
#include <stdint.h>
#include <libgame.h>
#include <sprite.h>
#include <font.h>
#include <storage.h>

const int SCALE = 8;

sf::VertexArray *screen;
sf::RenderWindow *window;
sf::Clock running_clock;

uint16_t buttons = 0;

bool paused = false;

void render();
void update(unsigned long delta);
void prepare();

unsigned long millis()
{
    return running_clock.getElapsedTime().asMilliseconds();
}


void storage_init() {}
void storage_format() {}

void storage_read(const char *name, void *buffer, size_t size){};
bool storage_write(const char *name, const void *buffer, size_t size){return 0;};

uint8_t game_sprite_width(const struct game_sprite *s)
{
    return s->width;
}

uint8_t game_sprite_height(const struct game_sprite *s)
{
    return s->height;
}

void game_draw_pixel(int x, int y, uint8_t c)
{
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
        return;
    int pos = 4 * (x * WIDTH + y);
    sf::Color color(((c >> 4) & 3) * 85, ((c >> 2) & 3) * 85, (c & 3) * 85);
    (*screen)[pos + 0].color = color;
    (*screen)[pos + 1].color = color;
    (*screen)[pos + 2].color = color;
    (*screen)[pos + 3].color = color;
}

void game_draw_vline(int x, int y1, int y2, uint8_t color)
{
    for (int i = y1 ; i <= y2 ; ++i)
        game_draw_pixel(x, i, color);
}

void clear_screen()
{
    for (int x = 0; x < HEIGHT; ++x)
    {
        for (int y = 0; y < WIDTH; ++y)
        {
            game_draw_pixel(x, y, BLACK);
        }
    }
}

bool game_is_button_pressed(uint8_t button)
{
    return (buttons >> button) & 1;
}

bool game_is_any_button_pressed(uint16_t bitmask)
{
    return buttons & bitmask;
}

void set_pressed(uint8_t button, bool pressed)
{
    if (pressed)
    {
        buttons |= (1 << button);
    }
    else
    {
        buttons &= ~(1 << button);
    }
}

void game_setup()
{
    screen = new sf::VertexArray(sf::Quads, WIDTH * HEIGHT * 4);
    for (int x = 0; x < WIDTH; ++x)
    {
        for (int y = 0; y < HEIGHT; ++y)
        {
            int pos = 4 * (x * HEIGHT + y);
            (*screen)[pos + 0].position = sf::Vector2f(x * SCALE, y * SCALE);
            (*screen)[pos + 1].position = sf::Vector2f((x + 1) * SCALE, y * SCALE);
            (*screen)[pos + 2].position = sf::Vector2f((x + 1) * SCALE, (y + 1) * SCALE);
            (*screen)[pos + 3].position = sf::Vector2f(x * SCALE, (y + 1) * SCALE);
            game_draw_pixel(x, y, BLACK);
        }
    }
    window = new sf::RenderWindow(sf::VideoMode(SCALE * WIDTH, SCALE * HEIGHT), "emulator", sf::Style::Close);
}

void game_set_ups(int ups)
{
    if (ups)
        window->setFramerateLimit(ups);
    else
        window->setFramerateLimit(30);
}

void game_draw_sprite(const struct game_sprite *s, int x, int y, uint8_t color)
{
    for (int dx = 0; dx < s->width; ++dx)
    {
        for (int dy = 0; dy < s->height; ++dy)
        {
            int xx = x + dx;
            int yy = y + dy;
            if (xx < 0 || xx >= WIDTH || yy < 0 || yy >= HEIGHT)
                continue;
            int byte = s->lineSize * dy + dx / 8;
            int bit = 7 - dx % 8;
            if ((s->lines[byte] >> bit) & 1)
                game_draw_pixel(xx, yy, color);
        }
    }
}

void game_draw_text(const uint8_t *s, int x, int y, uint8_t color)
{
    int xx = x;
    int yy = y;
    for (const uint8_t *c = s; *c; ++c)
    {
        game_draw_char(*c, xx, yy, color);
        xx += FONT_WIDTH + 1;
    }
}

void game_draw_char(uint8_t c, int x, int y, uint8_t color)
{
    int pos = (int)c * FONT_HEIGHT;
    for (int i = 0; i < FONT_WIDTH; ++i)
    {
        for (int j = 0; j < FONT_HEIGHT; ++j)
        {
            uint8_t d = font_data[pos + j];
            if ((d >> (FONT_WIDTH - 1 - i)) & 1)
                game_draw_pixel(x + i, y + j, color);
        }
    }
}

int main()
{
    prepare();
    running_clock.restart();
    sf::Clock elapsed;
    while (window->isOpen())
    {
        elapsed.restart();
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window->close();
            }
            if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
            {
                bool status = event.type == sf::Event::KeyPressed;
                if (event.key.code == sf::Keyboard::Q) set_pressed(BUTTON_NW, status);
                if (event.key.code == sf::Keyboard::A) set_pressed(BUTTON_SW, status);
                if (event.key.code == sf::Keyboard::P) set_pressed(BUTTON_NE, status);
                if (event.key.code == sf::Keyboard::L) set_pressed(BUTTON_SE, status);

                if (event.key.code == sf::Keyboard::Left) set_pressed(BUTTON_LEFT, status);
                if (event.key.code == sf::Keyboard::Right) set_pressed(BUTTON_RIGHT, status);
                if (event.key.code == sf::Keyboard::Up) set_pressed(BUTTON_UP, status);
                if (event.key.code == sf::Keyboard::Down) set_pressed(BUTTON_DOWN, status);
                if (event.key.code == sf::Keyboard::X) set_pressed(BUTTON_A, status);
                if (event.key.code == sf::Keyboard::Z) set_pressed(BUTTON_B, status);
                if (event.key.code == sf::Keyboard::Comma) set_pressed(BUTTON_SELECT, status);
                if (event.key.code == sf::Keyboard::Period) set_pressed(BUTTON_START, status);
                
                if (status && event.key.code == sf::Keyboard::Space) paused = !paused;
                if (status && event.key.code == sf::Keyboard::Escape) window->close();
            }
        }

        window->clear(sf::Color::Black);
        window->draw(*screen);
        window->display();

        if (!paused)
        {
            update(elapsed.getElapsedTime().asMilliseconds());
            clear_screen();
            render();
        }
    }

    return 0;
}
#endif
