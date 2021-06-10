#include "DelayLine.h"
#include <cmath>
delayline::delayline()
//constructor
{
	bfsize = 48000;
	rIndex = wIndex = 0;
	dline = new double[bfsize];  //dynamic allocation, first 48000, will be reset respectively in reset function in plugincore.cpp
	delay = 1; 
	reset();
	
}
delayline::~delayline()
//destructor
{
	delete[] dline;  //destructor delete the storage of the buffer
}

void delayline::reset()
{
	rIndex = wIndex = 0;   //reset the pointer to 0
	memset(dline, 0, bfsize * sizeof(double)); // fill the buffer with 0
	
}
void delayline::Buffersize(double samplingRate)
{
	delete[] dline;
	bfsize = samplingRate;
	dline = new double[bfsize];  //reset buffersize for each delayline
	reset();
}

void delayline::setdelaytime(double sampleRate, int _delay) {
	fsConverted = round(sampleRate / 29761);   //reset different delaytiime for each delayline, 29761 is the sampling rate of original dattorro's reverb
	delay = _delay * fsConverted;
	reset();
}

void delayline::setdelayparams(const int a) 
//function for the delay parameter
{
	
	delay = a;
	

}

double delayline::getdelayparams() 
//debug
{

	return(delay);

}


double delayline::audioprocessing(double input)
{
	rIndex = wIndex - delay; //read pointer to set a delay time
	if (rIndex < 0) rIndex += bfsize; //wrap when the read pointer is smaller than 0
	out = dline[rIndex]; //place output into the buffer with read pointer
	dline[wIndex] = input; // place input into the buffer with write pointer
	wIndex++; //increment write pointer
	if (wIndex >= bfsize) wIndex = 0;  // when the write pointer reaches the maximum size of the buffer, it goes back to 0
	tap = input; //tap output
	return out;

}
double delayline::delayout(void) 
//just for debugging
{
	return (out);
}
double delayline::dtapout(void) 
// tap output at the final final
{
		return (tap);
}
		

