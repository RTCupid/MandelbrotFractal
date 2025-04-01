#ifndef MANDELBROTFRACTAL_H
#define MANDELBROTFRACTAL_H

#include <xmmintrin.h>
// nav

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

const int      SIZE_SCREEN_X    = 800;

const int      SIZE_SCREEN_Y    = 400;

const uint32_t MASK_FFFFFFFF    = 4294967295;

int   RunMandelbrotFractal           (char* mode, int ntimes = 1);

void  Navigation                     (navigation_t* nvg);

void  CommonCalculateMandelbrot      (sf::VertexArray* points, int ntimes, float offset_x, float offset_y, float scale, float dx, float dy);

void  IntrinsicsCalculateMandelbrot  (sf::VertexArray* points, int ntimes, float offset_x, float offset_y, float scale, float dx, float dy,
                                                                    __m128 niterationmax, __m128 squared_r_max, __m128 array_dx_scale_index, __m128 mask_ffffffff);

void  SetArrayIndex                  (float array_index[NUMBER_POINTS_IN_PACK]);

void  PrintArray                     (float array[NUMBER_POINTS_IN_PACK]);

void  MyIncIter                      (__m128* niteration, __m128 cmp);

inline bool  mm_cmple_ps111          (float first_array[NUMBER_POINTS_IN_PACK], float second_array[NUMBER_POINTS_IN_PACK]);

inline void  mm_cmple_ps             (float dst[NUMBER_POINTS_IN_PACK], float first_array[NUMBER_POINTS_IN_PACK], float second_array[NUMBER_POINTS_IN_PACK]);

inline void  mm_set_ps1              (float dst[NUMBER_POINTS_IN_PACK], float value);

inline void  mm_set_ps               (float dst[NUMBER_POINTS_IN_PACK], float par1, float par2, float par3, float par4);

inline void  mm_add_ps               (float dst[NUMBER_POINTS_IN_PACK], float first_array[NUMBER_POINTS_IN_PACK], float second_array[NUMBER_POINTS_IN_PACK]);

inline void  mm_sub_ps               (float dst[NUMBER_POINTS_IN_PACK], float first_array[NUMBER_POINTS_IN_PACK], float second_array[NUMBER_POINTS_IN_PACK]);

inline void  mm_mul_ps               (float dst[NUMBER_POINTS_IN_PACK], float first_array[NUMBER_POINTS_IN_PACK], float second_array[NUMBER_POINTS_IN_PACK]);

inline int   mm_movemask_ps          (float array[NUMBER_POINTS_IN_PACK]);

#endif
