#pragma GCC optimize ("-O3")

#include "libgame.h"
#include "sprite.h"
#include "binary.h"
#include <stdlib.h>

/* Встроенные цвета:
 *
 *  BLACK - Чёрный
 *  BLUE - Синий
 *  GREEN - Зелёный
 *  RED - Красный
 *  CYAN - Циановый
 *  PURPLE - Фиолетовый
 *  BROWN - Коричневый
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

/* Спрайты
 * 
 * максимальная высота - 16 пикселей

 определение спрайта


   x     x      
    x   x       
     x x        
  xxxxxxxxx     
 xxxxxxxxxxx    
xxx  xxx  xxx   
 xxxxxxxxxxx    
  xxxxxxxxx     
    x x x       
   x     x      


 --------------------------------
 */
const uint8_t YourSprite_lines[] PROGMEM = {
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

const game_sprite YourSprite PROGMEM = {
    // ШИРИНА, ВЫСОТА, КОЛИЧЕСТВО БАЙТ НА СТРОКУ, ДАННЫЕ
    3, 12, 1, YourSprite_lines
};
const uint8_t BallData[] PROGMEM = {
    B01100000, //   xxxx  
    B11110000, // xxxxxxxx
    B11110000, //  xxxxxxxx
    B01100000  //    xxxx
};

const game_sprite Ball PROGMEM = {
    // ШИРИНА, ВЫСОТА, КОЛИЧЕСТВО БАЙТ НА СТРОКУ, ДАННЫЕ
     4, 4, 1, BallData
};



/* Функции отрисовки
 *
 * game_draw_pixel(x, y, color) - Красит точку (x, y) в цвет color
 * game_draw_char(char, x, y, color) - Выводит символ char с левым верхним углом в точке (x, y) с цветом color
 * game_draw_text(string, x, y, color) - Выводит строку string с левым верхним углом в точке (x, y) с цветом color
 * game_draw_sprite(sprite, x, y, color) - Выводит спрайт sprite с левым верхним углом в точке (x, y) с цветом color
 *
 * */

/* Функции ввода
 *
 * game_is_button_pressed(button) - Нажата ли кнопка? Например: game_is_button_pressed(BUTTON_START)
 * game_is_any_button_pressed(mask) - Нажата ли хотя бы одна кнопка? Например: game_is_any_button_pressed(BITMASK(BUTTON_SW) | BITMASK(BUTTON_DOWN))
 *
 * */

struct BreakOutData
{
	int Board1X,Board2X, flag, k, ballX, ballY,speedy,speedx;
	bool ft;
    /* Объявляйте ваши переменные здесь */
    /* Чтобы потом обращаться к ним, пишите data->ПЕРЕМЕННАЯ */
};
static BreakOutData* data; /* Эта переменная - указатель на структуру, которая содержит ваши переменные */

static void BreakOut_prepare()
{
	//game_set_ups(30);
    data->ballX = 30;
	data->ballY = 30;
    data->speedy = 1;
	data->speedx = 1;
	data->Board1X = 26;
	data->Board2X = 26;
	data->ft = true;
    /* Здесь код, который будет исполнятся один раз */
    /* Здесь нужно инициализировать переменные */
}

static void BreakOut_render()
{
    /* Здесь код, который будет вывзваться для отрисовки кадра */
    /* Он не должен менять состояние игры, для этого есть функция update */
    game_draw_sprite(&YourSprite,0,data->Board2X,GREEN);
    game_draw_sprite(&Ball,data->ballX,data->ballY,RED);
	game_draw_sprite(&YourSprite,61,data->Board1X,BLUE);
    /* Здесь (и только здесь) нужно вызывать функции game_draw_??? */
}

static void BreakOut_update(unsigned long delta)
{
    /* Здесь код, который будет выполняться в цикле */
    /* Переменная delta содержит количество миллисекунд с последнего вызова */
	
    if(game_is_button_pressed(BUTTON_DOWN) && data->Board1X < 51)
    {
      data->Board1X = (data->Board1X + 1);
      }
      if(game_is_button_pressed(BUTTON_UP) && data->Board1X > 0)
    {
      data->Board1X = (data->Board1X - 1);
      
      }

	  if(game_is_button_pressed(BUTTON_A) && data->Board2X < 51)
    {
      data->Board2X = (data->Board2X + 1);
      }
      if(game_is_button_pressed(BUTTON_B) && data->Board2X > 0)
    {
      data->Board2X = (data->Board2X - 1);
      
      }

	  if((data->ballX >= (data->Board1X - 4)) && (data->ballX <= (data->Board1X + 16)) && 
		  ( (data->ballX == 57) || (data->ballX == 3) )) {

		  data->speedx = -data->speedx;

	  }

	  if((data->ballX >= (data->Board2X - 4)) && (data->ballX <= (data->Board2X + 16)) && 
		  ( (data->ballX == 57) || (data->ballX == 3) ) ) {

		  data->speedx = -data->speedx;

	  }
	  
	  if( (data->ballY == 0) || (data->ballY == 60) ) data->speedy = -data->speedy;

	  if(1){
      data->ballX += data->speedx;
      
      data->ballY += data->speedy;
	  }
	  data->ft = !(data->ft);
    /* Здесь можно работать с кнопками и обновлять переменные */
}


game_instance BreakOut = {
    "BreakOut",         /* Имя, отображаемое в меню */
    BreakOut_prepare,
    BreakOut_render,
    BreakOut_update,
    sizeof(BreakOutData),
    (void**)(&data)
};

/* Не забудьте зарегистрировать игру в games.h */