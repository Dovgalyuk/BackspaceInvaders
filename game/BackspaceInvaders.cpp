#pragma GCC optimize ("-O3")

#include <stdint.h>
#include "libgame.h"
#include "binary.h"
#include "graphics.h"
#include "controls.h"

#define LEFT (BITMASK(BUTTON_NE) | BITMASK(BUTTON_LEFT))
#define RIGHT (BITMASK(BUTTON_SE) | BITMASK(BUTTON_RIGHT))
#define FIRE (BITMASK(BUTTON_SW) | BITMASK(BUTTON_A) | BITMASK(BUTTON_B))
#define PAUSE (BITMASK(BUTTON_NW) | BITMASK(BUTTON_START))

////////////////////////////////////////////////////////////
// Phases
////////////////////////////////////////////////////////////

enum Phases
{
    PHASE_LOGO,
    PHASE_GAME,
    PHASE_NEXT_WAVE,
    PHASE_GAMEOVER,
    PHASE_PAUSED
};

////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////

#define INVADERS 10

#define SHOOTS 4
#define SHOOT_H 4

#define MAX_WAVE INVADERS

#define SCORE_X 48
#define HISCORE_Y HISCORE_LABEL_Y
#define SCORE_Y HISCORE_Y + DIGIT_H + 1
#define SCORE_DIGITS 4

#define MAX_LIVES 3

struct BackspaceInvadersData
{
    int16_t hiscore;
    int16_t score;
    uint8_t phase;
    int invaderX[INVADERS];
    int invaderY[INVADERS];
    int invaderType[INVADERS];
    int invaderColor[INVADERS];
    int invaderSpeedX[INVADERS];
    int invaderSpeedY[INVADERS];
    int invaderPhase[INVADERS];
    int invaderExplosion[INVADERS];
    int cannonX;
    int cannonY;
    int shootX[SHOOTS];
    int shootY[SHOOTS];
    int wave;
    int waveInvaders;
    unsigned long frameTime;
    unsigned long invadeTime;
    unsigned long moveTime;
    unsigned long shootTime;
    unsigned long phaseTime;
    int lives;
    bool started;
    unsigned long pauseTime;
    unsigned long lastTime;
};

static BackspaceInvadersData* data;

////////////////////////////////////////////////////////////
// Invaders data and functions
////////////////////////////////////////////////////////////

const uint8_t explosion1lines[] PROGMEM = {
    B00100000,
    B10001000,
    B00100000,
    B00000000,
    B00010100,
    B01000000
};

const uint8_t explosion2lines[] PROGMEM = {
    B00010000,
    B10000100,
    B00100000,
    B00000000,
    B01000100,
    B00010000
};

const uint8_t invader1lines[] PROGMEM = {
    B00011000,
    B00111100,
    B01111110,
    B11011011,
    B01111110,
    B10000001,
    B01000010
};

const uint8_t invader2lines[] PROGMEM = {
    B00100000, B10000000,
    B00010001, B00000000,
    B00111111, B10000000,
    B01101110, B11000000,
    B11111111, B11100000,
    B10111111, B10100000,
    B10100000, B10100000,
    B00011011, B00000000
};

const uint8_t invader3lines[] PROGMEM = {
    B00010000, B01000000,
    B00001000, B10000000,
    B00000101, B00000000,
    B00111111, B11100000,
    B01111111, B11110000,
    B11100111, B00111000,
    B01111111, B11110000,
    B00111111, B11100000,
    B00001010, B10000000,
    B00010000, B01000000
};

const uint8_t invader4lines[] PROGMEM = {
    B11100111,
    B00111100,
    B01111110,
    B01011010,
    B11111111,
    B10111101,
    B00111100,
    B01111110,
};

#define EXPLOSION_FRAMES 2
#define EXPLOSION_WIDTH 6
#define EXPLOSION_HEIGHT 6
const game_sprite explosions[EXPLOSION_FRAMES] PROGMEM = {
    {EXPLOSION_WIDTH, EXPLOSION_HEIGHT, 1, explosion1lines},
    {EXPLOSION_WIDTH, EXPLOSION_HEIGHT, 1, explosion2lines}
};

#define INVADER_TYPES 4
const game_sprite invaders[INVADER_TYPES] PROGMEM = {
    {8, 7, 1, invader1lines}, {11, 8, 2, invader2lines},
    {13, 10, 2, invader3lines}, {8, 8, 1, invader4lines}
};

uint8_t invader_width(int type)
{
    return game_sprite_width(&invaders[type - 1]);
}

uint8_t invader_height(int type)
{
    return game_sprite_height(&invaders[type - 1]);
}

const game_sprite *invader_sprite(int type)
{
    return &invaders[type - 1];
}

const game_sprite *explosion_sprite(int frame)
{
    return &explosions[frame - 1];
}

////////////////////////////////////////////////////////////
// Cannon data
////////////////////////////////////////////////////////////

const uint8_t cannonLines[] PROGMEM = {
    B00000010, B00000000,
    B00000111, B00000000,
    B00000111, B00000000,
    B01111111, B11110000,
    B11111111, B11111000,
    B11111111, B11111000,
    B11111111, B11111000,
    B11111111, B11111000
};

const game_sprite cannon PROGMEM = {
    13, 8, 2, cannonLines
};

////////////////////////////////////////////////////////////
// Game over data
////////////////////////////////////////////////////////////

#define GAMEOVER_X 16
#define GAMEOVER_Y 24

const uint8_t gameOverLines[] PROGMEM = {
    B00111110, B00111000, B11000110, B11111110,
    B01100000, B01101100, B11101110, B11000000,
    B11000000, B11000110, B11111110, B11000000,
    B11001110, B11000110, B11111110, B11111100,
    B11000110, B11111110, B11010110, B11000000,  
    B01100110, B11000110, B11000110, B11000000,
    B00111110, B11000110, B11000110, B11111110,
    B00000000, B00000000, B00000000, B00000000,
    B01111100, B11000110, B11111110, B11111100,
    B11000110, B11000110, B11000000, B11000110,
    B11000110, B11000110, B11000000, B11000110,
    B11000110, B11000110, B11111100, B11001110,
    B11000110, B01101100, B11000000, B11111000,
    B11000110, B00111000, B11000000, B11011100,
    B01111100, B00010000, B11111110, B11001110
};

const game_sprite gameOver PROGMEM = {
    31, 15, 4, gameOverLines
};

////////////////////////////////////////////////////////////
// Lives data
////////////////////////////////////////////////////////////

#define LIFE_X 1
#define LIFE_Y 2 
const uint8_t lifeLines[] PROGMEM = {
    B01000000,
    B11100000,
    B11100000
};

const game_sprite life PROGMEM = {
    3, 3, 1, lifeLines
};

////////////////////////////////////////////////////////////
// Hiscore data
////////////////////////////////////////////////////////////

#define HISCORE_LABEL_X 40 
#define HISCORE_LABEL_Y 1
const uint8_t hiLines[] PROGMEM = {
    B10101110,
    B10100100,
    B11100100,
    B10100100,
    B10101110
};

const game_sprite hiLabel PROGMEM = {
    8, 5, 1, hiLines
};

////////////////////////////////////////////////////////////
// Wave data
////////////////////////////////////////////////////////////

const uint8_t waveLines[] PROGMEM = {
    B10100100, B10101110,
    B10101010, B10101000,
    B11101110, B10101110,
    B11101010, B10101000,
    B10101010, B01001110
};

#define WAVE_X 14
#define WAVE_Y 1
const game_sprite wave_sprite PROGMEM = {
    15, 5, 2, waveLines
};

////////////////////////////////////////////////////////////
// Logo data
////////////////////////////////////////////////////////////

const uint8_t logoLines[] PROGMEM = {
    B11111011, B11101111, B10100010, B11111011, B11101111, B10111110, B11111000,
    B10001000, B00001000, B00100010, B10000010, B00100000, B00100000, B00000000,
    B11111011, B11101000, B00111100, B11111010, B00101111, B10100000, B11111000,
    B10001010, B00101000, B00100010, B00001010, B00101000, B10100000, B00000000,
    B11111011, B11101111, B10100010, B11111011, B11101111, B10111110, B11111000,
    B00000000, B00000000, B00000000, B00000010, B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000, B00000010, B00000000, B00000000, B00000000,
    B00000010, B11111010, B00101111, B10111110, B11111011, B11101111, B10000000,
    B00000010, B10001010, B00100000, B00100010, B00000010, B00001000, B00000000,
    B00000010, B10001010, B00101111, B10100010, B11111010, B00001111, B10000000,
    B00000010, B10001001, B01001000, B10100010, B00000010, B00000000, B10000000,
    B00000010, B10001000, B10001111, B10111110, B11111010, B00001111, B10000000,
};

const game_sprite logo PROGMEM = {
    53, 12, 7, logoLines
};

////////////////////////////////////////////////////////////
// Pause data
////////////////////////////////////////////////////////////

#define PAUSE_X 14
#define PAUSE_Y 29

const uint8_t pauseLines[] PROGMEM = {
    B01111000,B11100100,B01001111,B01111101,B11100000,
    B01000101,B00010100,B01010000,B01000001,B00010000,
    B01111001,B11110100,B01011111,B01111101,B00010000,
    B01000001,B00010100,B01000001,B01000001,B00010000,
    B01000001,B00010011,B10011110,B01111101,B11100000,
};

const game_sprite pause PROGMEM = {
    36, 5, 5, pauseLines
};

////////////////////////////////////////////////////////////
// Digits data
////////////////////////////////////////////////////////////

#define DIGIT_H 5
const uint8_t digitLines[] PROGMEM = {
    B01100000, // 0
    B10100000,
    B10100000,
    B10100000,
    B11000000,
    B01000000, // 1
    B11000000,
    B01000000,
    B01000000,
    B01000000,
    B11000000, // 2
    B00100000,
    B01000000,
    B10000000,
    B11100000,
    B11000000, // 3
    B00100000,
    B01000000,
    B00100000,
    B11000000,
    B10100000, // 4
    B10100000,
    B11100000,
    B00100000,
    B00100000,
    B11100000, // 5
    B10000000,
    B11000000,
    B00100000,
    B11000000,
    B01100000, // 6
    B10000000,
    B11100000,
    B10100000,
    B11100000,
    B11100000, // 7
    B00100000,
    B01000000,
    B10000000,
    B10000000,
    B11100000, // 8
    B10100000,
    B11100000,
    B10100000,
    B11100000,
    B11100000, // 9
    B10100000,
    B11100000,
    B00100000,
    B11000000
};

const game_sprite digits[10] PROGMEM = {
    {3, 5, 1, digitLines},
    {3, 5, 1, digitLines + DIGIT_H},
    {3, 5, 1, digitLines + DIGIT_H * 2},
    {3, 5, 1, digitLines + DIGIT_H * 3},
    {3, 5, 1, digitLines + DIGIT_H * 4},
    {3, 5, 1, digitLines + DIGIT_H * 5},
    {3, 5, 1, digitLines + DIGIT_H * 6},
    {3, 5, 1, digitLines + DIGIT_H * 7},
    {3, 5, 1, digitLines + DIGIT_H * 8},
    {3, 5, 1, digitLines + DIGIT_H * 9},
};

////////////////////////////////////////////////////////////
// Trajectory data
////////////////////////////////////////////////////////////

#define T_LENGTH 64
#define T_WIDTH 49
const uint8_t trajectory[T_LENGTH] PROGMEM = {24, 26, 28, 30, 33, 35, 37, 39, 40, 42, 43, 45, 46, 46, 47, 47, 48, 47, 47, 46, 46, 45, 43, 42, 40, 39, 37, 35, 33, 30, 28, 26, 23, 21, 19, 17, 14, 12, 10, 8, 7, 5, 4, 2, 1, 1, 0, 0, 0, 0, 0, 1, 1, 2, 4, 5, 7, 8, 10, 12, 14, 17, 19, 21};

void render_digits(uint16_t num, int len, int x, int y, uint8_t color)
{
    uint8_t d[8] = {0};
    int i = len - 1;
    uint16_t s = num;
    while (i >= 0 && s)
    {
        d[i--] = s % 10;
        s /= 10;
    }
    for (int i = 0 ; i < len ; ++i)
    {
        game_draw_sprite(&digits[d[i]], x, y, color);
        x += game_sprite_width(&digits[d[i]]) + 1;
    }
}

void BackspaceInvaders_render()
{
    // draw invaders
    for (int i = 0 ; i < data->wave ; ++i)
    {
        if (data->invaderType[i])
        {
            if (data->invaderExplosion[i] == 0)
            {
                game_draw_sprite(invader_sprite(data->invaderType[i]), data->invaderX[i], data->invaderY[i], data->invaderColor[i]);
            }
            else
            {
                int w = invader_width(data->invaderType[i]);
                int h = invader_height(data->invaderType[i]);
                int x = ((w - EXPLOSION_WIDTH) >> 1) + data->invaderX[i];
                int y = ((h - EXPLOSION_HEIGHT) >> 1) + data->invaderY[i];
                game_draw_sprite(explosion_sprite(data->invaderExplosion[i]), x, y, data->invaderColor[i]);
            }
        }
    }

    // draw cannon
    if ((data->lives && data->phase == PHASE_GAME) || data->phase == PHASE_NEXT_WAVE || data->phase == PHASE_PAUSED)
        game_draw_sprite(&cannon, data->cannonX, data->cannonY, GREEN);

    // draw shoots
    if (data->phase == PHASE_GAME || data->phase == PHASE_NEXT_WAVE || data->phase == PHASE_PAUSED)
        for (int i = 0 ; i < SHOOTS ; ++i)
        {
            if (data->shootY[i] >= 0)
            {
                for (int y = data->shootY[i]; y < data->shootY[i] + SHOOT_H; ++y)
                    game_draw_pixel(data->shootX[i], y, WHITE);
            }
        }

    // draw gameover
    if (data->phase == PHASE_GAMEOVER)
        game_draw_sprite(&gameOver, GAMEOVER_X, GAMEOVER_Y, WHITE);

    if (data->phase == PHASE_PAUSED)
        game_draw_sprite(&pause, PAUSE_X, PAUSE_Y, WHITE);

    // draw lives
    int lifeWidth = game_sprite_width(&life);
    for (int i = 0 ; i < data->lives ; ++i)
        game_draw_sprite(&life, LIFE_X + (lifeWidth + 1) * i, LIFE_Y, GREEN);

    // draw wave
    if (data->phase == PHASE_GAME || data->phase == PHASE_NEXT_WAVE || data->phase == PHASE_PAUSED)
    {
        game_draw_sprite(&wave_sprite, WAVE_X, WAVE_Y, CYAN);
        render_digits(data->wave, 2, WAVE_X + 1 + game_sprite_width(&wave_sprite), WAVE_Y, WHITE);
    }

    // draw score
    game_draw_sprite(&hiLabel, HISCORE_LABEL_X, HISCORE_Y, CYAN);
    render_digits(data->hiscore, SCORE_DIGITS, SCORE_X, HISCORE_Y, WHITE);
    if (data->started)
        render_digits(data->score, SCORE_DIGITS, SCORE_X, SCORE_Y, WHITE);

    // draw logo
    if (data->phase == PHASE_LOGO)
        game_draw_sprite(&logo, (WIDTH - game_sprite_width(&logo)) / 2, (WIDTH - game_sprite_height(&logo)) / 2, rand() % 64);
}

bool intersectRect(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
    int xl = max(x1, x2);
    int xr = min(x1 + w1, x2 + w2);
    int yl = max(y1, y2);
    int yr = min(y1 + h1, y2 + h2);
    return xr > xl && yr > yl;
}


void BackspaceInvaders_update(unsigned long delta) {
    unsigned long curTime = data->lastTime + delta;
    data->lastTime = curTime;
    if (curTime - data->frameTime >= 160)
    {
        data->frameTime = curTime;

        // moving invaders
        if (data->phase != PHASE_PAUSED)
        {
            for (int i = 0 ; i < data->wave ; ++i)
            {
                if (data->invaderType[i])
                {
                    if (data->invaderExplosion[i] != 0)
                    {
                        if (data->invaderExplosion[i] == EXPLOSION_FRAMES)
                            data->invaderType[i] = 0;
                        else
                            data->invaderExplosion[i]++;
                        continue;
                    }
                    int x = data->invaderX[i];
                    int y = data->invaderY[i];
                    y += data->invaderSpeedY[i];
                    if (y < WIDTH - 18)
                    {
                        x -= pgm_read_byte(&trajectory[data->invaderPhase[i]]);
                        data->invaderPhase[i] = (data->invaderPhase[i] + data->invaderSpeedX[i]) % T_LENGTH;
                        x += pgm_read_byte(&trajectory[data->invaderPhase[i]]);
                    }
                    else
                    {
                        data->invaderSpeedY[i] = 5;
                    }
                    data->invaderX[i] = x;
                    data->invaderY[i] = y;

                    if (y > WIDTH)
                    {
                        data->invaderType[i] = 0;
                        if (data->lives)
                        {
                            if (data->score >= 5)
                                data->score -= 5;
                            else
                                data->score = 0;
                        }
                    }
                }
            }
        }

        // new invader
        if (data->phase == PHASE_GAME && curTime - data->invadeTime >= 500)
        {
            for (int i = 0 ; i < data->wave ; ++i)
                if (!data->invaderType[i] && data->waveInvaders)
                {
                    if (data->lives)
                        --data->waveInvaders;
                    data->invadeTime = curTime;
                    uint8_t t = rand() % INVADER_TYPES + 1;
                    int offs = rand() % (WIDTH - T_WIDTH - invader_width(t));
                    data->invaderPhase[i] = rand() % T_LENGTH;
                    data->invaderX[i] = offs + pgm_read_byte(&trajectory[data->invaderPhase[i]]);
                    data->invaderY[i] = 0;
                    data->invaderExplosion[i] = 0;
                    const int colors[5] = {BLUE, RED, GREEN, PURPLE, YELLOW};
                    data->invaderColor[i] = colors[rand() % 5];
                    data->invaderSpeedY[i] = rand() % 2 + 1;
                    data->invaderSpeedX[i] = rand() % 4 + 1;
                    data->invaderType[i] = t;
                    break;
                }
        }
    }

    // pause
    if (game_is_any_button_pressed(PAUSE) && (curTime >= data->pauseTime))
    {
        data->pauseTime = curTime + 500;
        if (data->phase == PHASE_PAUSED)
        {
            data->phase = PHASE_GAME;
        }
        else if ((data->phase == PHASE_GAME) || (data->phase == PHASE_NEXT_WAVE))
        {
            data->phase = PHASE_PAUSED;
        }
    }

    if ((data->phase == PHASE_GAME || data->phase == PHASE_NEXT_WAVE) && curTime - data->moveTime >= 20)
    {
        data->moveTime = curTime;  

        // intersections
        // shoot enemy
        for (int s = 0 ; s < SHOOTS ; ++s)
        {
            if (data->shootY[s] >= 0)
            {
                data->shootY[s] -= 3;
                for (int i = 0 ; i < data->wave ; ++i)
                {
                    if (data->invaderType[i] && !data->invaderExplosion[i]
                            && intersectRect(data->shootX[s], data->shootY[s], 1, SHOOT_H, data->invaderX[i], data->invaderY[i], invader_width(data->invaderType[i]), invader_height(data->invaderType[i])))
                    {
                        data->invaderExplosion[i] = 1;
                        data->shootY[s] = -1;

                        data->score += 1;//wave;
                        break;
                    }
                }
            }
        }
        if (data->lives)
        {
            // next wave
            bool haveInvaders = false;
            for (int i = 0 ; i < data->wave ; ++i)
                if (data->invaderType[i])
                    haveInvaders = true;
            if (!haveInvaders && !data->waveInvaders && data->phase == PHASE_GAME)
            {
                data->phase = PHASE_NEXT_WAVE;
                data->invaderType[data->wave] = 0;
                ++data->wave;
                data->waveInvaders = data->wave * 10;
                // Impossible mission
                if (data->wave == MAX_WAVE)
                    data->waveInvaders = 9999;
                data->phaseTime = curTime;
            }

            // kill cannon
            for (int i = 0 ; i < data->wave ; ++i)
            {
                if (data->invaderType[i] 
                        && intersectRect(data->cannonX, data->cannonY, game_sprite_width(&cannon), game_sprite_height(&cannon), data->invaderX[i], data->invaderY[i], invader_width(data->invaderType[i]), invader_height(data->invaderType[i])))
                {
                    --data->lives;
                    if (data->lives)
                    {
                        for (int j = 0 ; j < data->wave ; ++j)
                            data->invaderType[j] = 0;
                    }
                    else
                    {
                        data->phase = PHASE_GAMEOVER;
                        data->phaseTime = curTime;
                        if (data->score > data->hiscore)
                        {
                            data->hiscore = data->score;
                            game_save(&data->hiscore, sizeof(data->hiscore));
                        }
                    }
                    break;
                }
            }

            // move cannon
            int cannonW = game_sprite_width(&cannon);
            int step = 2;
            if (game_is_any_button_pressed(RIGHT)  && data->cannonX + step + cannonW <= WIDTH)
            {
                data->cannonX = data->cannonX + step;
            }
            else if (game_is_any_button_pressed(LEFT) && data->cannonX - step >= 0)
            {
                data->cannonX = data->cannonX - step;
            }
            if (game_is_any_button_pressed(FIRE)
                    && curTime - data->shootTime >= 500) // shoot
            {
                data->shootTime = curTime;
                for (int i = 0 ; i < SHOOTS ; ++i)
                {
                    if (data->shootY[i] < 0)
                    {
                        data->shootY[i] = data->cannonY - SHOOT_H;
                        data->shootX[i] = (data->cannonX + cannonW / 2) % WIDTH;
                        break;
                    }
                }
            }
        }
    }

    if (data->phase == PHASE_LOGO
            && game_is_any_button_pressed(FIRE))
    {
        data->started = true;
        data->phase = PHASE_GAME;
        data->lives = MAX_LIVES;
        data->wave = 0;
        data->waveInvaders = 0;
        data->score = 0;
        data->cannonX = (WIDTH - game_sprite_width(&cannon)) / 2;
        data->cannonY = WIDTH - game_sprite_height(&cannon);
        for (int i = 0 ; i < SHOOTS ; ++i)
            data->shootY[i] = -1;
    }

    if (data->phase == PHASE_NEXT_WAVE && curTime - data->phaseTime > 2000)
    {
        data->phase = PHASE_GAME;
    }

    if (data->phase == PHASE_GAMEOVER && curTime - data->phaseTime > 3000)
    {
        data->phase = PHASE_LOGO;
    }
}

void BackspaceInvaders_prepare()
{
    data->hiscore = 0;
    data->score = 0;
    data->phase = PHASE_LOGO;
    data->cannonX = (WIDTH - game_sprite_width(&cannon)) / 2;
    data->cannonY = WIDTH - game_sprite_height(&cannon);
    data->pauseTime = 0;
    data->lastTime = 0;
    game_load(&data->hiscore, sizeof(data->hiscore));
}


game_instance BackspaceInvaders = {
    "Invaders",
    BackspaceInvaders_prepare,
    BackspaceInvaders_render,
    BackspaceInvaders_update,
    sizeof(BackspaceInvadersData),
    (void**)(&data)
};

