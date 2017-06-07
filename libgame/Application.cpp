#include <stdint.h>
#include "libgame.h"

//#define LEFT BUTTON_SW
//#define RIGHT BUTTON_NW
//#define FIRE BUTTON_SE
//#define PAUSE BUTTON_NE

#define LEFT BUTTON_SW
#define RIGHT BUTTON_SE
#define FIRE BUTTON_NE
#define PAUSE BUTTON_NW

int16_t hiscore = 0;
int16_t score;

uint8_t logo_color = 0;


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
    {8, 8, 1, invader1lines}, {11, 8, 2, invader2lines},
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
uint8_t phase = PHASE_LOGO;

////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////

#define INVADERS 10
int invaderX[INVADERS];
int invaderY[INVADERS];
int invaderType[INVADERS];
int invaderColor[INVADERS];
int invaderSpeedX[INVADERS];
int invaderSpeedY[INVADERS];
int invaderPhase[INVADERS];

int cannonX = (WIDTH - game_sprite_width(&cannon)) / 2;
int cannonY = WIDTH - game_sprite_height(&cannon);

#define SHOOTS 4
#define SHOOT_H 4
int shootX[SHOOTS];
int shootY[SHOOTS];



#define MAX_WAVE INVADERS
int wave;
int waveInvaders;

#define SCORE_X 48
#define HISCORE_Y HISCORE_LABEL_Y
#define SCORE_Y HISCORE_Y + DIGIT_H + 1
#define SCORE_DIGITS 4

unsigned long frameTime;
unsigned long invadeTime;
unsigned long moveTime;
unsigned long shootTime;
unsigned long phaseTime;

#define MAX_LIVES 3
int lives;
bool started;

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

void render()
{
    // draw invaders
    for (int i = 0 ; i < wave ; ++i)
    {
        if (invaderType[i])
        {
            game_draw_sprite(invader_sprite(invaderType[i]), invaderX[i], invaderY[i], invaderColor[i]);
        }
    }

    // draw cannon
    if ((lives && phase == PHASE_GAME) || phase == PHASE_NEXT_WAVE || phase == PHASE_PAUSED)
        game_draw_sprite(&cannon, cannonX, cannonY, GREEN);

    // draw shoots
    if (phase == PHASE_GAME || phase == PHASE_NEXT_WAVE || phase == PHASE_PAUSED)
        for (int i = 0 ; i < SHOOTS ; ++i)
        {
            if (shootY[i] >= 0)
            {
                for (int y = shootY[i]; y < shootY[i] + SHOOT_H; ++y)
                    game_draw_pixel(shootX[i], y, WHITE);
            }
        }

    // draw gameover
    if (phase == PHASE_GAMEOVER)
        game_draw_sprite(&gameOver, GAMEOVER_X, GAMEOVER_Y, WHITE);

    if (phase == PHASE_PAUSED)
        game_draw_sprite(&pause, PAUSE_X, PAUSE_Y, WHITE);

    // draw lives
    int lifeWidth = game_sprite_width(&life);
    for (int i = 0 ; i < lives ; ++i)
        game_draw_sprite(&life, LIFE_X + (lifeWidth + 1) * i, LIFE_Y, GREEN);

    // draw wave
    if (phase == PHASE_GAME || phase == PHASE_NEXT_WAVE || phase == PHASE_PAUSED)
    {
        game_draw_sprite(&wave_sprite, WAVE_X, WAVE_Y, CYAN);
        render_digits(wave, 2, WAVE_X + 1 + game_sprite_width(&wave_sprite), WAVE_Y, WHITE);
    }

    // draw score
    game_draw_sprite(&hiLabel, HISCORE_LABEL_X, HISCORE_Y, CYAN);
    render_digits(hiscore, SCORE_DIGITS, SCORE_X, HISCORE_Y, WHITE);
    if (started)
        render_digits(score, SCORE_DIGITS, SCORE_X, SCORE_Y, WHITE);

    // draw logo
    if (phase == PHASE_LOGO)
        game_draw_sprite(&logo, (WIDTH - game_sprite_width(&logo)) / 2, (WIDTH - game_sprite_height(&logo)) / 2, logo_color);
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

void update(unsigned long delta) {
    logo_color = rand() % 64;
    unsigned long curTime = lastTime + delta;
    lastTime = curTime;
    if (curTime - frameTime >= 160)
    {
        frameTime = curTime;

        // moving invaders
        if (phase != PHASE_PAUSED)
        {
            for (int i = 0 ; i < wave ; ++i)
            {
                if (invaderType[i])
                {
                    int x = invaderX[i];
                    int y = invaderY[i];
                    y += invaderSpeedY[i];
                    if (y < WIDTH - 18)
                    {
                        x -= pgm_read_byte(&trajectory[invaderPhase[i]]);
                        invaderPhase[i] = (invaderPhase[i] + invaderSpeedX[i]) % T_LENGTH;
                        x += pgm_read_byte(&trajectory[invaderPhase[i]]);
                    }
                    else
                    {
                        invaderSpeedY[i] = 5;
                    }
                    invaderX[i] = x;
                    invaderY[i] = y;

                    if (y > WIDTH)
                    {
                        invaderType[i] = 0;
                        if (lives)
                        {
                            if (score >= 5)
                                score -= 5;
                            else
                                score = 0;
                        }
                    }
                }
            }
        }

        // new invader
        if (phase == PHASE_GAME && curTime - invadeTime >= 500)
        {
            for (int i = 0 ; i < wave ; ++i)
                if (!invaderType[i] && waveInvaders)
                {
                    if (lives)
                        --waveInvaders;
                    invadeTime = curTime;
                    invaderType[i] = rand() % INVADER_TYPES + 1;
                    int offs = rand() % (WIDTH - T_WIDTH - invader_width(invaderType[i]));
                    invaderPhase[i] = rand() % T_LENGTH;
                    invaderX[i] = offs + pgm_read_byte(&trajectory[invaderPhase[i]]);
                    invaderY[i] = 0;
                    const int colors[5] = {BLUE, RED, GREEN, PURPLE, BROWN};
                    invaderColor[i] = colors[rand() % 5];
                    invaderSpeedY[i] = rand() % 2 + 1;
                    invaderSpeedX[i] = rand() % 4 + 1;
                    break;
                }
        }
    }

    // pause
    if (game_is_button_pressed(PAUSE) && (curTime >= pauseTime))
    {
        pauseTime = curTime + pauseTimeout;
        if (phase == PHASE_PAUSED)
        {
            phase = PHASE_GAME;
        }
        else if ((phase == PHASE_GAME) || (phase == PHASE_NEXT_WAVE))
        {
            phase = PHASE_PAUSED;
        }
    }

    if ((phase == PHASE_GAME || phase == PHASE_NEXT_WAVE) && curTime - moveTime >= 20)
    {
        moveTime = curTime;  

        // intersections
        // shoot enemy
        for (int s = 0 ; s < SHOOTS ; ++s)
        {
            if (shootY[s] >= 0)
            {
                shootY[s] -= 3;
                for (int i = 0 ; i < wave ; ++i)
                {
                    if (invaderType[i] 
                            && intersectRect(shootX[s], shootY[s], 1, SHOOT_H, invaderX[i], invaderY[i], invader_width(invaderType[i]), invader_height(invaderType[i])))
                    {
                        invaderType[i] = 0;
                        shootY[s] = -1;

                        score += 1;//wave;
                        break;
                    }
                }
            }
        }
        if (lives)
        {
            // next wave
            bool haveInvaders = false;
            for (int i = 0 ; i < wave ; ++i)
                if (invaderType[i])
                    haveInvaders = true;
            if (!haveInvaders && !waveInvaders && phase == PHASE_GAME)
            {
                phase = PHASE_NEXT_WAVE;
                ++wave;
                invaderType[wave - 1] = 0;
                waveInvaders = wave * 10;
                // Impossible mission
                if (wave == MAX_WAVE)
                    waveInvaders = 9999;
                phaseTime = curTime;
            }

            // kill cannon
            for (int i = 0 ; i < wave ; ++i)
            {
                if (invaderType[i] 
                        && intersectRect(cannonX, cannonY, game_sprite_width(&cannon), game_sprite_height(&cannon), invaderX[i], invaderY[i], invader_width(invaderType[i]), invader_height(invaderType[i])))
                {
                    --lives;
                    if (lives)
                    {
                        for (int j = 0 ; j < wave ; ++j)
                            invaderType[j] = 0;
                    }
                    else
                    {
                        phase = PHASE_GAMEOVER;
                        phaseTime = curTime;
                        if (score > hiscore)
                        {
                            hiscore = score;
                        }
                    }
                    break;
                }
            }

            // move cannon
            int cannonW = game_sprite_width(&cannon);
            int step = 2;
            if (game_is_button_pressed(RIGHT)  && cannonX + step + cannonW <= WIDTH)
            {
                cannonX = cannonX + step;
            }
            else if (game_is_button_pressed(LEFT) && cannonX - step >= 0)
            {
                cannonX = cannonX - step;
            }
            if (game_is_button_pressed(FIRE)
                    && curTime - shootTime >= 500) // shoot
            {
                shootTime = curTime;
                for (int i = 0 ; i < SHOOTS ; ++i)
                {
                    if (shootY[i] < 0)
                    {
                        shootY[i] = cannonY - SHOOT_H;
                        shootX[i] = (cannonX + cannonW / 2) % WIDTH;
                        break;
                    }
                }
            }
        }
    }

    if (phase == PHASE_LOGO
            && game_is_button_pressed(FIRE))
    {
        started = true;
        phase = PHASE_GAME;
        lives = MAX_LIVES;
        wave = 0;
        waveInvaders = 0;
        score = 0;
        cannonX = (WIDTH - game_sprite_width(&cannon)) / 2;
        cannonY = WIDTH - game_sprite_height(&cannon);
        for (int i = 0 ; i < SHOOTS ; ++i)
            shootY[i] = -1;
    }

    if (phase == PHASE_NEXT_WAVE && curTime - phaseTime > 2000)
    {
        phase = PHASE_GAME;
    }

    if (phase == PHASE_GAMEOVER && curTime - phaseTime > 3000)
    {
        phase = PHASE_LOGO;
    }
}

void setup()
{
    game_setup(0);
}
