#include "Dezip.h"
DeZipper::DeZipper()
{
	DZMM = 0.0;
	DZFB = 0.999;
	DZFF = 1 - DZFB;

}
double DeZipper::smooth(double sample) {
	double temp = DZFF * sample + DZFB*DZMM;
	DZMM = temp;

	return temp;
}