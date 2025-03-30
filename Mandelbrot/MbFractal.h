#ifndef MANDELBROTFRACTAL_H
#define MANDELBROTFRACTAL_H

// nav

typedef struct navigation_t
{
    float offset_y;
    float offset_x;
    float scale;
} navigation_t;

const int NUMBER_POINTS_IN_PACK = 4;

const float NITERATIONMAX       = 256;

const float SQUARED_R_MAX       = 10 * 10;

const int   FACTOR_D            = 400;

const int   SIZE_SCREEN_X       = 1200;

const int   SIZE_SCREEN_Y       = 800;

int   RunMandelbrotFractal           (char* mode, int ntimes = 1);

void  Navigation                     (navigation_t* nvg);

void  CommonCalculateMandelbrot      (sf::VertexArray* points, int ntimes, float offset_x, float offset_y, float scale);

void  IntrinsicsCalculateMandelbrot  (sf::VertexArray* points, int ntimes, float offset_x, float offset_y, float scale);

bool  mm_cmple_ps111                    (float first_array[NUMBER_POINTS_IN_PACK], float second_array[NUMBER_POINTS_IN_PACK]);

void  mm_cmple_ps                    (float dst[NUMBER_POINTS_IN_PACK], float first_array[NUMBER_POINTS_IN_PACK], float second_array[NUMBER_POINTS_IN_PACK]);

void  mm_set_ps1                     (float dst[NUMBER_POINTS_IN_PACK], float value);

void  mm_add_ps                      (float dst[NUMBER_POINTS_IN_PACK], float first_array[NUMBER_POINTS_IN_PACK], float second_array[NUMBER_POINTS_IN_PACK]);

void  mm_sub_ps                      (float dst[NUMBER_POINTS_IN_PACK], float first_array[NUMBER_POINTS_IN_PACK], float second_array[NUMBER_POINTS_IN_PACK]);

void  mm_mul_ps                      (float dst[NUMBER_POINTS_IN_PACK], float first_array[NUMBER_POINTS_IN_PACK], float second_array[NUMBER_POINTS_IN_PACK]);

#endif
