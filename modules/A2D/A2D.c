#include "A2D.h"
#include <pthread.h>
#define POT_FILE 0
#define LIGHT_SENS_FILE 1
#define A2D_VOLTAGE_FILE "/sys/bus/iio/devices/iio:device0/in_voltage%d_raw"
#define A2D_VOLTAGE_REF_V 1.8
#define A2D_MAX_READING 

pthread_t lightSensorThread;
static bool shutdown = false;

static int getVoltageReading(int);

void A2D_init(void)
{
    pthread_create(&lightSensorThread, NULL, A2D_readLightSensor, NULL);
}

void A2D_cleanup(void)
{
    shutdown = true;
    pthread_join(lightSensorThread, NULL);
}


void * A2D_readLightSensor(void *nothing)
{
    while (!shutdown) {
        int reading = getVoltageReading(LIGHT_SENS_FILE);
        printf("Value %5d\n", reading);
        sleepForMs(1);
    }
    return NULL;
}

void * A2D_readPotMeter(void* nothing)
{
    while (!shutdown) {
        int reading = getVoltageReading(POT_FILE);
        printf("Value %5d\n", reading);
        sleepForMs(1000);
    }
    return NULL;
}

//From Brian's Guide on the A2D
int getVoltageReading(int fileNum)
{

    char fileName[256];
    sprintf(fileName, A2D_VOLTAGE_FILE, fileNum); 
    // Open file
    FILE *f = fopen(fileName, "r");
    if (!f) {
        printf("ERROR: Unable to open voltage input file: %s\n", fileName);
        printf("Cape Loaded?\n");
        printf(" Check /boot/uEnv.txt for correct options.\n");
        exit(-1);
    }
    // Get reading
    int a2dReading = 0;
    int itemsRead = fscanf(f, "%d", &a2dReading);
    if (itemsRead <= 0) {
        printf("ERROR: Unable to read values from voltage input file.\n");
        exit(-1);
    }
    // Close file
    fclose(f);
    return a2dReading;
}