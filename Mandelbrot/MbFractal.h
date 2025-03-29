#ifndef MANDELBROTFRACTAL_H
#define MANDELBROTFRACTAL_H

typedef struct navigation_t
{
    float offset_y;
    float offset_x;
    float scale;
} navigation_t;

const int NITERATIONMAX   = 256;

const float SQUARED_R_MAX = 10 * 10;

int   RunMandelbrotFractal           (char* mode, int ntimes = 1);

void  Navigation                     (navigation_t* nvg);

void  CommonCalculateMandelbrot      (sf::VertexArray* points, int ntimes, float offset_x, float offset_y, float scale);

void  IntrinsicsCalculateMandelbrot  (sf::VertexArray* points, int ntimes, float offset_x, float offset_y, float scale);

#endif
