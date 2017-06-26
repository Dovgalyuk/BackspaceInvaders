#include "libgame.h"
#include "font.h"
#include <math.h>
#include "binary.h"

#ifdef EMULATED
#define PI 3.141592653589793
#endif

#define W 8
#define H 8
#define RAY 64
#define FOW (PI * 0.7)
#define SPEED 0.05
#define ROTSPEED 0.04

const uint8_t gmap[] PROGMEM =
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

const float sines[] PROGMEM =
{
    0.0,
    0.004999979166692708,
    0.009999833334166664,
    0.01499943750632809,
    0.01999866669333308,
    0.024997395914712332,
    0.029995500202495664,
    0.034992854604336196,
    0.03998933418663416,
    0.044984814037660234,
    0.049979169270678324,
    0.054972275027067724,
    0.05996400647944459,
    0.06495423883478259,
    0.06994284733753275,
    0.07492970727274234,
    0.0799146939691727,
    0.08489768280241602,
    0.08987854919801105,
    0.09485716863455732,
    0.09983341664682817,
    0.10480716882888251,
    0.10977830083717484,
    0.11474668839366385,
    0.1197122072889194,
    0.12467473338522772,
    0.1296341426196949,
    0.13459031100734833,
    0.13954311464423652,
    0.14449242971052645,
    0.14943813247359927,
    0.15438009929114346,
    0.15931820661424603,
    0.16425233099048103,
    0.16918234906699608,
    0.17410813759359603,
    0.17902957342582426,
    0.1839465335280413,
    0.18885889497650066,
    0.19376653496242202,
    0.1986693307950613,
    0.20356715990477808,
    0.20845989984609967,
    0.2133474283007824,
    0.21822962308086943,
    0.22310636213174556,
    0.22797752353518852,
    0.23284298551241692,
    0.23770262642713472,
    0.24255632478857217,
    0.24740395925452305,
    0.25224540863437817,
    0.25708055189215523,
    0.26190926814952453,
    0.26673143668883126,
    0.271546936956113,
    0.2763556485641139,
    0.2811574512952942,
    0.2859522251048357,
    0.2907398501236429,
    0.2955202066613397,
    0.3002931752092617,
    0.30505863644344366,
    0.30981647122760303,
    0.31456656061611793,
    0.3193087858570011,
    0.32404302839486854,
    0.32876916987390326,
    0.33348709214081457,
    0.33819667724779146,
    0.34289780745545156,
    0.34759036523578446,
    0.35227423327509017,
    0.3569492944769116,
    0.3616154319649622,
    0.3662725290860478,
    0.3709204694129829,
    0.3755591367475014,
    0.38018841512316165,
    0.38480818880824524,
    0.3894183423086507,
    0.39401876037078065,
    0.3986093279844231,
    0.4031899303856269,
    0.4077604530595704,
    0.412320781743425,
    0.416870802429211,
    0.4214104013666483,
    0.42593946506599983,
    0.4304578803009091,
    0.43496553411123046,
    0.4394623138058535,
    0.44394810696552,
    0.4484228014456347,
    0.45288628537906855,
    0.4573384471789558,
    0.4617791755414832,
    0.466208359448673,
    0.4706258881711583,
    0.4750316512709511,
    0.4794255386042033,
    0.48380744032396045,
    0.4881772468829078,
    0.49253484903610895,
    0.496880137843737,
    0.5012130046737981,
    0.5055333412048473,
    0.5098410394286961,
    0.5141359916531134,
    0.5184180905045173,
    0.5226872289306594,
    0.5269433002033017,
    0.5311861979208837,
    0.5354158160111837,
    0.5396320487339695,
    0.5438347906836429,
    0.5480239367918739,
    0.5521993823302279,
    0.5563610229127841,
    0.5605087544987446,
    0.5646424733950357,
    0.5687620762589004,
    0.5728674601004816,
    0.5769585222853971,
    0.5810351605373054,
    0.5850972729404625,
    0.5891447579422698,
    0.5931775143558132,
    0.5971954413623924,
    0.6011984385140414,
    0.6051864057360399,
    0.6091592433294152,
    0.6131168519734341,
    0.617059132728087,
    0.62098598703656,
    0.6248973167277002,
    0.6287930240184689,
    0.6326730115163868,
    0.6365371822219683,
    0.6403854395311473,
    0.6442176872376915,
    0.6480338295356076,
    0.6518337710215371,
    0.6556174166971407,
    0.6593846719714735,
    0.6631354426633501,
    0.6668696350036982,
    0.6705871556379042,
    0.6742879116281455,
    0.6779718104557152,
    0.6816387600233346,
    0.6852886686574553,
    0.6889214451105518,
    0.6925369985634017,
    0.6961352386273572,
    0.699716075346604,
    0.7032794192004106,
    0.7068251811053663,
    0.7103532724176083,
    0.7138636049350372,
    0.7173560908995231,
    0.7208306429990989,
    0.7242871743701429,
    0.727725598599551,
    0.7311458297268963,
    0.734547782246579,
    0.7379313711099631,
    0.7412965117275034,
    0.7446431199708597,
    0.7479711121750002,
    0.7512804051402932,
    0.7545709161345867,
    0.7578425628952774,
    0.7610952636313667,
    0.7643289370255055,
    0.7675435022360275,
    0.7707388788989697,
    0.7739149871300821,
    0.7770717475268243,
    0.7802090811703507,
    0.7833269096274839,
    0.7864251549526744,
    0.7895037396899508,
    0.7925625868748549,
    0.7956016200363665,
    0.7986207631988146,
    0.8016199408837775,
    0.8045990781119695,
    0.8075581004051147,
    0.8104969337878101,
    0.8134155047893742,
    0.8163137404456838,
    0.8191915683009987,
    0.8220489164097722,
    0.8248857133384505,
    0.8277018881672581,
    0.8304973704919709,
    0.8332720904256765,
    0.836025978600521,
    0.8387589661694433,
    0.8414709848078968,
    0.8441619667155568,
    0.8468318446180154,
    0.8494805517684645,
    0.8521080219493631,
    0.8547141894740935,
    0.8572989891886034,
    0.8598623564730346,
    0.8624042272433383,
    0.8649245379528778,
    0.8674232255940167,
    0.869900227699694,
    0.8723554823449859,
    0.8747889281486545,
    0.8772005042746812,
    0.8795901504337885,
    0.881957806884947,
    0.8843034144368686,
    0.8866269144494866,
    0.888928248835422,
    0.8912073600614346,
    0.8934641911498629,
    0.8956986856800468,
    0.8979107877897399,
    0.9001004421765042,
    0.9022675940990943,
    0.9044121893788251,
    0.906534174400926,
    0.9086334961158823,
    0.9107101020407603,
    0.9127639402605201,
    0.914794959429313,
    0.9168031087717659,
    0.9187883380842494,
    0.9207505977361344,
    0.9226898386710318,
    0.9246060124080191,
    0.9264990710428528,
    0.9283689672491654,
    0.9302156542796494,
    0.9320390859672251,
    0.9338392167261952,
    0.9356160015533845,
    0.9373693960292652,
    0.9390993563190662,
    0.9408058391738707,
    0.942488801931696,
    0.9441482025185611,
    0.9457839994495375,
    0.9473961518297873,
    0.9489846193555848,
    0.9505493623153246,
    0.9520903415905143,
    0.9536075186567522,
    0.9551008555846907,
    0.9565703150409844,
    0.9580158602892235,
    0.9594374551908519,
    0.9608350642060711,
    0.9622086523947287,
    0.9635581854171914,
    0.964883629535204,
    0.9661849516127325,
    0.9674621191167928,
    0.9687151001182638,
    0.9699438632926856,
    0.971148377921043,
    0.972328613890533,
    0.9734845416953178,
    0.9746161324372625,
    0.9757233578266575,
    0.9768061901829258,
    0.9778646024353147,
    0.9788985681235731,
    0.9799080613986128,
    0.9808930570231543,
    0.9818535303723583,
    0.9827894574344412,
    0.9837008148112751,
    0.9845875797189733,
    0.9854497299884588,
    0.9862872440660202,
    0.987100101013849,
    0.987888280510564,
    0.9886517628517185,
    0.9893905289502941,
    0.9901045603371766,
    0.9907938391616186,
    0.9914583481916853,
    0.9920980708146857,
    0.9927129910375874,
    0.993303093487417,
    0.9938683634116439,
    0.9944087866785494,
    0.9949243497775799,
    0.995415039819685,
    0.9958808445376391,
    0.9963217522863486,
    0.9967377520431426,
    0.9971288334080489,
    0.9974949866040538,
    0.9978362024773463,
    0.9981524724975475,
    0.9984437887579234,
    0.9987101439755824,
    0.9989515314916584,
    0.9991679452714756,
    0.999359379904701,
    0.9995258306054787,
    0.9996672932125499,
    0.9997837641893568,
    0.9998752406241309,
    0.9999417202299662,
    0.999983201344876,
    1.0
};

#ifdef EMULATED
#define SINES(x, r) do { r = (sines[x]); } while (0);
#define MAP(x) (gmap[x])
#else
#define SINES(x, res) do { uint32_t w = pgm_read_dword_near(sines + (x)); float *p = (float*)(&w); res = *p; } while (0);
#define MAP(x) (pgm_read_byte_near(gmap + (x)))
#endif

float fastsin(float x)
{
    x -= (int)(x / (2 * PI)) * (2 * PI);
    if (x < 0)
        return fastsin(x + 2 * PI);
    if (x > PI)
        return -fastsin(x - PI);
    if (x > PI / 2)
        return fastsin(PI - x);
    float res;
    SINES((int)(x * 200), res);
    return res;
}

float fastcos(float x)
{
    return fastsin(PI / 2 - x);
}


struct Point
{
	float x;
	float y;
};

struct RaycasterData
{
    Point playerPos;
    Point playerDir;
    Point rayPos;
    Point rayDir;
    Point cameraPlane;
    uint8_t walls[WIDTH];
};


static RaycasterData* data;

bool getmap(int x, int y)
{
    return (MAP(y) >> x) & 1;
}

int getHeightForWallDist(float dist)
{
    int height = fabs(HEIGHT/dist);
    if (height > HEIGHT) {
        height = HEIGHT;
    }
    return height;
}

int playerInWall()
{
    int x = (int)data->playerPos.x;
    int y = (int)data->playerPos.y;
    return getmap(x, y);
}

int getWallForRay(Point rayPos, Point rayDir)
{
    int mapX = (int)rayPos.x;
    int mapY = (int)rayPos.y;
    
    float sideDistX, sideDistY;

    float deltaDistX = sqrt(1 + (rayDir.y * rayDir.y)/(rayDir.x * rayDir.x));
    float deltaDistY = sqrt(1 + (rayDir.x * rayDir.x)/(rayDir.y * rayDir.y));
    
    int stepX, stepY;
    int hit = 0;
    int side;
    
    if (rayDir.x < 0)
    {
        stepX = -1;
        sideDistX = (rayPos.x - mapX) * deltaDistX;
    }
    else
    {
        stepX = 1;
        sideDistX = (mapX + 1.0 - rayPos.x) * deltaDistX;
    }
    if (rayDir.y < 0)
    {
        stepY = -1;
        sideDistY = (rayPos.y - mapY) * deltaDistY;
    }
    else
    {
        stepY = 1;
        sideDistY = (mapY + 1.0 - rayPos.y) * deltaDistY;
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

    float wallDist;
    if (side == 0)
    {
        wallDist = fabs((mapX - rayPos.x + (1 - stepX) / 2) / rayDir.x);
	}
    else
    {
        wallDist = fabs((mapY - rayPos.y + (1 - stepY) / 2) / rayDir.y);
	}

    return getHeightForWallDist(wallDist);
}

void Raycaster_prepare()
{
    data->playerPos.x = 2;
    data->playerPos.y = 2;
    data->playerDir.x = -1;
    data->playerDir.y = 0;
    data->cameraPlane.x = 0;
    data->cameraPlane.y = 0.66;
    game_set_ups(30);
    for (int i = 0; i < WIDTH; ++i)
    {
        data->walls[i] = 0;
    }
}

void Raycaster_render()
{
    for (int i = 0; i < WIDTH; ++i)
    {
        int h = data->walls[i]; 
        int offset = (HEIGHT - h) / 2;
        if (offset)
        {
            game_draw_pixel(i, offset, WHITE);
            game_draw_pixel(i, offset + h - 1, WHITE);
        }
    };
}

void Raycaster_update(unsigned long delta)
{
    if (game_is_button_pressed(BUTTON_UP))
    {
        data->playerPos.x += data->playerDir.x * SPEED;
        data->playerPos.y += data->playerDir.y * SPEED;
        if (playerInWall())
        {
            data->playerPos.x -= data->playerDir.x * SPEED;
            data->playerPos.y -= data->playerDir.y * SPEED;
        }
    }
    if (game_is_button_pressed(BUTTON_DOWN))
    {
        data->playerPos.x -= data->playerDir.x * SPEED;
        data->playerPos.y -= data->playerDir.y * SPEED;
        if (playerInWall())
        {
            data->playerPos.x += data->playerDir.x * SPEED;
            data->playerPos.y += data->playerDir.y * SPEED;
        }
    }
    if (game_is_button_pressed(BUTTON_LEFT))
    {
        Point oldPlayerDir, oldCameraPlane;
        oldPlayerDir.x = data->playerDir.x;
        data->playerDir.x = data->playerDir.x * fastcos(ROTSPEED) - data->playerDir.y * fastsin(ROTSPEED);
        data->playerDir.y = oldPlayerDir.x * fastsin(ROTSPEED) + data->playerDir.y * fastcos(ROTSPEED);
        oldCameraPlane.x = data->cameraPlane.x;
        data->cameraPlane.x = data->cameraPlane.x * fastcos(ROTSPEED) - data->cameraPlane.y * fastsin(ROTSPEED);
        data->cameraPlane.y = oldCameraPlane.x * fastsin(ROTSPEED) + data->cameraPlane.y * fastcos(ROTSPEED);
    }
    if (game_is_button_pressed(BUTTON_RIGHT))
    {
        Point oldPlayerDir, oldCameraPlane;
        oldPlayerDir.x = data->playerDir.x;
        data->playerDir.x = data->playerDir.x * fastcos(-ROTSPEED) - data->playerDir.y * fastsin(-ROTSPEED);
        data->playerDir.y = oldPlayerDir.x * fastsin(-ROTSPEED) + data->playerDir.y * fastcos(-ROTSPEED);
        oldCameraPlane.x = data->cameraPlane.x;
        data->cameraPlane.x = data->cameraPlane.x * fastcos(-ROTSPEED) - data->cameraPlane.y * fastsin(-ROTSPEED);
        data->cameraPlane.y = oldCameraPlane.x * fastsin(-ROTSPEED) + data->cameraPlane.y * fastcos(-ROTSPEED);
    }
    for (int i = 0; i < WIDTH; ++i)
    {
        float cameraXOffset = 2 * i / float(WIDTH) - 1; //maps camera plane from -1 to 1
        data->rayPos.x = data->playerPos.x;
        data->rayPos.y = data->playerPos.y;
        data->rayDir.x = data->playerDir.x + data->cameraPlane.x * cameraXOffset;
        data->rayDir.y = data->playerDir.y + data->cameraPlane.y * cameraXOffset;
        data->walls[i] = getWallForRay(data->rayPos, data->rayDir);
    }
}

game_instance Raycaster = {
    "3D",
    Raycaster_prepare,
    Raycaster_render,
    Raycaster_update,
    2,
    sizeof(RaycasterData),
    (void**)(&data)
};
