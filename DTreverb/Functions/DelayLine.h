#ifndef DelayLine_h
#define DelayLine_h
#include <stdio.h>
#include <string.h>

class delayline{
	public:
		delayline();
		
		void reset();
		void Buffersize(double sampleRate);
		void setdelaytime(double sampleRate, int _delay);
		
		double audioprocessing(double input);
		double dtapout(void);
		double delayout(void);
		void setdelayparams(const int a);
		double getdelayparams();
		double delaymsec;
		int fsConverted;
		int delay;
		int bfsize;
		double out;
		double in;
		
		double* dline;
		~delayline();
		double tap;
	private:
		
		int wIndex;
		int rIndex;
	};







#endif