#ifndef MANDELBROTFRACTAL_H
#define MANDELBROTFRACTAL_H

#include <SFML/Graphics.hpp>
#include <xmmintrin.h>

typedef struct navigation_t
{
    float offset_y;
    float offset_x;
    float scale;
} navigation_t;

const int NUMBER_POINTS_IN_PACK = 4;

const float    NITERATIONMAX    = 256;

const float    SQUARED_R_MAX    = 10 * 10;

const int      FACTOR_D         = 400;

const int      SIZE_SCREEN_X    = 1200;

const int      SIZE_SCREEN_Y    = 800;

int   RunMandelbrotFractal           (char* mode, int ntimes = 1);

void  Navigation                     (navigation_t* nvg);

void  CommonCalculateMandelbrot      (sf::VertexArray* points, int ntimes, float offset_x, float offset_y, float scale, float dx, float dy);

void  IntrinsicsCalculateMandelbrot  (sf::VertexArray* points, int ntimes, float offset_x, float offset_y, float scale, float dx, float dy,
                                                                    __m128 niterationmax, __m128 squared_r_max, __m128 array_dx_scale_index);

void  PrintArray                     (float array[NUMBER_POINTS_IN_PACK]);

#endif
