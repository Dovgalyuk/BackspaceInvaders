#pragma GCC optimize ("-O3")

#include "libgame.h"
#include "font.h"
#include "binary.h"
#include "avrfix.h"

#define W 8
#define H 8
#define SPEED ftok(0.05)
#define ROTCOUNT 128

#define FLOAT fix_t

static const uint8_t gmap[] PROGMEM =
{
    B11111111,
    B10000001,
    B10000001,
    B10110001,
    B10010001,
    B10011101,
    B10000001,
    B11111111
};

struct Point
{
	FLOAT x;
	FLOAT y;
};

static const Point vectors[ROTCOUNT / 4] PROGMEM = {
    {0, 65536},
    {3215, 65457},
    {6423, 65220},
    {9616, 64826},
    {12785, 64276},
    {15923, 63571},
    {19024, 62714},
    {22078, 61705},
    {25079, 60547},
    {28020, 59243},
    {30893, 57797},
    {33692, 56212},
    {36409, 54491},
    {39039, 52639},
    {41575, 50660},
    {44011, 48558},
    {46340, 46340},
    {48558, 44011},
    {50660, 41575},
    {52639, 39039},
    {54491, 36409},
    {56212, 33692},
    {57797, 30893},
    {59243, 28020},
    {60547, 25079},
    {61705, 22078},
    {62714, 19024},
    {63571, 15923},
    {64276, 12785},
    {64826, 9616},
    {65220, 6423},
    {65457, 3215},
};

#define MAP(x) (pgm_read_byte_near(gmap + (x)))
#define VECTOR(i, res) do { (res).x = pgm_read_dword_near(vectors + (i)); (res).y = pgm_read_dword_near((uint8_t*)(vectors + (i)) + 4); } while (0)

struct RaycasterData
{
    Point playerPos;
    uint8_t dir;
    uint8_t walls[WIDTH];
    //char text1[8];
    //char text2[8];
};


static RaycasterData* data;

static inline Point get_vector(uint8_t dir)
{
    Point res;
    dir &= ROTCOUNT - 1;
    int part = dir / (ROTCOUNT / 4);
    dir &= ROTCOUNT / 4 - 1;
    switch (part)
    {
    case 0:
        VECTOR(dir, res);
        break;
    case 1:
        VECTOR(ROTCOUNT / 4 - dir - 1, res);
        res.y = -res.y;
        break;
    case 2:
        VECTOR(dir, res);
        res.x = -res.x;
        res.y = -res.y;
        break;
    case 3:
        VECTOR(ROTCOUNT / 4 - dir - 1, res);
        res.x = -res.x;
        break;
    }
    return res;
}

static inline bool getmap(int x, int y)
{
    if (x < 0 || x >= W || y < 0 || y >= H) return 1;
    return (MAP(y) >> x) & 1;
}

static inline int getHeightForWallDist(FLOAT dist)
{
    int height = divikS(HEIGHT, dist);
    if (height > HEIGHT - 2) {
        height = HEIGHT - 2;
    }
    return height;
}

static inline int playerInWall()
{
    int x = ktoi(data->playerPos.x);
    int y = ktoi(data->playerPos.y);
    return getmap(x, y);
}

static int getWallForRay(Point rayPos, Point rayDir)
{
    int mapX = ktoi(rayPos.x);
    int mapY = ktoi(rayPos.y);
    FLOAT sideDistX, sideDistY;

    FLOAT len = sqrtkD(mulkD(rayDir.x, rayDir.x) + mulkD(rayDir.y, rayDir.y));
    FLOAT deltaDistX = divkS(len, absk(rayDir.x));
    FLOAT deltaDistY = divkS(len, absk(rayDir.y));

    int stepX, stepY;
    int hit = 0;
    int side;

    if (rayDir.x < 0)
    {
        stepX = -1;
        sideDistX = mulkD(rayPos.x - itok(mapX), deltaDistX);
    }
    else
    {
        stepX = 1;
        sideDistX = mulkD(itok(mapX + 1) - rayPos.x, deltaDistX);
    }
    if (rayDir.y < 0)
    {
        stepY = -1;
        sideDistY = mulkD(rayPos.y - itok(mapY), deltaDistY);
    }
    else
    {
        stepY = 1;
        sideDistY = mulkD(itok(mapY + 1) - rayPos.y, deltaDistY);
    }

    while (!hit)
    {
        if (sideDistX < sideDistY)
        {
            sideDistX += deltaDistX;
            mapX += stepX;
            side = 0;
        }
        else
        {
            sideDistY += deltaDistY;
            mapY += stepY;
            side = 1;
        }
        
        if (getmap(mapX, mapY))
        {
            hit = 1;
        }
    }

    FLOAT wallDist;
    if (side == 0)
    {
        wallDist = divkS(itok(mapX + (1 - stepX) / 2) - rayPos.x, rayDir.x);
    }
    else
    {
        wallDist = divkS(itok(mapY + (1 - stepY) / 2) - rayPos.y, rayDir.y);
    }
    wallDist = absk(wallDist);

    return getHeightForWallDist(wallDist);
}

static void update_walls()
{
    for (int i = 0; i < WIDTH; ++i)
    {
        FLOAT cameraXOffset = itok(2 * i) / WIDTH - itok(1); //maps camera plane from -1 to 1
        Point playerDir = get_vector(data->dir);
        Point cameraPlane = get_vector(data->dir + ROTCOUNT / 4);
        //sprintf(data->text1, "%d", playerDir.x);
        //sprintf(data->text2, "%d", playerDir.y);
        Point rayDir = { .x = playerDir.x + mulkD(cameraPlane.x, cameraXOffset),
                         .y = playerDir.y + mulkD(cameraPlane.y, cameraXOffset) };
        data->walls[i] = getWallForRay(data->playerPos, rayDir);
    }
}

static void Raycaster_prepare()
{
    data->playerPos.x = itok(2);
    data->playerPos.y = itok(2);
    data->dir = 0;//8 * ROTCOUNT / 8;//ROTCOUNT / 2;
    game_set_ups(30);
    update_walls();
}

static void Raycaster_render()
{
//    int prev = 0;
    for (int i = 0; i < WIDTH; ++i)
    {
        int h = data->walls[i]; 
        int offset = (HEIGHT - h) / 2;
        if (offset)
        {
            game_draw_pixel(i, offset, WHITE);
            game_draw_pixel(i, offset + h - 1, WHITE);
            /*if (abs(offset - prev) > 5)
            {
                for (int j = offset ; j < offset + h - 1 ; ++j)
                {
                    game_draw_pixel(i, j, WHITE);
                }
            }*/
        }
        //prev = offset;
    }
    //game_draw_text((const uint8_t*)data->text1, 0, 0, GREEN);
    //game_draw_text((const uint8_t*)data->text2, 0, 8, GREEN);
}

static void Raycaster_update(unsigned long delta)
{
    bool moved = false;
    if (game_is_button_pressed(BUTTON_UP))
    {
        Point playerDir = get_vector(data->dir);
        moved = true;
        data->playerPos.x += mulkD(SPEED, playerDir.x);
        data->playerPos.y += mulkD(SPEED, playerDir.y);
        if (playerInWall())
        {
            data->playerPos.x -= mulkD(SPEED, playerDir.x);
            data->playerPos.y -= mulkD(SPEED, playerDir.y);
        }
    }
    if (game_is_button_pressed(BUTTON_DOWN))
    {
        Point playerDir = get_vector(data->dir);
        moved = true;
        data->playerPos.x -= mulkD(SPEED, playerDir.x);
        data->playerPos.y -= mulkD(SPEED, playerDir.y);
        if (playerInWall())
        {
            data->playerPos.x += mulkD(SPEED, playerDir.x);
            data->playerPos.y += mulkD(SPEED, playerDir.y);
        }
    }
    if (game_is_button_pressed(BUTTON_LEFT))
    {
        moved = true;
        --data->dir;
    }
    if (game_is_button_pressed(BUTTON_RIGHT))
    {
        moved = true;
        ++data->dir;
    }
    if (moved)
    {
        update_walls();
    }
}

game_instance Raycaster = {
    "3D",
    Raycaster_prepare,
    Raycaster_render,
    Raycaster_update,
    sizeof(RaycasterData),
    (void**)(&data)
};

