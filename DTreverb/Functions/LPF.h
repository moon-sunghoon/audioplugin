#ifndef LPF_h
#define LPF_h
#include <stdio.h>
#include <string.h>

class LowpassFilter {
public:
	LowpassFilter();
	~LowpassFilter();
	void reset();
	double audioprocessing(double input);
	double lpfout(void);
	void Buffersize(double sampleRate);
	void setcutoffparams(const double a);
	double getcutoffparams();

	int bfsize;
	double out;
	double in;
	double* dline;
	double cutoff;
	double d_out;
	double gain;
private:

	int wIndex;
	int rIndex;
};

#endif
