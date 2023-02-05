#include "CircularBuffer.h"

struct Cbuff {
    int * buffer;
    int head;
    int tail;
    int max;
    bool full;
};

static void advance_pointer(Cbuff_t buf);
static void retreat_pointer(Cbuff_t buf);

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
    assert(buf);
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

int Cbuff_get(Cbuff_t buf, int * data)
{
    int result = -1;

    if(!Cbuff_isEmpty(buf)){
        *data = buf->buffer[buf->tail];
        retreat_pointer(buf);

        result = 0;
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