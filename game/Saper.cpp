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
 
const uint8_t YourSprite_lines[] PROGMEM = {
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

const game_sprite YourSprite PROGMEM = {
    // ШИРИНА, ВЫСОТА, КОЛИЧЕСТВО БАЙТ НА СТРОКУ, ДАННЫЕ
    13, 10, 2, YourSprite_lines
};

*/
const uint8_t YourSprite_lines[] PROGMEM = {
  B11000000,
  B11000000,
  B11000000,
  B11000000,
  B11110000,
  B11110000
};
const game_sprite YourSprite PROGMEM = {
    // ШИРИНА, ВЫСОТА, КОЛИЧЕСТВО БАЙТ НА СТРОКУ, ДАННЫЕ
    4, 6, 1, YourSprite_lines
};
const uint8_t YourSprite1_lines[] PROGMEM = {
  B11111000,
  B11111000,
  B11111000,
  B11111000,
  B11111000
};
const game_sprite YourSprite1 PROGMEM = {
    // ШИРИНА, ВЫСОТА, КОЛИЧЕСТВО БАЙТ НА СТРОКУ, ДАННЫЕ
    5, 5, 1, YourSprite1_lines
};
//------------------------------------------------------------------------------------------------
const uint8_t bl_lines[] PROGMEM = {
  B11111110,
  B10000010,
  B10000010,
  B10000010,
  B10000010,
  B10000010,
  B11111110
};
const game_sprite bl PROGMEM = {
    // ШИРИНА, ВЫСОТА, КОЛИЧЕСТВО БАЙТ НА СТРОКУ, ДАННЫЕ
    7, 7, 1, bl_lines
};
const uint8_t bl1_lines[] PROGMEM = {
  B10101000,
  B01110000,
  B11111000,
  B01110000,
  B10101000
};
const game_sprite bl1 PROGMEM = {
    // ШИРИНА, ВЫСОТА, КОЛИЧЕСТВО БАЙТ НА СТРОКУ, ДАННЫЕ
    5, 5, 1, bl1_lines
};
const uint8_t bl2_lines[] PROGMEM = {
  B00110000,
  B01110000,
  B00010000,
  B00010000,
  B00010000
};
const game_sprite bl2 PROGMEM = {
    // ШИРИНА, ВЫСОТА, КОЛИЧЕСТВО БАЙТ НА СТРОКУ, ДАННЫЕ
    5, 5, 1, bl2_lines
};
const uint8_t bl3_lines[] PROGMEM = {
  B00110000,
  B01001000,
  B00010000,
  B00100000,
  B01111000
};
const game_sprite bl3 PROGMEM = {
    // ШИРИНА, ВЫСОТА, КОЛИЧЕСТВО БАЙТ НА СТРОКУ, ДАННЫЕ
    5, 5, 1, bl3_lines
};
const uint8_t bl4_lines[] PROGMEM = {
  B00110000,
  B01001000,
  B00010000,
  B01001000,
  B00110000
};
const game_sprite bl4 PROGMEM = {
    // ШИРИНА, ВЫСОТА, КОЛИЧЕСТВО БАЙТ НА СТРОКУ, ДАННЫЕ
    5, 5, 1, bl4_lines
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

struct SaperData
{
    /* Объявляйте ваши переменные здесь */
    /* Чтобы потом обращаться к ним, пишите data->ПЕРЕМЕННАЯ */
    int x,m,n,b,v, y,mn;
	int t[11][11];
    char q;
};
static SaperData* data; /* Эта переменная - указатель на структуру, которая содержит ваши переменные */

static void Saper_prepare()
{
	
    /* Здесь код, который будет исполнятся один раз */
    /* Здесь нужно инициализировать переменные */
/*9320939432483298493249832948392432492839432949349824343248380934289348934280923423*/
for (int a=0 ;a<=10;a++ )
for (int i=0 ;i<=10;i++ )
data->t[a][i]=10;



for (int a=1;a<9;a++)
{
data->mn=rand()%10;
data->t[a][data->mn]=19;
if (data->t[a][data->mn+1]!=19){
data->t[a][data->mn+1]=data->t[a][data->mn+1]+1;
}
if (data->t[a][data->mn-1]!=19){
data->t[a][data->mn-1]=data->t[a][data->mn-1]+1;
}
if (data->t[a+1][data->mn]!=19){
data->t[a+1][data->mn]=data->t[a+1][data->mn]+1;
}
if (data->t[a-1][data->mn]!=19){
data->t[a-1][data->mn]=data->t[a-1][data->mn]+1;
}
/*308019283129830912*/
if (data->t[a+1][data->mn+1]!=19){
data->t[a+1][data->mn+1]=data->t[a+1][data->mn+1]+1;
}
if (data->t[a+1][data->mn-1]!=19){
data->t[a+1][data->mn-1]=data->t[a+1][data->mn-1]+1;
}
if (data->t[a-1][data->mn+1]!=19){
data->t[a-1][data->mn+1]=data->t[a-1][data->mn+1]+1;
}
if (data->t[a-1][data->mn-1]!=19){
data->t[a-1][data->mn-1]=data->t[a-1][data->mn-1]+1;
}
/*3420932840932*/
}
/*0293019381293820938012981328932891328091328793287132287913287123712937123871213270132*/
}

static void Saper_render()
{
    /* Здесь код, который будет вывзваться для отрисовки кадра */
    /* Он не должен менять состояние игры, для этого есть функция update */
for (int a=0 ;a<10;a++ )
for (int i=0 ;i<10;i++ )
if (data->t[a][i]==9)
{
game_draw_sprite(&bl1, 14+i*5, 14+a*5, RED);
}
for (int a=0 ;a<10;a++ )
for (int i=0 ;i<10;i++ )
if (data->t[a][i]==1 )
{
game_draw_sprite(&bl2, 14+i*5, 14+a*5, BLUE);
}
for (int a=0 ;a<10;a++ )
for (int i=0 ;i<10;i++ )
if (data->t[a][i]==2 )
{
game_draw_sprite(&bl3, 14+i*5, 14+a*5, CYAN);
}
/*12930928103*/

for (int a=0 ;a<10;a++ )
for (int i=0 ;i<10;i++ )
if (data->t[a][i]==3 )
{
game_draw_sprite(&bl3, 14+i*5, 14+a*5, CYAN);
}
/*1239123912*/
game_draw_text((const uint8_t*)"rec  time", 0, 0, BLUE);
for (int a=0 ;a<10;a++ )
for (int i=0 ;i<10;i++ )
if (data->t[a][i]>9)
{
game_draw_sprite(&YourSprite1, 14+i*5, 14+a*5, CYAN);
}
game_draw_sprite(&bl, 13+data->x*5, 13+data->y*5, BLUE);
    /* Здесь (и только здесь) нужно вызывать функции game_draw_??? */
}

static void Saper_update(unsigned long delta)
{
	if(game_is_button_pressed (BUTTON_UP) && data->m<3 && data->y>0)
  {
	++data->m ;
	if (data->m<3){
		if (data->m>1){
--data->y;
		}
	}


  }
  if(game_is_button_pressed (BUTTON_DOWN) && data->n<3 && data->y<9)
  {
	  ++data->n;
	  if(data->n<3){
		  if (data->n>1){
    ++data->y;
	  }
  }
  }
  if(game_is_button_pressed (BUTTON_LEFT) && data->b<3 && data->x>0)
  {++data->b;
	  if (data->b<3)
	  if(data->b>1){
		  
    --data->x;
	  }
  }
if(game_is_button_pressed (BUTTON_RIGHT) && data->v<3 && data->x<9)
{++data->v;
if(data->v==2){
	  
    ++data->x;
	  }
  }

if(!game_is_button_pressed (BUTTON_UP) &&   data->m>0)
	  --data->m;
if(!game_is_button_pressed (BUTTON_DOWN) &&   data->n>0)
	  --data->n;	
if(!game_is_button_pressed (BUTTON_LEFT) &&   data->b>0)
	  --data->b;
if(!game_is_button_pressed (BUTTON_RIGHT) &&   data->v>0)
	  --data->v;
if(game_is_button_pressed (BUTTON_START) &&   data->t[data->y][data->x]>9)
	  data->t[data->y][data->x]=data->t[data->y][data->x]-10;
if(game_is_button_pressed (BUTTON_SELECT))
{/*9320939432483298493249832948392432492839432949349824343248380934289348934280923423*/
for (int a=0 ;a<=10;a++ )
for (int i=0 ;i<=10;i++ )
data->t[a][i]=10;



for (int a=1;a<9;a++)
{
data->mn=rand()%10;
data->t[a][data->mn]=19;
if (data->t[a][data->mn+1]!=19){
data->t[a][data->mn+1]=data->t[a][data->mn+1]+1;
}
if (data->t[a][data->mn-1]!=19){
data->t[a][data->mn-1]=data->t[a][data->mn-1]+1;
}
if (data->t[a+1][data->mn]!=19){
data->t[a+1][data->mn]=data->t[a+1][data->mn]+1;
}
if (data->t[a-1][data->mn]!=19){
data->t[a-1][data->mn]=data->t[a-1][data->mn]+1;
}
/*308019283129830912*/
if (data->t[a+1][data->mn+1]!=19){
data->t[a+1][data->mn+1]=data->t[a+1][data->mn+1]+1;
}
if (data->t[a+1][data->mn-1]!=19){
data->t[a+1][data->mn-1]=data->t[a+1][data->mn-1]+1;
}
if (data->t[a-1][data->mn+1]!=19){
data->t[a-1][data->mn+1]=data->t[a-1][data->mn+1]+1;
}
if (data->t[a-1][data->mn-1]!=19){
data->t[a-1][data->mn-1]=data->t[a-1][data->mn-1]+1;
}
/*3420932840932*/









}

}
  /* Здесь код, который будет выполняться в цикле */
    /* Переменная delta содержит количество миллисекунд с последнего вызова */

    /* Здесь можно работать с кнопками и обновлять переменные */
}

game_instance Saper = {
    "Saper",         /* Имя, отображаемое в меню */
    Saper_prepare,
    Saper_render,
    Saper_update,
    sizeof(SaperData),
    (void**)(&data)
};


/* Не забудьте зарегистрировать игру в games.h */
