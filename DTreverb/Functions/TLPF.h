#ifndef TLPF_h
#define TLPF_h
#include <stdio.h>
#include <string.h>

class TLowpassFilter {
public:
	TLowpassFilter();
	~TLowpassFilter();
	void reset();
	double audioprocessing(double input);
	double lpfout(void);
	void Buffersize(double sampleRate);
	void setgainparams(const double a);
	double getgainparams();

	int bfsize;
	double out;
	double in;
	double* dline;
	
	double d_out;
	double gain;
private:

	int wIndex;
	int rIndex;
};

#endif

