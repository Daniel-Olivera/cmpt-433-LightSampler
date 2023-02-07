#include <stdio.h>
#include "modules/tools/tools.h"
#include "modules/A2D/A2D.h"
#include "modules/SegDisplay/SegDisplay.h"
#include <pthread.h>
#include "modules/CircularBuffer/CircularBuffer.h"
#include <stdlib.h>

#define ON "1"
#define OFF "0"

int main()
{
    segDisplayInit();
    A2D_init();

    sleepForMs(20000);

    printf("total number of light samples taken: %d\n", A2D_getNumLightSamples());

    SegDisplay_cleanup();
    A2D_cleanup();



    // int * buffer = malloc(10 * sizeof(int));

    // Cbuff_t buf = Cbuff_init(buffer, 10);

    // for(int i = 0; i < 10; i++){
    //     Cbuff_put(buf, i);
    // }

    // sleepForMs(1000);

    // Cbuff_set_size(buf, 5);

    // while(1){    
    //     printf("data = %d\n", Cbuff_get(buf));
    //     sleepForMs(500);
    // }

    // free(buffer);
    // Cbuff_free(buf);

    return 0;

}