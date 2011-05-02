#ifndef _MATH_H
#define _MATH_H

#include <time.h>

const double PI			= 3.1415926536;
const double HALF_PI	= 0.5*PI;
const double TWO_PI		= 2.0*PI;
const float DEG_TO_RAD = float(PI/180.0f);
static int lastRandom = 0;
inline float randomf(float minf, float maxf){ 
	//srand(unsigned int(clock()));
	lastRandom = rand();
	return float((double(lastRandom)/RAND_MAX)*(maxf-minf)) + minf; 
}

#endif