#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "../tools/tools.h"

void A2D_init(void);
void A2D_cleanup(void);
int A2D_getNumLightSamples(void);
double A2D_getAverageReading(void);
double A2D_calculateAvg(void);
void * A2D_readLightSensor(void*);
void * A2D_readPotMeter(void*);
int* A2D_getHistory(void);
int A2D_getPotValue(void);
int A2D_getHistoryLength(void);
double A2D_convertToVoltage(int n);
int A2D_getDips(void);
