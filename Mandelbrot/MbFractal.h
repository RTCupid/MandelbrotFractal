#ifndef MANDELBROTFRACTAL_H
#define MANDELBROTFRACTAL_H

// nav

typedef struct navigation_t
{
    float offset_y;
    float offset_x;
    float scale;
} navigation_t;

const int   NITERATIONMAX = 256;

const float SQUARED_R_MAX = 10 * 10;

const int   FACTOR_D      = 400;

const int   SIZE_SCREEN_X = 1200;

const int   SIZE_SCREEN_Y = 800;

int   RunMandelbrotFractal           (char* mode, int ntimes = 1);

void  Navigation                     (navigation_t* nvg);

void  CommonCalculateMandelbrot      (sf::VertexArray* points, int ntimes, float offset_x, float offset_y, float scale);

void  IntrinsicsCalculateMandelbrot  (sf::VertexArray* points, int ntimes, float offset_x, float offset_y, float scale);

#endif
