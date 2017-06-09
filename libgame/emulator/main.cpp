/*
 *  CONTROLS:
 *
 *  Q               P
 *   A             L
 *
 *
 */




#include <SFML/Graphics.hpp>
#include <stdint.h>
#include "../libgame.h"
#include "../font.h"

const int SCALE = 8;

sf::VertexArray *screen;
sf::RenderWindow *window;
sf::Clock running_clock;

bool btnSWpressed = false;
bool btnNWpressed = false;
bool btnSEpressed = false;
bool btnNEpressed = false;

bool paused = false;

void render();
void update(unsigned long delta);
void prepare();

unsigned long millis()
{
    return running_clock.getElapsedTime().asMilliseconds();
}

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
    switch (button)
    {
    case BUTTON_SW: return btnSWpressed;
    case BUTTON_NW: return btnNWpressed;
    case BUTTON_SE: return btnSEpressed;
    case BUTTON_NE: return btnNEpressed;
    }
    return false;
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
        xx += 6;
    }
}

void game_draw_char(uint8_t c, int x, int y, uint8_t color)
{
    int pos = (int)c * 7;
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 7; ++j)
        {
            uint8_t d = font_data[pos + j];
            if ((d >> i) & 1)
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
                if (event.key.code == sf::Keyboard::Q) btnNWpressed = status;
                if (event.key.code == sf::Keyboard::A) btnSWpressed = status;
                if (event.key.code == sf::Keyboard::P) btnNEpressed = status;
                if (event.key.code == sf::Keyboard::L) btnSEpressed = status;
                
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

