#ifndef GAMES_H
#define GAMES_H

#include "libgame.h"
extern game_instance BackspaceInvaders;
extern game_instance Snake;
#endif

#ifdef _OUTPUT_INSTANCES
game_instance instances[] = {
    BackspaceInvaders,
    Snake,
};
#endif
