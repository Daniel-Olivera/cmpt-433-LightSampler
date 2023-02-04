#include <stdio.h>
#include "modules/tools/tools.h"
#include "modules/A2D/A2D.h"
#include "modules/SegDisplay/SegDisplay.h"
#include <pthread.h>

#define ON "1"
#define OFF "0"

int main()
{
    segDisplayInit();
    A2D_init();

    sleepForMs(10000);

    A2D_cleanup();
    SegDisplay_cleanup();

    
    return 0;

}