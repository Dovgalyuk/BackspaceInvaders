#pragma GCC optimize ("-O3")

#include "libgame.h"
#include "graphics.h"
#include "binary.h"
//#include "Arduino.h"
#include <stdlib.h>
#include "controls.h"
//изменить управление!!!!!!!!!!!!!
/* Встроенные цвета:
 *
 *  BLACK - Чёрный
 *  BLUE - Синий
 *  GREEN - Зелёный
 *  RED - Красный
 *  CYAN - Циановый
 *  PURPLE - Фиолетовый
 *  YELLOW - Коричневый
 *  WHITE - Белый
 *
 *  Для использования 64-х цветной палитры, укажите в game.ino COLOR_6BIT = 1
 * 
 * */

/* Кнопки:
 *
 * НА КОРПУСЕ:
 * BUTTON_SW, BUTTON_NW, BUTTON_SE, BUTTON_NE
 *
 * NW              NE
 *  SW            SE
 *
 *
 * НА ДЖОЙСТИКЕ:
 * BUTTON_UP, BUTTON_LEFT, BUTTON_RIGHT, BUTTON_DOWN, BUTTON_SELECT, BUTTON_START, BUTTON_A, BUTTON_B
 *
 *      UP
 *  LEFT+RIGHT     SELECT  START      B  A
 *     DOWN
 *
 * */

const uint8_t BoardData[] PROGMEM = {
    B11110000, 
	B11110000,
	B11110000,
	B11110000,
	B11110000,
	B11110000,
	B11110000, 
	B11110000,
	B11110000,
	B11110000,
	B11110000,
	B11110000
};

const game_sprite Board PROGMEM = {
    3, 12, 1, BoardData
};
const uint8_t BallData[] PROGMEM = {
    B01100000,  
    B11110000, 
    B11110000, 
    B01100000  
};

const game_sprite Ball PROGMEM = {
     4, 4, 1, BallData
};

const game_sprite Bonus PROGMEM = {
	2, 2, 1, BallData
};




struct BreakOutData
{
	int Board1Y, Board2Y, 
		flag, k, ballX, 
		ballY, speedy, i,
		speedx, lvlcount;
    
};
static BreakOutData* data;

static void BreakOut_prepare()
{

	game_set_ups(10);
	data->lvlcount = 0;
	data->i  = 75;
    data->ballX = data->ballY = 30;
	if(rand() % 2) data->speedx = 1;
	else data->speedx = -1;
	if(rand() % 2) data->speedy = 1;
	else data->speedy = -1;
	data->Board1Y = data->Board2Y  = 26;
	
	 
}

static void BreakOut_render()
{	
	if( (data->ballX > 64) || 
		(data->ballX < 0))
	{
		if(data->ballX > 64)
		{
			game_draw_text((const unsigned char*)"P2 WINS",12,18,GREEN);
			if(game_is_button_pressed(BUTTON_START))
			BreakOut_prepare();
		}
		else{
			game_draw_text((const unsigned char*)"P1 WINS",12,18,BLUE);
			if(game_is_button_pressed(BUTTON_START))
			BreakOut_prepare();
		}
	}

    game_draw_sprite(&Board,0,data->Board2Y,GREEN);
    game_draw_sprite(&Ball,data->ballX,data->ballY,RED);
	game_draw_sprite(&Board,61,data->Board1Y,BLUE);
	
	switch(data->lvlcount)
	{
	case 3:
		game_draw_text((uint8_t*)"GO GO", 18, 26, WHITE);
		game_set_ups(25);
		break;

	case 10: //10
		game_draw_text((uint8_t*)"NORMA", 18, 26, GREEN);
		game_set_ups(35);
		break;

	case 20: //20
		game_draw_text((uint8_t*)"HARD", 18, 26, YELLOW);
		game_set_ups(45);
		break;

	case 30:
		game_draw_text((uint8_t*)"HARDCORE", 12, 26, RED);
		game_set_ups(55);
		break;

	
	}
	if(data->lvlcount > 40 && (data->lvlcount % 4 == 0)) { 
		data->i++;
		game_set_ups(data->i);
		game_draw_text((uint8_t*)"SUICIDE", 12, 26, PURPLE);
	}
		
}

static void BreakOut_update(unsigned long delta)
{
    if(game_is_button_pressed(BUTTON_DOWN) && data->Board1Y < 52)
		data->Board1Y = (data->Board1Y + 1);

    if(game_is_button_pressed(BUTTON_UP) && data->Board1Y > 0)
		data->Board1Y = (data->Board1Y - 1);
	if(true){
	if(game_is_button_pressed(BUTTON_A) && data->Board2Y < 52)
		data->Board2Y = (data->Board2Y + 1);

    if(game_is_button_pressed(BUTTON_B) && data->Board2Y > 0)
		data->Board2Y = (data->Board2Y - 1);
	}
	else if(data->ballY <= 52)
			data->Board2Y = data->ballY;

	if((data->ballY >= data->Board1Y - 4) && 
		(data->ballY <= data->Board1Y + 16) && 
			(data->ballX == 57)) 
	{
		data->speedx = -data->speedx;
		data->lvlcount++;
	}

	if((data->ballY >= data->Board2Y - 6) && 
		(data->ballY <= data->Board2Y + 16) && 
			( data->ballX == 3) ) {

		  data->speedx = -data->speedx;
		  data->lvlcount++;
	  }
	  
	  if( (data->ballY == 0) || (data->ballY == 60) ) 
		  data->speedy = -data->speedy;

      data->ballX += data->speedx;
      data->ballY += data->speedy;

}
game_instance BreakOut = {
    "BreakOut",
    BreakOut_prepare,
    BreakOut_render,
    BreakOut_update,
    sizeof(BreakOutData),
    (void**)(&data)
};