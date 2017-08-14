#pragma GCC optimize ("-O3")

#include "libgame.h"
#include "sprite.h"
#include "binary.h"

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
    B11111111, B11111111,
    B11111111, B11111111,
    B11111111, B11111111
};

const game_sprite YourSprite PROGMEM = {
    // ШИРИНА, ВЫСОТА, КОЛИЧЕСТВО БАЙТ НА СТРОКУ, ДАННЫЕ
    13, 3, 2, YourSprite_lines
};
const uint8_t BallData[] PROGMEM = {
    B01100000, //   xxxx  
    B11110000, // xxxxxxxx
    B11110000, //  xxxxxxxx
    B01100000  //    xxxx
};
const uint8_t BrickData[] PROGMEM = {
    B11111111,
	B11111111
};
const game_sprite Brick PROGMEM = {
    // ШИРИНА, ВЫСОТА, КОЛИЧЕСТВО БАЙТ НА СТРОКУ, ДАННЫЕ
    8,2, 2, BrickData
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
	int BoardX, ballX, ballY,speedy,speedx,BricksX[48] {0,8,16,24,32,40,48,56,
														0,8,16,24,32,40,48,56,
														0,8,16,24,32,40,48,56,
														0,8,16,24,32,40,48,56,
														0,8,16,24,32,40,48,56,
														0,8,16,24,32,40,48,56,};
	int BricksY[48] {0,0,0,0,0,0,0,0,
					 2,2,2,2,2,2,2,2,
					 4,4,4,4,4,4,4,4,
					 6,6,6,6,6,6,6,6,
					 8,8,8,8,8,8,8,8,
					 10,10,10,10,10,10,10,10};
    /* Объявляйте ваши переменные здесь */
    /* Чтобы потом обращаться к ним, пишите data->ПЕРЕМЕННАЯ */
};
static BreakOutData* data; /* Эта переменная - указатель на структуру, которая содержит ваши переменные */

static void BreakOut_prepare()
{
    data->ballX = data->ballY = 30;
    data->speedy = 1;
	data->speedx = -1;
    /* Здесь код, который будет исполнятся один раз */
    /* Здесь нужно инициализировать переменные */
}

static void BreakOut_render()
{
    /* Здесь код, который будет вывзваться для отрисовки кадра */
    /* Он не должен менять состояние игры, для этого есть функция update */
    game_draw_sprite(&YourSprite,data->BoardX,61,WHITE);
    game_draw_sprite(&Ball,data->ballX,data->ballY,RED);
	for(int i = 0; i < 16; i++) {

		game_draw_sprite(&Brick, data->BricksX[i], data->BricksY[i], CYAN);

	}
	for(int i = 17; i < 32; i++) {

		game_draw_sprite(&Brick, data->BricksX[i], data->BricksY[i], GREEN);

	}
    /* Здесь (и только здесь) нужно вызывать функции game_draw_??? */
}

static void BreakOut_update(unsigned long delta)
{
    /* Здесь код, который будет выполняться в цикле */
    /* Переменная delta содержит количество миллисекунд с последнего вызова */
    if(game_is_button_pressed(BUTTON_RIGHT) && data->BoardX < 51)
    {
      data->BoardX = (data->BoardX + 1);
      }
      if(game_is_button_pressed(BUTTON_LEFT) && data->BoardX > 0)
    {
      data->BoardX = (data->BoardX - 1);
      
      }

	  if((data->ballX>=data->BoardX) && (data->ballX<=data->BoardX+13) && ( data->ballX == 57)) {

		  data->speedy = -data->speedy;

	  }

	  if( (data->ballX == 0) || (data->ballX == 60) ) data->speedx = -data->speedx;

	  for(int i = 0; i < 48; i++) {

		  if( (BricksX[i] >= ballX) %% (BricksX[i]+8 <= ballX) && (ballX == BricksY[i]) && (BricksX[i] != null) ) {

				BricksX[i] = null;

				data->speedy = -data->speedy;

		  }

	  }

      data->ballX += data->speedx;
      
      data->ballY += data->speedy;
      
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