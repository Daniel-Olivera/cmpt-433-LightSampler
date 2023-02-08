#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../A2D/A2D.h"
#include "../SegDisplay/SegDisplay.h"
#include "../tools/tools.h"

#define MAX_LEN 1024
#define PORT 22110

#define TOKEN_BUFSIZE 64
#define TOK_DELIMITER " "

static char lastCommand[MAX_LEN];
static pthread_t UDPThread;
static int sock_desc;
static struct sockaddr_in sock_in;
static struct sockaddr_in sinRemote;
static unsigned int sin_len = sizeof(sinRemote);
static char messageRx[MAX_LEN];

static char* parseInput(char* input);

char* UDP_noCommand(void)
{
    char repeatedCommand[MAX_LEN] = "";
    char* output = "Unknown Command.\n";

    if(strcmp(lastCommand, "") != 0){
        //lastCommand gets consumed for some reason
        //so we have to just send a copy of it
        strcpy(repeatedCommand, lastCommand);
        output = parseInput(repeatedCommand);
    }
    return output;
}

void UDP_stop(void)
{
    shutdown_app = true;
}

void * UDP_getCommand(void* nothing)
{
    while(!shutdown_app){

        int bytesRx = recvfrom(sock_desc, messageRx, MAX_LEN - 1, 0, 
                                (struct sockaddr*) &sinRemote, &sin_len);

        messageRx[bytesRx] = 0;
        //Hard stop if the msg is "stop"
        if(strcmp(messageRx, "stop\n") == 0){
            UDP_stop();
            break;
        }

        char* output;
        if(strcmp(messageRx, "\n") == 0){
            output = UDP_noCommand();
        }
        else{
            //parse and run the command from the message
            strcpy(lastCommand, messageRx);
            output = parseInput(messageRx);
        }

        //send response
        char messageTx[MAX_LEN];
        snprintf(messageTx, MAX_LEN + 256, "%s\n", output);
        sin_len = sizeof(sinRemote);
        sendto(sock_desc, messageTx, strlen(messageTx), 0, (struct sockaddr*) &sinRemote, sin_len);
    }

    close(sock_desc);
    return NULL;
}

void UDP_init(void)
{
    memset(&sock_in, 0, sizeof(sock_in));
    sock_in.sin_family = AF_INET;
    sock_in.sin_port = htons(PORT);
    sock_in.sin_addr.s_addr = htonl(INADDR_ANY);

    sock_desc = socket(PF_INET, SOCK_DGRAM, 0);
    bind(sock_desc, (struct sockaddr*) &sock_in, sizeof(sock_in));

    pthread_create(&UDPThread, NULL, UDP_getCommand, NULL);
}

void UDP_cleanup(void)
{
    pthread_join(UDPThread, NULL);    
}

char* UDP_help(void)
{
    return  "Accepted command examples:\n"
            "count\t-- display total number of samples taken.\n"
            "length\t-- display number of samples in history (both max, and current).\n"
            "history\t-- display the full sample history being saved.\n"
            "get 10\t-- display the 10 most recent history values.\n"
            "dips\t-- display number of dips.\n"
            "stop\t-- cause the server program to end.\n"
            "<enter>\t-- repeat last command.\n";
}

char* UDP_getCount(void)
{
    int numSamples = A2D_getNumLightSamples();
    char buffer[MAX_LEN];
    sprintf(buffer, "%d", numSamples);
    char* result = buffer;
    return result;
}

char* UDP_getLength(void)
{
    int capacity = A2D_getPotValue();
    int historyLength = A2D_getHistoryLength();

    char buffer[MAX_LEN];

    sprintf(buffer, "History can hold\t%d samples.\nCurrently holding\t%d samples.", capacity, historyLength);   

    char* result = buffer;
    return result;
}

char* UDP_getHistory(void)
{
    int* array = A2D_getHistory();

    int arrayLen = A2D_getHistoryLength();

    char vBuffer[24] = "";

    // Need an especially huge buffer to hold up to 4096 values
    char strBuffer[(MAX_LEN * 24) + 1] = ""; 
    for(int i = 0; i < arrayLen-1; i++){
        double voltage = A2D_convertToVoltage(array[i]);
        sprintf(vBuffer, "%.3f, ", voltage);
        strcat(strBuffer, vBuffer);
    }

    free(array);
    char* result = strBuffer;
    return result;
}

char* UDP_getLastN(int n)
{
    int* array = A2D_getHistory();
    int arrayLen = A2D_getHistoryLength();

    char vBuffer[24] = "";
    char strBuffer[MAX_LEN] = "";

    if((arrayLen - n) <= 0){
        return UDP_getHistory();
    }
    else{
        for(int i = arrayLen; i > arrayLen - n; i--){
            double voltage = A2D_convertToVoltage(array[i]);
            sprintf(vBuffer, "%.3f, ", voltage);
            strcat(strBuffer, vBuffer);
        }
    }
    free(array);
    char* result = strBuffer;
    return result;
}

char* UDP_getDips(void)
{
    int dips = A2D_getDips();
    char buffer[MAX_LEN] = "";

    sprintf(buffer, "Dips = %d.\n", dips);
    char* result = buffer;
    return result;
}

static char* parseInput(char* input)
{
    int bufSize = TOKEN_BUFSIZE, position = 0;
    char* token;
    char** tokens = malloc(bufSize * sizeof(char*));

    token = strtok(input, TOK_DELIMITER);
    while(token != NULL){
        tokens[position] = token;
        position++;

        if(position >= bufSize){
            //too many args
            break;
        }

        token = strtok(NULL, TOK_DELIMITER);
    }

    tokens[position] = NULL;
    char* result = "";

    if(strcmp(tokens[0], "help\n") == 0){
        result = UDP_help();
    }
    else if(strcmp(tokens[0], "count\n") == 0){
        result = UDP_getCount();
    }
    else if(strcmp(tokens[0], "get") == 0){
        int num = atoi(tokens[1]);
        result = UDP_getLastN(num);
    }
    else if(strcmp(tokens[0], "length\n") == 0){
        result = UDP_getLength();
    }
    else if(strcmp(tokens[0], "history\n") == 0){
        result = UDP_getHistory();
    }
    else if(strcmp(tokens[0], "dips\n") == 0){
        result = UDP_getDips();
    }
    else{
        result = "Unknown Command.\n";
    }
    
    free(tokens);
    return result;
}