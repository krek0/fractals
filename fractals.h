#ifndef FR_H
#define FR_H

#include "img.h"
#include "debug.h"
#include <pthread.h>
#include <stdlib.h>
#include <math.h>

typedef struct
{
  long double x;
  long double y;
} pt;

int _mande_conv(pt c);
im* render_fractal(pt p1, pt p2, int size, int sample);

#endif
