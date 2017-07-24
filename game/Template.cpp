#pragma GCC optimize ("-O3")

#include "libgame.h"

struct TemplateData
{
    /* Объявляйте ваши переменные здесь */
    /* Чтобы потом обращатся к ним, пишите data->ПЕРЕМЕННАЯ */
};
static TemplateData* data;

void Template_prepare()
{
    /* Здесь код, который будет исполнятся единожды */
}

void Template_render()
{
    /* Здесь код, который будет вывзваться для отрисовки кадра */
    /* Он не должен менять состояние игры, для этого есть функция update */
}

void Template_update(unsigned long delta)
{
    /* Здесь код, который будет выполняться в цикле */
    /* Переменная delta содержит количество миллисекунд с последнего вызова */
}

game_instance Template = {
    "Template",         /* Имя, отображаемое в меню */
    Template_prepare,
    Template_render,
    Template_update,
    sizeof(TemplateData),
    (void**)(&data)
};


/* Не забудьте зарегистрировать игру в games.h */
