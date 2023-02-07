#include <stdbool.h>
#include <stdlib.h>

//Implemented using the guide from:
//https://embeddedartistry.com/blog/2017/05/17/creating-a-circular-buffer-in-c-and-c/

//circular buffer structure
typedef struct Cbuff Cbuff;
//Handle for the user to interact with
typedef Cbuff* Cbuff_t;


Cbuff_t Cbuff_init(int* buffer, int size);

//free the buffer
void Cbuff_free(Cbuff_t buf);

//reset the buffer to empty; head == tail
void Cbuff_reset(Cbuff_t buf);

//put data into the buffer
void Cbuff_put(Cbuff_t buf, int value);

//get data from the buffer
//returns 0 on success, -1 if the buffer is empty
int Cbuff_get(Cbuff_t buf);

//returns true if the buffer is empty
bool Cbuff_isEmpty(Cbuff_t buf);

//returns true if the buffer is full
bool Cbuff_isFull(Cbuff_t buf);

//returns the max capacity of the buffer
int Cbuff_capacity(Cbuff_t buf);

//returns the number of elements in the buffer
int Cbuff_size(Cbuff_t buf);

//change the size of the buffer
void Cbuff_set_size(Cbuff_t buf, int newSize);

//get a copy of the buffer
int* Cbuff_getHistory(Cbuff_t buf);