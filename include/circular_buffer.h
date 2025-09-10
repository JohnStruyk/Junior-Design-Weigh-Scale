#ifndef CIRCULAR_BUFFER_H
#define	CIRCULAR_BUFFER_H

#ifdef	__cplusplus
extern "C" {
#endif
void putVal(int newValue);	// add a new value to the buffer
int getAvg();          	// average all buffer vals
void initBuffer();		// set all buffer vals to zero
#ifdef	__cplusplus
}
#endif

#endif	/* CIRCULAR_BUFFER_H */

