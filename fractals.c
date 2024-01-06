#include "fractals.h"
#include "img.h"
#include <math.h>
#include <stdlib.h>

int _mande_conv(pt c) {
  int i = 0;
  long double x = c.x, y = c.y;
  long double x2 = x * x, y2 = y * y;

  while ((x2 + y2 <= 4) && i != 499) {
    y = 2 * x * y + c.y;
    x = x2 - y2 + c.x;
    x2 = x * x;
    y2 = y * y;
    i++;
  }
  return i;
}

int _ship_conv(pt c)
{
  int i = 0;
  long double x = c.x, y = c.y;
  long double x2 = x * x, y2 = y * y;

  while ((x2 + y2 <= 4) && i != 499) {
    //x = x2*x - 3*y2*x + c.x;
    //y = 3*x2*y-y2*y + c.y;
    y = 2 * x * y + c.y;
    x = x2 - y2 + c.x;
    y = fabsl(y);
    x = fabsl(x);
    x2 = x * x;
    y2 = y * y;
    i++;
  }
  return i;
}

void swap(long double *a, long double *b) {
  long double temp = *a;
  *a = *b;
  *b = temp;
}

typedef struct {
  int startLine;
  int endLine;
  im *img;
  long double scale;
  pt p1;
  color* colors;
  int sample_n;
} ThreadData;

void *render_segment(void *arg) {
  ThreadData *data = (ThreadData *)arg;
  pt c, c2;
  int value;
  int tb, tg, tr;
  color color;
  long double random_mult = data->scale/RAND_MAX;
  
  for (int y = data->startLine; y < data->endLine; y++)
  {
    c.y = data->p1.y + y * data->scale;
    for (int x = 0; x < data->img->width; x++)
    {
      c.x = data->p1.x + x * data->scale;
      // Multisampling antalisasing
      if (data->sample_n == 0)
      {
        value = _ship_conv(c);
        color = data->colors[value];
      }
      else
      {
        tb = 0; tg = 0; tr = 0;
        for (int i = 0; i < data->sample_n; i++)
        {
          c2.y = c.y + (double)random()*random_mult;
          c2.x = c.x + (double)random()*random_mult;
          value = _ship_conv(c2);
          color = data->colors[value];
          tr += color.r;
          tg += color.g;
          tb += color.b;
        }
        color.b = tr/data->sample_n;
        color.g = tg/data->sample_n;
        color.r = tb/data->sample_n;
      }
      putpixel(data->img, x, y, &color);
    }
  }

  return NULL;
}

im* render_fractal(pt p1, pt p2, int size, int sample)
{
  color* colors;
  colors = MALLOC(sizeof(unsigned char*)*500);
  for (int i = 0; i < 500; i ++)
  {
    colors[i].b = 255 - pow(cos(i / 63.66), 2) * 255;
    colors[i].g = 255 - pow(cos(i / 21.22), 2) * 255;
    colors[i].r = 255 - pow(cos(i / 12.73), 2) * 255;
  }
  
  if (p1.x > p2.x)
    swap(&p1.x, &p2.x);
  if (p1.y > p2.y)
    swap(&p1.y, &p2.y);

  long double scaleX = (p2.x - p1.x) / size;
  long double scaleY = (p2.y - p1.y) / size;
  long double scale = fmax(scaleX, scaleY);

  int width = (p2.x - p1.x) / scale;
  int height = (p2.y - p1.y) / scale;
  
  im* img = make_im(width, height, &colors[0]);

  const int numThreads = 16;
  pthread_t threads[numThreads];
  ThreadData threadData[numThreads];

  for (int i = 0; i < numThreads; i++) {
    threadData[i].startLine = height * i / numThreads;
    threadData[i].endLine = height * (i + 1) / numThreads;
    threadData[i].img = img;
    threadData[i].scale = scale;
    threadData[i].p1 = p1;
    threadData[i].colors = colors;
    threadData[i].sample_n = sample;

    pthread_create(&threads[i], NULL, render_segment, &threadData[i]);
  }

  for (int i = 0; i < numThreads; i++) {
    pthread_join(threads[i], NULL);
  }

  FREE(colors);
  return img;
}
