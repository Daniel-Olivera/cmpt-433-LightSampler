#include "CircularBuffer.h"
#include <string.h>
#include <stdio.h>

struct Cbuff {
    int * buffer;
    int head;
    int tail;
    int max;
    bool full;
};

static void advance_pointer(Cbuff_t buf);
static void retreat_pointer(Cbuff_t buf);
static int min(int a, int b);

Cbuff_t Cbuff_init(int* buffer, int size)
{
    Cbuff_t cbuf = malloc(sizeof(Cbuff));

    cbuf->buffer = buffer;
    cbuf->max = size;
    Cbuff_reset(cbuf);

    return cbuf;
}

void Cbuff_free(Cbuff_t buf)
{
    free(buf);
}

void Cbuff_reset(Cbuff_t buf)
{
    buf->head = 0;
    buf->tail = 0;
    buf->full = false;
}

void Cbuff_put(Cbuff_t buf, int value)
{
    buf->buffer[buf->head] = value;
    advance_pointer(buf);
}

int Cbuff_get(Cbuff_t buf)
{
    int result = -1;

    if(!Cbuff_isEmpty(buf)){
        result = buf->buffer[buf->tail];
        retreat_pointer(buf);
    }

    return result;
}


bool Cbuff_isEmpty(Cbuff_t buf)
{
    return (!buf->full && (buf->head == buf->tail));
}

bool Cbuff_isFull(Cbuff_t buf)
{
    return buf->full;
}

int Cbuff_capacity(Cbuff_t buf)
{
    return buf->max;
}

int Cbuff_size(Cbuff_t buf)
{
    int size = buf->max;

    if(!buf->full){
        if(buf->head >= buf->tail){
            size = (buf->head - buf->tail);
        }
        else{
            size = (buf->max + buf->head - buf->tail);
        }
    }

    return size;
}

static void advance_pointer(Cbuff_t buf)
{
    if(buf->full){
        if(++(buf->tail) == buf->max){
            buf->tail = 0;
        }
    }

    if(++(buf->head) == buf->max){
        buf->head = 0;
    }

    buf->full = (buf->head == buf->tail);
}

static void retreat_pointer(Cbuff_t buf)
{
    buf->full = false;
    if(++(buf->tail) == buf->max){
        buf->tail = 0;
    }
}

void Cbuff_set_size(Cbuff_t buf, int newSize)
{
    
    //allocate new buffer of new size
    int * newBuffer = malloc((newSize + 1) * sizeof(int));

    //get min size between the 2 buffers
    int minSize = min((newSize+1) * sizeof(int), Cbuff_size(buf) * sizeof(int));

    //copy old buffer into the new one
    memcpy(newBuffer, buf->buffer, minSize);

    //set the buffer to be the new buffer
    buf->buffer = newBuffer;
    buf->max = newSize;
}

int* Cbuff_getHistory(Cbuff_t buf)
{
    int * result = malloc(Cbuff_size(buf) * sizeof(int));
    memcpy(result, buf->buffer, Cbuff_size(buf) * sizeof(int));
    return result;
}

static int min(int a, int b)
{
    if(a < b)
        return a;
    else   
        return b;
}