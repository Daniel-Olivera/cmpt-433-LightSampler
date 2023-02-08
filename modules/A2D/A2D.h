#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "../tools/tools.h"

//Init and cleanup of the A2D converter
void A2D_init(void);
void A2D_cleanup(void);

//Retrieves the total number of light samples
int A2D_getNumLightSamples(void);

//Retrieves the stored current average
double A2D_getAverageReading(void);

//Calculates a new average
double A2D_calculateAvg(void);

//Reads the light sensor every 1ms
void * A2D_readLightSensor(void*);

//Reads the Potentiometer every 1s
void * A2D_readPotMeter(void*);

//Retrieves a copy of the history for displaying purposes
int* A2D_getHistory(void);

//Retrieves the stores potentiometer value
int A2D_getPotValue(void);

//Gets the history length
int A2D_getHistoryLength(void);

//Converts A2D value to a voltage
double A2D_convertToVoltage(int n);

//Retrieves the stored amount of dips currently in the history
int A2D_getDips(void);
