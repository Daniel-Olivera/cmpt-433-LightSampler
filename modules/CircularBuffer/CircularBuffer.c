#include <stdbool.h>

#define SIZE_OF_BUFFER 10

static int bufferLength = 0;

static int head = 0;
static int tail = 0;
static bool isFull = false;



void push(int item)
{
    if(isBufferFull()){
        //cannot add new elem
    }
    else{
        buffer[head] = item;
        bufferLength++;
        head = (head+1) % SIZE_OF_BUFFER;
        if(head == tail){
            isFull = true;
        }
    }
}

void pop(void)
{
    if(isEmpty()){
        //no element to read
    }
    else{
        int data = buffer[tail];
        tail = (tail+1) % SIZE_OF_BUFFER;
        isFull = false;
    }
}

bool isBufferFull(void)
{
    return isFull;
}

bool isEmpty(void)
{
    return (head == tail) && (isFull == false);
}