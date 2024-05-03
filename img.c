#include "img.h"

// Write a image
void save(char* name, im* img)
{
  FILE* file = fopen(name, "wb");
  fprintf(file, "P6\n"); // magic number
  fprintf(file, "%li %li\n", img->width, img->height); // size
  fprintf(file, "%i\n", 255); // max color
  
  // Write data
  unsigned char* p = img->mat;
  size_t dataSize = img->width * img->height * 3;
  fwrite(p, 1, dataSize, file);

  fclose(file);
}

im* load(char* name, im* img)
{
	FILE* file = fopen(name, "rb");
	// Check if this file exist and start with the good magic number
	if (file == NULL || fgetc(file) != 'P' || fgetc(file) != '6')
    	return NULL;
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

// Create a image (with default color c, NULL to write nothing)
im* make_im(long width, long height, color* c)
{
  unsigned char* mat = malloc(3*height*width);
  if (c != NULL) // if a default color is fixed
  {
    
    unsigned char* pt = mat;
    for (long i = 0; i < width*height; i++)
    {
      *pt++ = c->r;
      *pt++ = c->g;
      *pt++ = c->b;
    }
  }
  // create img and write image data
  im* img = malloc(sizeof(im));
  img->mat=mat;
  img->height=height;
  img->width=width;
  
  return img;
}

void free_im(im* img){
  free(img->mat);
  free(img);
}
