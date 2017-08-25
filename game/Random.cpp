#include "libgame.h"
#include "random.h"
#include <stdlib.h>
#ifdef EMULATED
#include <time.h>
#endif

void random_setup()
{
    srand(
#ifdef EMULATED
        (int)time(NULL)
#else
        (int)millis()
#endif
        );
}

