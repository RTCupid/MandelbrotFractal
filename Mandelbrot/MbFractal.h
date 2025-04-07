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

const int START_X               = -2;

const int START_Y               = 1;

const float    NITERATIONMAX    = 256;

const float    SQUARED_R_MAX    = 10 * 10;

const int      FACTOR_D         = 400;

const int      SIZE_SCREEN_X    = 1200;

const int      SIZE_SCREEN_Y    = 800;

int   RunMandelbrotFractal           (char* mode, int ntimes = 1);

void  Navigation                     (navigation_t* nvg);

void  CommonCalculateMandelbrot      (sf::VertexArray* points, int ntimes, navigation_t nvg);

void  IntrinsicsCalculateMandelbrot  (sf::VertexArray* points, int ntimes, navigation_t nvg);

void  PrintArray                     (float array[NUMBER_POINTS_IN_PACK]);

#endif
