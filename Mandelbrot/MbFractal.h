#ifndef MANDELBROTFRACTAL_H
#define MANDELBROTFRACTAL_H

typedef struct navigation_t
{
    float offset_y;
    float offset_x;
    float scale;
} navigation_t;

const int NITERATIONMAX = 256;

const float SQUARED_R_MAX = 10 * 10;

int  RunMandelbrotFractal (int ntimes = 1);

void Navigation           (navigation_t* nvg);

void CalculateMandelbrot  (sf::VertexArray* points, float offset_x, float offset_y, float scale);

#endif
