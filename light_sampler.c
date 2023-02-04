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

    pthread_t thread_id;
    int *i = malloc(sizeof(*i));
    *i = 45;
    pthread_create(&thread_id, NULL, showNum, i);

    while (true) {
    int reading = getVoltageReading(POT_FILE);
    printf("Value %5d\n", reading);
    }
    pthread_join(thread_id, NULL);
    return 0;

}