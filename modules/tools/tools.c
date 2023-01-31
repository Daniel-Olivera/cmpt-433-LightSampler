// Utility functions such as sleep and get current time
// Also random number generators for different uses in the main loop

#include "tools.h"

long long getTimeInMs(void)
{
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    long long seconds = spec.tv_sec;
    long long nanoSeconds = spec.tv_nsec;
    long long milliSeconds = seconds * 1000 + nanoSeconds / 1000000;

    return milliSeconds;
}

void sleepForMs (long long delayInMs)
{
    const long long NS_PER_MS = 1000000;
    const long long NS_PER_SECOND = 1000000000;

    long long delayNs = delayInMs * NS_PER_MS;
    int seconds = delayNs / NS_PER_SECOND;
    int nanoseconds = delayNs % NS_PER_SECOND;

    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep(&reqDelay, (struct timespec *) NULL);
}

//gets random number in a range of 0.5s to 3s
//from https://www.geeksforgeeks.org/generating-random-number-range-c/
long long getRandomTimeInMs(void)
{
    srand(time(0));
    return (rand() % (3000 + 1 - 500)) + 500;
}

int getRandomZeroOrOne(void)
{
    return rand() % 2;
}