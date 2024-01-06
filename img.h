#ifndef IM_H
#define IM_H

#include <stdio.h>
#include <stdlib.h>

// debug mod
#include "debug.h"

typedef struct
{
  char r;
  char g;
  char b;
} color;

typedef struct
{
  long height;
  long width;
  unsigned char* mat;
} im;

void putpixel(im* img, long x, long y, color* c);
void getpixel(im* img, long x, long y, color* c);
void save(char* nom, im* img);
im* make_im(long width, long height, color* c);

void free_im(im* img);
#endif
/*
|========> (x)
|
v
(y)
*/
