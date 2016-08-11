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
#define WHITE 7

volatile uint8_t
    *latport, *oeport, *addraport, *addrbport, *addrcport, *addrdport,
    *leftport, *rightport, *fireport;
uint8_t
    sclkpin, latpin, oepin, addrapin, addrbpin, addrcpin, addrdpin,
    leftpin, rightpin, firepin;

int cx, cy;
int color = 1;

void setup() {
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

  //frameTime = millis();
}

const uint8_t invaders[2][8][11] = {
{
  {0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0},
  {0, 0, 6, 6, 6, 6, 0, 0, 0, 0, 0},
  {0, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0},
  {6, 6, 0, 6, 6, 0, 6, 6, 0, 0, 0},
  {6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0},
  {0, 6, 0, 6, 6, 0, 6, 0, 0, 0, 0},
  {6, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0},
  {0, 6, 0, 0, 0, 0, 6, 0, 0, 0, 0},
}, {
  {0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0},
  {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
  {0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0},
  {0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1},
  {1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1},
  {0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0},
}
};

#define CANNON_W 13
#define CANNON_H 8
 
const uint8_t cannon[CANNON_H][CANNON_W] = {
  {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0},
  {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

#define GAMEOVER_W 31
#define GAMEOVER_H 15
#define GAMEOVER_X 16
#define GAMEOVER_Y 24
const uint8_t gameover[GAMEOVER_H][GAMEOVER_W + 1] = {
  "..xxxxx...xxx...xx...xx.xxxxxxx",
  ".xx......xx.xx..xxx.xxx.xx.....",
  "xx......xx...xx.xxxxxxx.xx.....",
  "xx..xxx.xx...xx.xxxxxxx.xxxxxx.",
  "xx...xx.xxxxxxx.xx.x.xx.xx.....",
  ".xx..xx.xx...xx.xx...xx.xx.....",
  "..xxxxx.xx...xx.xx...xx.xxxxxxx",
  "...............................",
  ".xxxxx..xx...xx.xxxxxxx.xxxxxx.",
  "xx...xx.xx...xx.xx......xx...xx",
  "xx...xx.xx...xx.xx......xx...xx",
  "xx...xx.xx...xx.xxxxxx..xx..xxx",
  "xx...xx..xx.xx..xx......xxxxx..",
  "xx...xx...xxx...xx......xx.xxx.",
  ".xxxxx.....x....xxxxxxx.xx..xxx"
};

#define LIFE_W 5
#define LIFE_H 3
#define LIFE_X 1
#define LIFE_Y 1 
const uint8_t life[LIFE_H][(LIFE_W + 7) / 8] = {
  {B00100000},
  {B11111000},
  {B11111000}
};

#define DIGIT_W 3
#define DIGIT_H 5
const uint8_t digits[10][5] = {
  {B01100000, // 0
   B10100000,
   B10100000,
   B10100000,
   B11000000},
  {B01000000, // 1
   B11000000,
   B01000000,
   B01000000,
   B01000000},
  {B11000000, // 2
   B00100000,
   B01000000,
   B10000000,
   B11100000},
  {B11000000, // 3
   B00100000,
   B01000000,
   B00100000,
   B11000000},
  {B10100000, // 4
   B10100000,
   B11100000,
   B00100000,
   B00100000},
  {B11100000, // 5
   B10000000,
   B11000000,
   B00100000,
   B11000000},
  {B01100000, // 6
   B10000000,
   B11100000,
   B10100000,
   B11100000},
  {B11100000, // 7
   B00100000,
   B01000000,
   B10000000,
   B10000000},
  {B11100000, // 8
   B10100000,
   B11100000,
   B10100000,
   B11100000},
  {B11100000, // 9
   B10100000,
   B11100000,
   B00100000,
   B11000000}
};

#define INVADER_TYPES 2

int invaderW[INVADER_TYPES] = {8, 11};
int invaderH[INVADER_TYPES] = {8, 8};

#define INVADERS 4
int invaderX[INVADERS];
int invaderY[INVADERS];
int invaderType[INVADERS];
int invaderColor[INVADERS];
int invaderSpeedX[INVADERS];
int invaderSpeedY[INVADERS];

int cannonX = (WIDTH - CANNON_W) / 2;
int cannonY = WIDTH - CANNON_H;

#define SHOOTS 4
#define SHOOT_H 4
int shootX[SHOOTS];
int shootY[SHOOTS];

#define T_LENGTH 64
#define T_WIDTH 49
int trajectory[T_LENGTH] = {24, 26, 28, 30, 33, 35, 37, 39, 40, 42, 43, 45, 46, 46, 47, 47, 48, 47, 47, 46, 46, 45, 43, 42, 40, 39, 37, 35, 33, 30, 28, 26, 23, 21, 19, 17, 14, 12, 10, 8, 7, 5, 4, 2, 1, 1, 0, 0, 0, 0, 0, 1, 1, 2, 4, 5, 7, 8, 10, 12, 14, 17, 19, 21};
int invaderPhase[INVADERS] = {0, 0, 0, 0};

#define SCORE_X 48
#define SCORE_Y 1
#define SCORE_DIGITS 4
long score;

unsigned long frameTime;
unsigned long invadeTime;
unsigned long moveTime;
unsigned long shootTime;

int lives = 3;

void renderLine(uint8_t *buf, int y)
{
  for (int i = 0 ; i < WIDTH ; ++i)
    buf[i] = 0;

  // draw invaders
  for (int i = 0 ; i < INVADERS ; ++i)
  {
    if (invaderType[i])
    {
      if (invaderY[i] <= y && invaderY[i] + invaderH[invaderType[i] - 1] > y)
      {
        int w = invaderW[invaderType[i] - 1];
        for (int x = 0 ; x < w ; ++x)
        {
          int xx = invaderX[i] + x;
          if (xx < WIDTH && xx >= 0)
          {
            if (invaders[invaderType[i] - 1][y - invaderY[i]][x])
              buf[xx] = invaderColor[i];
          }
        }
      }
    }
  }
  // draw cannon
  if (lives && cannonY <= y && cannonY + CANNON_H > y)
  {
    for (int x = 0 ; x < CANNON_W ; ++x)
    {
      int xx = (cannonX + x) % WIDTH;
      if (xx < WIDTH && xx >= 0)
      {
        if (cannon[y - cannonY][x])
          buf[xx] = GREEN;
      }
    }
  }
  // draw shoots
  for (int i = 0 ; i < SHOOTS ; ++i)
  {
    if (shootY[i] >= 0)
    {
      if (y >= shootY[i] && y < shootY[i] + SHOOT_H)
        buf[shootX[i]] = WHITE;
    }
  }

  // draw gameover
  if (!lives && y >= GAMEOVER_Y && y < GAMEOVER_Y + GAMEOVER_H)
  {
    for (int x = 0 ; x < GAMEOVER_W ; ++x)
      if (gameover[y - GAMEOVER_Y][x] != '.')
        buf[x + GAMEOVER_X] = WHITE;
  }

  // draw lives
  if (y >= LIFE_Y && y < LIFE_Y + LIFE_H)
  {
    for (int i = 0 ; i < lives ; ++i)
    {
      uint8_t mask = 0x80;
      int offs = 0;
      for (int x = 0 ; x < LIFE_W ; ++x)
      {
        if (life[y - LIFE_Y][offs] & mask)
          buf[x + LIFE_X + (LIFE_W + 1) * i] = GREEN;
        mask >>= 1;
        if (!mask)
        {
          mask = 0x80;
          ++offs;
        }
      }
    }
  }

  // draw score
  if (y >= SCORE_Y && y < SCORE_Y + DIGIT_H)
  {
    int d[SCORE_DIGITS] = {0};
    int i = SCORE_DIGITS - 1;
    long s = score;
    while (i >= 0 && s)
    {
      d[i--] = s % 10;
      s /= 10;
    }
    for (int i = 0 ; i < SCORE_DIGITS ; ++i)
    {
      uint8_t mask = 0x80;
      for (int x = 0 ; x < DIGIT_W ; ++x)
      {
        if (digits[d[i]][y - SCORE_Y] & mask)
          buf[x + SCORE_X + (DIGIT_W + 1) * i] = WHITE;
        mask >>= 1;
      }
    }
  }

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
    for (int i = 0 ; i < INVADERS ; ++i)
    {
      if (invaderType[i])
      {
        int x = invaderX[i];
        int y = invaderY[i];
        y += invaderSpeedY[i];
        if (y < WIDTH - 18)
        {        
          x -= trajectory[invaderPhase[i]];
          invaderPhase[i] = (invaderPhase[i] + invaderSpeedX[i]) % T_LENGTH;
          x += trajectory[invaderPhase[i]];
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
            score -= 5;
            if (score < 0)
              score = 0;
          }
        }
      }
    }

    // new invader
    if (curTime - invadeTime >= 500)
    {
      for (int i = 0 ; i < INVADERS ; ++i)
        if (!invaderType[i])
        {
          invadeTime = curTime;
          invaderType[i] = rand() % INVADER_TYPES + 1;
          int offs = rand() % (WIDTH - T_WIDTH - invaderW[invaderType[i] - 1]);
          invaderPhase[i] = rand() % T_LENGTH;
          invaderX[i] = offs + trajectory[invaderPhase[i]];
          invaderY[i] = 0;
          const int colors[5] = {4, 1, 5, 6, 3};
          invaderColor[i] = colors[rand() % 5];
          invaderSpeedY[i] = rand() % 2 + 1;
          invaderSpeedX[i] = rand() % 4 + 1;
          break;
        }
    }
  }

  if (step == 0 && curTime - moveTime >= 20)
  {
    moveTime = curTime;  

    // intersections
    // shoot enemy
    for (int s = 0 ; s < SHOOTS ; ++s)
    {
      if (shootY[s] >= 0)
      {
        shootY[s] -= 3;
        for (int i = 0 ; i < INVADERS ; ++i)
        {
          if (invaderType[i] 
            && intersectRect(shootX[s], shootY[s], 1, SHOOT_H, invaderX[i], invaderY[i], invaderW[invaderType[i] - 1], invaderH[invaderType[i] - 1]))
          {
            invaderType[i] = 0;
            shootY[s] = -1;

            ++score;
            break;
          }
        }
      }
    }
    if (lives)
    {
      // kill cannon
      for (int i = 0 ; i < INVADERS ; ++i)
      {
        if (invaderType[i] 
          && intersectRect(cannonX, cannonY, CANNON_W, CANNON_H, invaderX[i], invaderY[i], invaderW[invaderType[i] - 1], invaderH[invaderType[i] - 1]))
        {
          --lives;
          if (lives)
            for (int j = 0 ; j < INVADERS ; ++j)
              invaderType[j] = 0;
          break;
        }
      }
  
      if ((*rightport & rightpin) == 0)
      {
        cannonX = (cannonX + 2) % WIDTH;
      }
      else if ((*leftport & leftpin) == 0)
      {
        cannonX = (cannonX - 2 + WIDTH) % WIDTH;
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
            shootX[i] = (cannonX + CANNON_W / 2) % WIDTH;
            break;
          }
        }
      }
    }
  }
}

