#include <stdio.h>
#include "modules/tools/tools.h"
#include "modules/A2D/A2D.h"
#include "modules/SegDisplay/SegDisplay.h"

int main()
{
    for(int i = 0; i < 10; i++){
        setDisplayValue(i);
        sleepForMs(1000);
    }
    // while (true) {
    // int reading = getVoltageReading(POT_FILE);
    // printf("Value %5d\n", reading);
    // }
    return 0;

}