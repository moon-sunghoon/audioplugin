
#ifndef Dezip_h
#define Dezip_h

#include <stdio.h>
class DeZipper {
public:
	DeZipper();
	double smooth(double sample);
private:
	double DZMM;
	double DZFB;
	double DZFF;
};

#endif