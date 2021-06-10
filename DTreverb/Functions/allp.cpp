#include "allp.h"
#include <cmath>
//comment for the basic strucutre of methods are cited in the Delayline.cpp
allp::allp()
{
	bfsize = 48000;
	rIndex = wIndex = 0;
	dline = new double[bfsize];
	gain = 0.5;
	delay = 1;
	reset();

}
allp::~allp()
{
	delete[] dline;
}

void allp::reset()
{
	rIndex = wIndex = 0;
	memset(dline, 0, bfsize * sizeof(double));

}
void allp::Buffersize(double samplingRate)
{
	delete[] dline;
	bfsize = samplingRate;
	dline = new double[bfsize];
	reset();
}


void allp::setdelaytime(double sampleRate, int _delay)
{
	fsConverted = round(sampleRate / 29761);
	delay = _delay * fsConverted;
	reset();
}

void allp::setgainparams(const double a) {
	
	gain = a;

}

double allp::getgainparams() {
	
	return(gain);

}

double allp::audioprocessing(double input)
{
	rIndex = wIndex - delay;
	if (rIndex < 0) rIndex += bfsize;

	d_out = dline[rIndex]; //output of delayline
	d_in = input + d_out * -gain; //input of delayline
	out = d_in * gain + d_out; //output 
	dline[wIndex] = d_in; //put the tapin to the write pointer

	wIndex++;
	if (wIndex >= bfsize) wIndex = 0;
	return out;
}

double allp::allpout(void) {
	return (out);
}

double allp::atapout(void) {
	return (d_in);
}
