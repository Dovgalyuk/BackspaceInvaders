#ifndef GAMES_H
#define GAMES_H

#include "libgame.h"
extern game_instance BackspaceInvaders;
extern game_instance Snake;
extern game_instance Tester;
extern game_instance Raycaster;
extern game_instance Dump;

/* Register your game like so:
 * 
 * extern game_instance YOUR_GAME_NAME;
 */
#endif

#ifdef _OUTPUT_INSTANCES
game_instance instances[] = {
    BackspaceInvaders,
    Snake,
    Raycaster,
    Tester,
    Dump,

    /* Register your game like so:
     * 
     * YOUR_GAME_NAME,
     */
};
#endif
