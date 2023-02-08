//Copied over from assignment 1
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

extern bool shutdown_app;

//Sleeps the thread for a given amount of Milliseconds
void sleepForMs (long long delayInMs);

//Runs a given command in the shell
void runCommand(char* command);
