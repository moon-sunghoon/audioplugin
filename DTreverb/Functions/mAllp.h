
#ifndef mAllp_h
#define mAllp_h
#include <stdio.h>
#include <string.h>

class MAllp {
public:
	MAllp();
	~MAllp();
	void reset();
	double audioprocessing(double input);
	double MAllpout(void);
	void excursion(double a);
	void Buffersize(double sampleRate);
	void setdelaytime(double sampleRate, int _delay);
	void setgainparams(const double a);
	double getgainparams();


	int bfsize;
	double out;
	double in;
	
	double* dline;
	
	double fsConverted;
	int delay;
	double d_out;
	double d_in;
	double sine_float;
	double sine_int;
	double fs;
	double gain;
private:

	int wIndex;
	int rIndex;
};

#endif

