#define _USE_MATH_DEFINES
#include "LPF.h"
#include <cmath>
//comment for the basic strucutre of methods are cited in the Delayline.cpp
LowpassFilter::LowpassFilter()
{
	bfsize = 48000;
	rIndex = wIndex = 0;
	dline = new double[bfsize];
	cutoff = 200;
	gain = 0.9;
	reset();

}
LowpassFilter::~LowpassFilter()
{
	delete[] dline;
}

void LowpassFilter::reset()
{
	rIndex = wIndex = 0;
	memset(dline, 0, bfsize * sizeof(double));

}
void LowpassFilter::Buffersize(double samplingRate)
{
	delete[] dline;
	bfsize = samplingRate;
	dline = new double[bfsize];
	reset();
}
void LowpassFilter::setcutoffparams(const double a) {

	cutoff = a;

}

double LowpassFilter::getcutoffparams() {

	return(cutoff);

}
double LowpassFilter::audioprocessing(double input)
{
	rIndex = wIndex - 1; //one sample behind
	if (rIndex < 0) rIndex += bfsize;

	gain = exp(-2 * M_PI * (cutoff / bfsize));
	
	out = input * (1-gain) + dline[rIndex] * gain;
	dline[wIndex] = out;

	wIndex++;
	if (wIndex >= bfsize) wIndex = 0;

	return out;
}

double LowpassFilter::lpfout(void) {
	return (out);
}


