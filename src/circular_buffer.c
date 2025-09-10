#include "xc.h"
#include "../include/circular_buffer.h"

#define BUFFER_SIZE 128

int writeIndx = 0;
int numElemsInBuffer = 0;
int buffer[BUFFER_SIZE];

void putVal(int newValue){ // add a new value to the buffer
    buffer[writeIndx] = newValue;
    writeIndx++;
    writeIndx %= BUFFER_SIZE;
    if (numElemsInBuffer < BUFFER_SIZE) {
        numElemsInBuffer++;
    }
}	

int getAvg() { // average all buffer vals
    unsigned long int avg = 0;
    for (int i = 0; i < numElemsInBuffer; i++) {
        avg += buffer[i];
    }
    return avg/numElemsInBuffer;
}          	

void initBuffer() { // set all buffer vals to zero
    writeIndx = 0;
    numElemsInBuffer = 0;
    for (int i = 0; i < BUFFER_SIZE; i++) {
        buffer[i] = 0;
    }
}		
