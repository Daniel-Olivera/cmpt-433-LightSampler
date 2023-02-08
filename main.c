#include <stdio.h>
#include "modules/tools/tools.h"
#include "modules/A2D/A2D.h"
#include "modules/SegDisplay/SegDisplay.h"
#include <pthread.h>
#include "modules/CircularBuffer/CircularBuffer.h"
#include "modules/UDP/UDP.h"
#include <stdlib.h>

#define ON "1"
#define OFF "0"

bool shutdown_app;

int main()
{
    shutdown_app = false;
    SegDisplay_Init();
    A2D_init();
    UDP_init();

    while(!shutdown_app){
        sleepForMs(1);
    }

    SegDisplay_cleanup();
    A2D_cleanup();
    UDP_cleanup();

    printf("Shutting down.\n");
    return 0;

}