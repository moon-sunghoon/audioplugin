
#ifndef allp_h
#define allp_h
#include <stdio.h>
#include <string.h>

class allp {
public:
	allp();
	~allp();
	void reset();
	double audioprocessing(double input);
	double atapout(void);
	double allpout(void);


	void Buffersize(double sampleRate);
	void setdelaytime(double sampleRate, int _delay);
	void setgainparams(const double a);
	double getgainparams();
	

	int bfsize;
	double out;
	double in;
	
	
	double* dline;
	
	
	int fsConverted;
	int delay;
	double d_out;
	double d_in;
	double gain;
	
private:

	int wIndex;
	int rIndex;
	
};

#endif
