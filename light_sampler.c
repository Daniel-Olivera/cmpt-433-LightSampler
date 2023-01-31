#include <stdio.h>
#include "modules/A2D/A2D.h"

int main()
{
    while (true) {
    int reading = getVoltageReading(POT_FILE);
    printf("Value %5d\n", reading);
    }
    return 0;

}