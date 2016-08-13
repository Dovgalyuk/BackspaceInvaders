#include <avr/pgmspace.h>
#include <EEPROM.h>

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

#define LEFT 11
#define RIGHT 12
#define FIRE 13

#define WIDTH 64

#define DATAPORT PORTD
#define DATADIR  DDRD
#define SCLKPORT PORTB

#define GREEN 2
#define CYAN  6
#define WHITE 7

volatile uint8_t
    *latport, *oeport, *addraport, *addrbport, *addrcport, *addrdport,
    *leftport, *rightport, *fireport;
uint8_t
    sclkpin, latpin, oepin, addrapin, addrbpin, addrcpin, addrdpin,
    leftpin, rightpin, firepin;

int16_t hiscore;
int16_t score;

void setup()
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

  leftport = portInputRegister(digitalPinToPort(LEFT));
  leftpin  = digitalPinToBitMask(LEFT); 
  rightport = portInputRegister(digitalPinToPort(RIGHT));
  rightpin  = digitalPinToBitMask(RIGHT); 
  fireport = portInputRegister(digitalPinToPort(FIRE));
  firepin  = digitalPinToBitMask(FIRE); 

  // put your setup code here, to run once:
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

  pinMode(LEFT, INPUT);
  pinMode(RIGHT, INPUT);

  SCLKPORT   &= ~sclkpin;
  *latport   &= ~latpin;
  *oeport    &= ~oepin;
  *addraport &= ~addrapin;
  *addrbport &= ~addrbpin;
  *addrcport &= ~addrcpin;
  *addrdport &= ~addrdpin;

  //DATADIR  = B11111100;
  //DATAPORT = 0;
  
  digitalWrite(IR1, HIGH);
  digitalWrite(IG1, HIGH);
  digitalWrite(IB1, HIGH);
  digitalWrite(IR2, HIGH);
  digitalWrite(IG2, HIGH);
  digitalWrite(IB2, HIGH);

  //EEPROM.write(0, 0);
  //EEPROM.write(1, 0);
  hiscore = (EEPROM.read(1) << 8) + EEPROM.read(0);
}

////////////////////////////////////////////////////////////
// Sprite structure and functions
////////////////////////////////////////////////////////////

struct Sprite
{
  uint8_t width;
  uint8_t height;
  uint8_t lineSize;
  const uint8_t *lines;
};

// need to have 'struct' here because of some compiler bug
uint8_t sprite_width(const struct Sprite *s)
{
  return pgm_read_byte(&s->width);
}

uint8_t sprite_height(const Sprite *s)
{
  return pgm_read_byte(&s->height);
}

const uint8_t *sprite_line(const Sprite *s, int line)
{
  return pgm_read_byte(&s->lineSize) * line + (const uint8_t*)pgm_read_pointer(&s->lines);
}

void sprite_render_line(const Sprite *s, int x, int line, uint8_t *buf, uint8_t color)
{
  uint8_t mask = 0x80;
  uint8_t width = sprite_width(s);
  const uint8_t *ptr = sprite_line(s, line);
  uint8_t spr = pgm_read_byte(ptr);
  for (uint8_t dx = 0 ; dx < width ; ++dx)
  {
    int xx = x + dx;
    if (xx >= 0 && xx < WIDTH)
    {
      if (spr & mask)
        buf[xx] = color;
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

void sprite_render(const Sprite *s, int x, int y, uint8_t *buf, int line, uint8_t color)
{
  uint8_t height = sprite_height(s);
  if (line >= y && line < y + height)
  {
    sprite_render_line(s, x, line - y, buf, color);
  }
}

////////////////////////////////////////////////////////////
// Invaders data and functions
////////////////////////////////////////////////////////////

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

#define INVADER_TYPES 4
const Sprite invaders[INVADER_TYPES] PROGMEM = {
  {8, 8, 1, invader1lines}, {11, 8, 2, invader2lines},
  {13, 10, 2, invader3lines}, {8, 8, 1, invader4lines}
};

uint8_t invader_width(int type)
{
  return sprite_width(&invaders[type - 1]);
}

uint8_t invader_height(int type)
{
  return sprite_height(&invaders[type - 1]);
}

const Sprite *invader_sprite(int type)
{
  return &invaders[type - 1];
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

const Sprite cannon PROGMEM = {
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

const Sprite gameOver PROGMEM = {
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

const Sprite life PROGMEM = {
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

const Sprite hiLabel PROGMEM = {
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
const Sprite waveSprite PROGMEM = {
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

const Sprite logo PROGMEM = {
  53, 12, 7, logoLines
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

const Sprite digits[10] PROGMEM = {
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

////////////////////////////////////////////////////////////
// Phases
////////////////////////////////////////////////////////////

#define PHASE_LOGO 1
#define PHASE_GAME 2
#define PHASE_GAMEOVER 3
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

int cannonX = (WIDTH - sprite_width(&cannon)) / 2;
int cannonY = WIDTH - sprite_height(&cannon);

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

void render_digits(uint16_t num, int len, int x, int y, uint8_t *buf, int line, uint8_t color)
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
    sprite_render(&digits[d[i]], x, y, buf, line, color);
    x += sprite_width(&digits[d[i]]) + 1;
  }
}

void renderLine(uint8_t *buf, int y)
{
  for (int i = 0 ; i < WIDTH ; ++i)
    buf[i] = 0;

  // draw invaders
  for (int i = 0 ; i < wave ; ++i)
  {
    if (invaderType[i])
    {
      sprite_render(invader_sprite(invaderType[i]), invaderX[i], invaderY[i], buf, y, invaderColor[i]);
    }
  }
  
  // draw cannon
  if (lives && phase == PHASE_GAME)
    sprite_render(&cannon, cannonX, cannonY, buf, y, GREEN);

  // draw shoots
  if (phase == PHASE_GAME)
    for (int i = 0 ; i < SHOOTS ; ++i)
    {
      if (shootY[i] >= 0)
      {
        if (y >= shootY[i] && y < shootY[i] + SHOOT_H)
          buf[shootX[i]] = WHITE;
      }
    }

  // draw gameover
  if (phase == PHASE_GAMEOVER)
    sprite_render(&gameOver, GAMEOVER_X, GAMEOVER_Y, buf, y, WHITE);

  // draw lives
  int lifeWidth = sprite_width(&life);
  for (int i = 0 ; i < lives ; ++i)
    sprite_render(&life, LIFE_X + (lifeWidth + 1) * i, LIFE_Y, buf, y, GREEN);

  // draw wave
  if (phase == PHASE_GAME)
  {
    sprite_render(&waveSprite, WAVE_X, WAVE_Y, buf, y, CYAN);
    render_digits(wave, 2, WAVE_X + 1 + sprite_width(&waveSprite), WAVE_Y, buf, y, WHITE);
  }

  // draw score
  sprite_render(&hiLabel, HISCORE_LABEL_X, HISCORE_Y, buf, y, CYAN);
  render_digits(hiscore, SCORE_DIGITS, SCORE_X, HISCORE_Y, buf, y, WHITE);
  if (phase == PHASE_GAME)
    render_digits(score, SCORE_DIGITS, SCORE_X, SCORE_Y, buf, y, WHITE);

  // draw logo
  if (phase == PHASE_LOGO)
    sprite_render(&logo, (WIDTH - sprite_width(&logo)) / 2, (WIDTH - sprite_height(&logo)) / 2, buf, y, rand() % 6 + 1);

  // there is broken LED here:
  if (y == 63)
    buf[63] = 0;
}

bool intersectRect(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
  int xl = max(x1, x2);
  int xr = min(x1 + w1, x2 + w2);
  int yl = max(y1, y2);
  int yr = min(y1 + h1, y2 + h2);
  return xr > xl && yr > yl;
}

int step;
int frame;

void loop() {

//  /----1111< - 16x64
//  |    1111
//  | /--2222<
//  | |  2222
//  | \---------\
//  \---------\ |
//       1111</ |
//       1111   |
//       2222<--/
//       2222
// По вертикали - поле 1 - биты BGR1
//                поле 2 - биты BGR2
//                координата - биты ABCD
// По горизонтали - задвигаем 128 пикселей

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
  renderLine(lines[0], y1);
  renderLine(lines[1], y2);
  renderLine(lines[2], y3);
  renderLine(lines[3], y4);

// DATAPORT : BGR2 BGR1 00
      
  for (int i = 0 ; i < WIDTH ; ++i)
  {
    DATAPORT = (lines[3][i] << 5) | (lines[2][i] << 2);//B01001100;
    SCLKPORT = tick; // Clock lo
    SCLKPORT = tock; // Clock hi
  }
  for (int i = 0 ; i < WIDTH ; ++i)
  {
    DATAPORT = (lines[1][i] << 5) | (lines[0][i] << 2);//B01001100;
    SCLKPORT = tick; // Clock lo
    SCLKPORT = tock; // Clock hi
  }

  unsigned long curTime = millis();
  if (step == 0 && curTime - frameTime >= 160)
  {
    frameTime = curTime;

    // moving invaders
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
          const int colors[5] = {4, 1, 5, 6, 3};
          invaderColor[i] = colors[rand() % 5];
          invaderSpeedY[i] = rand() % 2 + 1;
          invaderSpeedX[i] = rand() % 4 + 1;
          break;
        }
    }
  }

  if (step == 0 && phase == PHASE_GAME && curTime - moveTime >= 20)
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
      if (!haveInvaders && !waveInvaders)
      {
        ++wave;
        invaderType[wave - 1] = 0;
        waveInvaders = wave * 10;
        // Impossible mission
        if (wave == MAX_WAVE)
          waveInvaders = 9999;
      }
      
      // kill cannon
      for (int i = 0 ; i < wave ; ++i)
      {
        if (invaderType[i] 
          && intersectRect(cannonX, cannonY, sprite_width(&cannon), sprite_height(&cannon), invaderX[i], invaderY[i], invader_width(invaderType[i]), invader_height(invaderType[i])))
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
              EEPROM.write(0, hiscore & 0xff);
              EEPROM.write(1, (hiscore >> 8) & 0xff);
            }
          }
          break;
        }
      }

      // move cannon
      int cannonW = sprite_width(&cannon);
      int step = 2;
      if ((*rightport & rightpin) == 0 && cannonX + step + cannonW <= WIDTH)
      {
        cannonX = cannonX + step;
      }
      else if ((*leftport & leftpin) == 0 && cannonX - step >= 0)
      {
        cannonX = cannonX - step;
      }
      if ((*fireport & firepin) == 0
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

  if (step == 0 && phase == PHASE_LOGO && (*fireport & firepin) == 0)
  {
    phase = PHASE_GAME;
    lives = MAX_LIVES;
    wave = 0;
    waveInvaders = 0;
    score = 0;
    cannonX = (WIDTH - sprite_width(&cannon)) / 2;
    cannonY = WIDTH - sprite_height(&cannon);
    for (int i = 0 ; i < SHOOTS ; ++i)
      shootY[i] = -1;
  }

  if (step == 0 && phase == PHASE_GAMEOVER && curTime - phaseTime > 3000)
  {
    phase = PHASE_LOGO;
  }
}

