#include "A2D.h"
#include "../CircularBuffer/CircularBuffer.h"
#include "../SegDisplay/SegDisplay.h"
#include <pthread.h>
#include "../periodTimer/periodTimer.h"

#define POT_FILE 0
#define LIGHT_SENS_FILE 1
#define A2D_VOLTAGE_FILE "/sys/bus/iio/devices/iio:device0/in_voltage%d_raw"
#define A2D_VOLTAGE_REF_V 1.8
#define A2D_MAX_READING 4095
#define DEFAULT_BUFFER_SIZE 1000

static pthread_t lightSensorThread;
static pthread_t potMeterThread;
static pthread_mutex_t lock;
static int numLightSamples = 0;
static bool shutdown = false;
Cbuff_t buf;
static int potValue = 0;
static double currentAvg = 0;
static int numDips = 0;
static Period_statistics_t periodTimer;

static int getVoltageReading(int);
static void A2D_checkForDips(double avg);
static void printData(void);

void A2D_init(void)
{

    int * buffer = malloc((DEFAULT_BUFFER_SIZE+1) * sizeof(int));
    buf = Cbuff_init(buffer, DEFAULT_BUFFER_SIZE+1);
    Period_init();

    pthread_create(&lightSensorThread, NULL, A2D_readLightSensor, NULL);
    pthread_create(&potMeterThread, NULL, A2D_readPotMeter, NULL);
}

void A2D_cleanup(void)
{
    shutdown = true;
    pthread_join(lightSensorThread, NULL);
    pthread_join(potMeterThread, NULL);
    Cbuff_free(buf);
    Period_cleanup();
}


void * A2D_readLightSensor(void* nothing)
{
    while (!shutdown) {
        int reading = getVoltageReading(LIGHT_SENS_FILE);   
        pthread_mutex_lock(&lock);
        Cbuff_put(buf, reading);
        currentAvg = A2D_calculateAvg();
        // printf("Current avg = %f,   dips = %d\n", currentAvg, numDips);
        // printf("dips: %d\n", numDips);
        pthread_mutex_unlock(&lock);
        numLightSamples += 1;
        Period_markEvent(PERIOD_EVENT_SAMPLE_LIGHT);
        sleepForMs(1);
    }
    return NULL;
}

void * A2D_readPotMeter(void* nothing)
{
    while (!shutdown) {
        A2D_checkForDips(currentAvg);
        // printf("%d\n", numDips);
        int reading = getVoltageReading(POT_FILE);
        potValue = reading;
        pthread_mutex_lock(&lock);
        if(reading == 0){
            Cbuff_set_size(buf, 1);
        }    
        else{
            Cbuff_set_size(buf, reading);
        }
        printData();
        pthread_mutex_unlock(&lock);
        sleepForMs(1000);
    }
    return NULL;
}

int A2D_getNumLightSamples(void)
{
    // printf("buffer size was: %d\n", Cbuff_capacity(buf));
    // printf("avg light reading was: %f\n", A2D_getAverageReading());
    return numLightSamples;
}

double A2D_getAverageReading(void)
{
    return currentAvg;
}

double A2D_calculateAvg(void)
{
    int* array = Cbuff_getHistory(buf);

    int sum = 0;

    for(int i = 0; i < Cbuff_size(buf); i++){
        sum += array[i];
    }
    free(array);

    double avg = sum / Cbuff_size(buf);

    avg = (0.99 * avg) + (0.01 * currentAvg);

    return (avg/A2D_MAX_READING) * A2D_VOLTAGE_REF_V;
}

static void A2D_checkForDips(double avg)
{
    int* array = Cbuff_getHistory(buf);

    double voltage = 0;

    bool dipDetected = false;

    int sumOfDipsInHistory = 0;

    for(int i = 0; i < Cbuff_size(buf); i++){
        double result = (double) array[i];
        double lastVoltage = voltage;
        voltage = (result/A2D_MAX_READING) * A2D_VOLTAGE_REF_V;
        if((voltage < avg - 0.1) && !dipDetected){
            sumOfDipsInHistory += 1;
            dipDetected = true;
        }
        if((voltage > lastVoltage + 0.03) && dipDetected){
            dipDetected = false;
        }
    }

    numDips = sumOfDipsInHistory;
    SegDisplay_setNum(numDips);
    free(array);
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

static void printData(void)
{
    Period_getStatisticsAndClear(PERIOD_EVENT_SAMPLE_LIGHT, &periodTimer);
    printf("Samples/s = %d\t", A2D_getNumLightSamples());
    printf("Pot Value = %d\t", potValue);
    printf("History size = %d\t", Cbuff_size(buf));
    printf("avg = %.3f\t", currentAvg);
    printf("dips = %d\t", numDips);
    printf("Sampling[%.3f, %.3f] avg %.3f/%d\n", periodTimer.minPeriodInMs, periodTimer.maxPeriodInMs,
                                                     periodTimer.avgPeriodInMs, periodTimer.numSamples);

    int* array = Cbuff_getHistory(buf);

    short ds = Cbuff_size(buf);

    if(ds >= 200){
        for(int i = 0; i < ds; i+=200){
            double result = (double) array[i];
            double voltage = (result/A2D_MAX_READING) * A2D_VOLTAGE_REF_V;
            printf("%.3f\t", voltage);
        }
    }
    free(array);

    printf("\n");

}