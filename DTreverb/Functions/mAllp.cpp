#define _USE_MATH_DEFINES
#include "mAllp.h"
#include <cmath>
//comment for the basic strucutre of methods are cited in the Delayline.cpp
MAllp::MAllp()
{
	bfsize = 48000;
	rIndex = wIndex = 0;
	dline = new double[bfsize];
	gain = 0.5;
	delay = 1;
	reset();

}
MAllp::~MAllp()
{
	delete[] dline;
}

void MAllp::reset()
{
	rIndex = wIndex = 0;
	memset(dline, 0, bfsize * sizeof(double));

}
void MAllp::Buffersize(double samplingRate)
{
	delete[] dline;
	bfsize = samplingRate;
	dline = new double[bfsize];
	reset();
}

void MAllp::setdelaytime(double sampleRate, int _delay)
{
	fsConverted = round(sampleRate / 29761);
	delay = _delay * fsConverted;
	reset();
}



void MAllp::setgainparams(const double a) {

	gain = a;

}

double MAllp::getgainparams() {

	return(gain);

}
void MAllp::excursion(double sampleRate) {
	sine_float = 2 * (round(8*(sampleRate/29761)))* (sin(2 * M_PI * 1 / sampleRate) + 1); ///lfoIndex =1  1Hz LFO delay modulation; excursion = round(8*(converted samplerate) , referenced from dattorro's journal
	sine_int = ceil(sine_float);  //this will modulate the delaytime 
}    //29761 is the original sampling frequency of dattorro's reverb algorithm 


double MAllp::audioprocessing(double input)
{
	rIndex = wIndex - delay;
	if (rIndex < 0) rIndex += bfsize;

	d_out = dline[rIndex];
	d_in = input + d_out * gain; //the polarity of gain is reversed from normal all pass filter
	out = d_in * -gain + d_out;  
	dline[wIndex] = d_in;
	 
	wIndex++;
	rIndex = wIndex + sine_int; //modulated
	if (wIndex >= bfsize) wIndex = 0;

	return out;
}

double MAllp::MAllpout(void) {
	return (out);
}



