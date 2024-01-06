#include "img.h"
#include "fractals.h"

int main(int argc, char **argv) {
  //pt p1 = {-1.418841, -0.001215};
  //pt p2 = {-1.415852, 0.001188};
  pt p1 = {-1.8, -0.09};
  pt p2 ={-1.7, 0.01};
  im* img = render_fractal(p1, p2, 10000, 0);
  save("test.ppm", img);

  #ifdef DEBUG
  extern int nbfree, nballoc;
  printf("%i free pour %i malloc\n", nbfree, nballoc);
  #endif
}
