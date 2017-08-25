#ifndef GAMES_H
#define GAMES_H

#include "libgame.h"
extern game_instance BackspaceInvaders;
extern game_instance Snake;
extern game_instance Flappy;
extern game_instance Tester;
extern game_instance Raycaster;
extern game_instance Saper;
#ifndef EMULATED /* for use only on real hardware */
extern game_instance Dump;
#endif
/* Register your game like so:
 * 
 * extern game_instance YOUR_GAME_NAME;
 */
#endif

#ifdef _OUTPUT_INSTANCES
game_instance instances[] = {
    BackspaceInvaders,
    Snake,
    Flappy,
    Raycaster,
    Tester,
	Saper,
#ifndef EMULATED /* for use only on real hardware */
    Dump,
#endif

    /* Register your game like so:
     * 
     * YOUR_GAME_NAME,
     */
};
#endif
