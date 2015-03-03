#ifndef NOISE_H
#define NOISE_H

#include <math.h>
#include <stdlib.h>
#include <string.h>

#define F2 0.3660254037844386f
#define G2 0.21132486540518713f
#define F3 (1.0f / 3.0f)
#define G3 (1.0f / 6.0f)
/*
const static float GRAD3[16][3];
static unsigned char PERM[];*/

void seed(unsigned int);
float noise2(float, float);
float simplex2(float, float, float, float, float);

#endif
