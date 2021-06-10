#define _USE_MATH_DEFINES
#include "TLPF.h"
#include <cmath>
//comment for the basic strucutre of methods are cited in the Delayline.cpp
TLowpassFilter::TLowpassFilter()
{
	bfsize = 48000;
	rIndex = wIndex = 0;
	dline = new double[bfsize];
	
	gain = 0.9;
	reset();

}
TLowpassFilter::~TLowpassFilter()
{
	delete[] dline;
}

void TLowpassFilter::reset()
{
	rIndex = wIndex = 0;
	memset(dline, 0, bfsize * sizeof(double));

}
void TLowpassFilter::Buffersize(double samplingRate)
{
	delete[] dline;
	bfsize = samplingRate;
	dline = new double[bfsize];
	reset();
}
void TLowpassFilter::setgainparams(const double a) {

	gain = a;

}

double TLowpassFilter::getgainparams() {

	return(gain);

}
double TLowpassFilter::audioprocessing(double input)
{
	rIndex = wIndex - 1;
	if (rIndex < 0) rIndex += bfsize;


	out = input *  gain + dline[rIndex] *(1- gain); //gain form is reversed 
	dline[wIndex] = out;

	wIndex++;
	if (wIndex >= bfsize) wIndex = 0;

	return out;
}

double TLowpassFilter::lpfout(void) {
	return (out);
}


