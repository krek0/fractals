#include "img.h"

void save(char* nom, im* img)
{
  FILE* file = fopen(nom, "wb");
  fprintf(file, "P6\n"); // magic number
  fprintf(file, "%li %li\n", img->width, img->height); // size
  fprintf(file, "%i\n", 255); // max color
  
  // Point the first pixel
  // write the colour then advance the pointer to next color
  // each 3 iteration p is on the next pixel
  unsigned char* p = img->mat;
  for (long i = 0; i < img->width*img->height*3; i++)
    fwrite (p++, 1, 1, file);
  fclose(file);
}

void putpixel(im* img, long x, long y, color* c)  
{
  // Point the first color of the (x,y) pixel
  unsigned char* p = &img->mat[(x+y*img->width)*3];
  // write color
  p[0] = c->r;
  p[1] = c->g;
  p[2] = c->b;
}

// Write the pixel color into c
void getpixel(im* img, long x, long y, color* c)
{
  unsigned char* p = &img->mat[(x+y*img->width)*3];
  c->r = p[0];
  c->g = p[1];
  c->b = p[2];
}

// Create a image (with default color c, NULL if no color)
im* make_im(long width, long height, color* c)
{
  unsigned char* mat = MALLOC(3*height*width);
  if (c != NULL) // if a default color is fixed
  {
    // Point the first pixel
    // write the colour then advance the pointer to next color
    // each loop iteration p is on the next pixel
    unsigned char* pt = mat;
    for (long i = 0; i < width*height; i++)
    {
      *pt++ = c->r;
      *pt++ = c->g;
      *pt++ = c->b;
    }
  }
  // create img and write image data
  im* img = MALLOC(sizeof(im));
  img->mat=mat;
  img->height=height;
  img->width=width;
  
  return img;
}

void free_im(im* img){
  FREE(img->mat);
  FREE(img);
}
