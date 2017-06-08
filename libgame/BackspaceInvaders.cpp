#include <stdint.h>
#include "libgame.h"
#include "games.h"

#define LEFT BUTTON_NE
#define RIGHT BUTTON_SE
#define FIRE BUTTON_SW
#define PAUSE BUTTON_NW

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
};

static BackspaceInvadersData* data;

////////////////////////////////////////////////////////////
// Invaders data and functions
////////////////////////////////////////////////////////////

const uint8_t invader1lines[] DATA = {
    0x18,
    0x3c,
    0x7e,
    0xdb,
    0x7e,
    0x81,
    0x42
};

const uint8_t invader2lines[] DATA = {
    0x20, 0x80,
    0x11, 0x00,
    0x3f, 0x80,
    0x6e, 0xc0,
    0xff, 0xe0,
    0xbf, 0xa0,
    0xa0, 0xa0,
    0x1b, 0x00
};

const uint8_t invader3lines[] DATA = {
    0x10, 0x40,
    0x08, 0x80,
    0x05, 0x00,
    0x3f, 0xe0,
    0x7f, 0xf0,
    0xe7, 0x38,
    0x7f, 0xf0,
    0x3f, 0xe0,
    0x0a, 0x80,
    0x10, 0x40
};

const uint8_t invader4lines[] DATA = {
    0xe7,
    0x3c,
    0x7e,
    0x5a,
    0xff,
    0xbd,
    0x3c,
    0x7e,
};

#define INVADER_TYPES 4
const game_sprite invaders[INVADER_TYPES] DATA = {
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

////////////////////////////////////////////////////////////
// Cannon data
////////////////////////////////////////////////////////////

const uint8_t cannonLines[] DATA = {
    0x02, 0x00,
    0x07, 0x00,
    0x07, 0x00,
    0x7f, 0xf0,
    0xff, 0xf8,
    0xff, 0xf8,
    0xff, 0xf8,
    0xff, 0xf8
};

const game_sprite cannon DATA = {
    13, 8, 2, cannonLines
};

////////////////////////////////////////////////////////////
// Game over data
////////////////////////////////////////////////////////////

#define GAMEOVER_X 16
#define GAMEOVER_Y 24

const uint8_t gameOverLines[] DATA = {
    0x3e, 0x38, 0xc6, 0xfe,
    0x60, 0x6c, 0xee, 0xc0,
    0xc0, 0xc6, 0xfe, 0xc0,
    0xce, 0xc6, 0xfe, 0xfc,
    0xc6, 0xfe, 0xd6, 0xc0,  
    0x66, 0xc6, 0xc6, 0xc0,
    0x3e, 0xc6, 0xc6, 0xfe,
    0x00, 0x00, 0x00, 0x00,
    0x7c, 0xc6, 0xfe, 0xfc,
    0xc6, 0xc6, 0xc0, 0xc6,
    0xc6, 0xc6, 0xc0, 0xc6,
    0xc6, 0xc6, 0xfc, 0xce,
    0xc6, 0x6c, 0xc0, 0xf8,
    0xc6, 0x38, 0xc0, 0xdc,
    0x7c, 0x10, 0xfe, 0xce
};

const game_sprite gameOver DATA = {
    31, 15, 4, gameOverLines
};

////////////////////////////////////////////////////////////
// Lives data
////////////////////////////////////////////////////////////

#define LIFE_X 1
#define LIFE_Y 2 
const uint8_t lifeLines[] DATA = {
    0x40,
    0xe0,
    0xe0
};

const game_sprite life DATA = {
    3, 3, 1, lifeLines
};

////////////////////////////////////////////////////////////
// Hiscore data
////////////////////////////////////////////////////////////

#define HISCORE_LABEL_X 40 
#define HISCORE_LABEL_Y 1
const uint8_t hiLines[] DATA = {
    0xae,
    0xa4,
    0xe4,
    0xa4,
    0xae
};

const game_sprite hiLabel DATA = {
    8, 5, 1, hiLines
};

////////////////////////////////////////////////////////////
// Wave data
////////////////////////////////////////////////////////////

const uint8_t waveLines[] DATA = {
    0xa4, 0xae,
    0xaa, 0xa8,
    0xee, 0xae,
    0xea, 0xa8,
    0xaa, 0x4e
};

#define WAVE_X 14
#define WAVE_Y 1
const game_sprite wave_sprite DATA = {
    15, 5, 2, waveLines
};

////////////////////////////////////////////////////////////
// Logo data
////////////////////////////////////////////////////////////

const uint8_t logoLines[] DATA = {
    0xfb, 0xef, 0xa2, 0xfb, 0xef, 0xbe, 0xf8,
    0x88, 0x08, 0x22, 0x82, 0x20, 0x20, 0x00,
    0xfb, 0xe8, 0x3c, 0xfa, 0x2f, 0xa0, 0xf8,
    0x8a, 0x28, 0x22, 0x0a, 0x28, 0xa0, 0x00,
    0xfb, 0xef, 0xa2, 0xfb, 0xef, 0xbe, 0xf8,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
    0x02, 0xfa, 0x2f, 0xbe, 0xfb, 0xef, 0x80,
    0x02, 0x8a, 0x20, 0x22, 0x02, 0x08, 0x00,
    0x02, 0x8a, 0x2f, 0xa2, 0xfa, 0x0f, 0x80,
    0x02, 0x89, 0x48, 0xa2, 0x02, 0x00, 0x80,
    0x02, 0x88, 0x8f, 0xbe, 0xfa, 0x0f, 0x80,
};

const game_sprite logo DATA = {
    53, 12, 7, logoLines
};

////////////////////////////////////////////////////////////
// Pause data
////////////////////////////////////////////////////////////

#define PAUSE_X 14
#define PAUSE_Y 29

const uint8_t pauseLines[] DATA = {
    0x78,0xe4,0x4f,0x7d,0xe0,
    0x45,0x14,0x50,0x41,0x10,
    0x79,0xf4,0x5f,0x7d,0x10,
    0x41,0x14,0x41,0x41,0x10,
    0x41,0x13,0x9e,0x7d,0xe0,
};

const game_sprite pause DATA = {
    36, 5, 5, pauseLines
};

////////////////////////////////////////////////////////////
// Digits data
////////////////////////////////////////////////////////////

#define DIGIT_H 5
const uint8_t digitLines[] DATA = {
    0x60, // 0
    0xa0,
    0xa0,
    0xa0,
    0xc0,
    0x40, // 1
    0xc0,
    0x40,
    0x40,
    0x40,
    0xc0, // 2
    0x20,
    0x40,
    0x80,
    0xe0,
    0xc0, // 3
    0x20,
    0x40,
    0x20,
    0xc0,
    0xa0, // 4
    0xa0,
    0xe0,
    0x20,
    0x20,
    0xe0, // 5
    0x80,
    0xc0,
    0x20,
    0xc0,
    0x60, // 6
    0x80,
    0xe0,
    0xa0,
    0xe0,
    0xe0, // 7
    0x20,
    0x40,
    0x80,
    0x80,
    0xe0, // 8
    0xa0,
    0xe0,
    0xa0,
    0xe0,
    0xe0, // 9
    0xa0,
    0xe0,
    0x20,
    0xc0
};

const game_sprite digits[10] DATA = {
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
const uint8_t trajectory[T_LENGTH] DATA = {24, 26, 28, 30, 33, 35, 37, 39, 40, 42, 43, 45, 46, 46, 47, 47, 48, 47, 47, 46, 46, 45, 43, 42, 40, 39, 37, 35, 33, 30, 28, 26, 23, 21, 19, 17, 14, 12, 10, 8, 7, 5, 4, 2, 1, 1, 0, 0, 0, 0, 0, 1, 1, 2, 4, 5, 7, 8, 10, 12, 14, 17, 19, 21};

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
            game_draw_sprite(invader_sprite(data->invaderType[i]), data->invaderX[i], data->invaderY[i], data->invaderColor[i]);
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

int frame;
unsigned long pauseTime = 0;
int pauseTimeout = 500;
unsigned long lastTime = 0;

void BackspaceInvaders_update(unsigned long delta) {
    unsigned long curTime = lastTime + delta;
    lastTime = curTime;
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
                    data->invaderType[i] = rand() % INVADER_TYPES + 1;
                    int offs = rand() % (WIDTH - T_WIDTH - invader_width(data->invaderType[i]));
                    data->invaderPhase[i] = rand() % T_LENGTH;
                    data->invaderX[i] = offs + pgm_read_byte(&trajectory[data->invaderPhase[i]]);
                    data->invaderY[i] = 0;
                    const int colors[5] = {BLUE, RED, GREEN, PURPLE, BROWN};
                    data->invaderColor[i] = colors[rand() % 5];
                    data->invaderSpeedY[i] = rand() % 2 + 1;
                    data->invaderSpeedX[i] = rand() % 4 + 1;
                    break;
                }
        }
    }

    // pause
    if (game_is_button_pressed(PAUSE) && (curTime >= pauseTime))
    {
        pauseTime = curTime + pauseTimeout;
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
                    if (data->invaderType[i] 
                            && intersectRect(data->shootX[s], data->shootY[s], 1, SHOOT_H, data->invaderX[i], data->invaderY[i], invader_width(data->invaderType[i]), invader_height(data->invaderType[i])))
                    {
                        data->invaderType[i] = 0;
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
                ++data->wave;
                data->invaderType[data->wave - 1] = 0;
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
                        }
                    }
                    break;
                }
            }

            // move cannon
            int cannonW = game_sprite_width(&cannon);
            int step = 2;
            if (game_is_button_pressed(RIGHT)  && data->cannonX + step + cannonW <= WIDTH)
            {
                data->cannonX = data->cannonX + step;
            }
            else if (game_is_button_pressed(LEFT) && data->cannonX - step >= 0)
            {
                data->cannonX = data->cannonX - step;
            }
            if (game_is_button_pressed(FIRE)
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
            && game_is_button_pressed(FIRE))
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
}


game_instance BackspaceInvaders = {
    "Налётчики",
    BackspaceInvaders_prepare,
    BackspaceInvaders_render,
    BackspaceInvaders_update,
    2,
    sizeof(BackspaceInvadersData),
    (void**)(&data)
};
