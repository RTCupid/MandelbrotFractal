#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <SFML/Graphics.hpp>
#include <xmmintrin.h>
#include <immintrin.h>
#include <emmintrin.h>
#include <smmintrin.h>

#include "MbFractal.h"
#include "../common/colors.h"

int RunMandelbrotFractal (char* mode, int ntimes)
{
    printf (GRN "ntimes = <%d>\n" RESET, ntimes);

    sf::RenderWindow window (sf::VideoMode(SIZE_SCREEN_X, SIZE_SCREEN_Y), "Brilliant graphics, amazing fractal!");

    sf::VertexArray points (sf::Points, SIZE_SCREEN_X * SIZE_SCREEN_Y);

    sf::Clock fpsClock;

    sf::Time elapsedTime = sf::Time::Zero;

    float Fps = 0;

    sf::Font font;
    if (!font.loadFromFile ("/usr/share/fonts/opentype/urw-base35/NimbusRoman-Bold.otf"))
    {
        fprintf (stderr, RED "Unknown font\n" RESET);
    }

    sf::Text fpsText;
    fpsText.setFont (font);
    fpsText.setCharacterSize (20);
    fpsText.setFillColor (sf::Color::White);
    fpsText.setPosition (10, 10);

    navigation_t nvg {0, 0, 1};

    FILE* Measuring_FILE = fopen ("Measuring.txt", "w");
    if (!Measuring_FILE)
    {
        fprintf (stderr, RED "ERROR: can't open file to write \"Measuring.txt\"\n" RESET);
    }

    fprintf (Measuring_FILE, "Time of frame (seconds):\n");


    while (window.isOpen ())
    {
        sf::Event event;

        while (window.pollEvent (event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close ();
            }
        }

        Navigation (&nvg);

        fpsClock.restart ();

        if      (strcmp (mode, "common") == 0)
        {
            float dx = (float)1 / FACTOR_D, dy = (float)1 / FACTOR_D;

            CommonCalculateMandelbrot     (&points, ntimes, nvg.offset_x, nvg.offset_y, nvg.scale, dx, dy);
        }
        else if (strcmp (mode, "intrinsics") == 0)
        {
            float dx = (float)1 / FACTOR_D, dy = (float)1 / FACTOR_D;

            const __m128 array_index          = _mm_set_ps   (0, 1, 2, 3);

            const __m128 array_dx_scale       = _mm_set_ps1  (dx * nvg.scale);

            const __m128 array_dx_scale_index = _mm_mul_ps   (array_dx_scale, (__m128) array_index);

            const __m128 niterationmax        = _mm_set_ps1  (NITERATIONMAX);

            const __m128 squared_r_max        = _mm_set_ps1  (SQUARED_R_MAX);

            const __m128 mask_ffffffff        = _mm_set_ps1  (MASK_FFFFFFFF);

            IntrinsicsCalculateMandelbrot (&points, ntimes, nvg.offset_x, nvg.offset_y, nvg.scale, dx, dy, niterationmax, squared_r_max, array_dx_scale_index, mask_ffffffff);
        }
        else
        {
            fprintf (stderr, RED "Unknown mode: use mode: \"common\" or \"intrinsics\"\n" RESET);
            return 1;
        }

        elapsedTime = fpsClock.restart ();

        printf  (            BLU "elapsed time = <%f>\n" RESET, elapsedTime.asSeconds ());
        fprintf (Measuring_FILE, "%f\n",                        elapsedTime.asSeconds ());

        Fps = (float)1 / elapsedTime.asSeconds ();

        fpsText.setString  ("FPS: " + std::to_string (Fps));

        window.clear ();

        window.draw (points);                                        //; draw all points
        window.draw (fpsText);

        window.display ();
    }

    printf (GRN "End of main cycle of graphics and fractal\n" RESET);
    return 0;
}

void Navigation (navigation_t* nvg)
{
    if (sf::Keyboard::isKeyPressed (sf::Keyboard::Up))    nvg->offset_y += (float)0.05;
    if (sf::Keyboard::isKeyPressed (sf::Keyboard::Down))  nvg->offset_y -= (float)0.05;
    if (sf::Keyboard::isKeyPressed (sf::Keyboard::Left))  nvg->offset_x -= (float)0.05;
    if (sf::Keyboard::isKeyPressed (sf::Keyboard::Right)) nvg->offset_x += (float)0.05;

    if (sf::Keyboard::isKeyPressed (sf::Keyboard::P))     nvg->scale    /= (float)1.25;
    if (sf::Keyboard::isKeyPressed (sf::Keyboard::M))     nvg->scale    *= (float)1.25;

    return;
}

void IntrinsicsCalculateMandelbrot (sf::VertexArray* points, int ntimes, float offset_x, float offset_y, float scale, float dx, float dy,
                                                                __m128 niterationmax, __m128 squared_r_max, __m128 array_dx_scale_index, __m128 mask_ffffffff)
{
    for (int itest = 0; itest < ntimes; itest++)
    {
        assert (itest < ntimes);

        for (int iy = 0; iy < SIZE_SCREEN_Y; iy++)
        {
            assert (iy < SIZE_SCREEN_Y);

            float X0 = -2 + offset_x ;                                      //; start from upper left cornel
            float Y0 =  1 + offset_y - (float)iy * dy * scale;

            __m128 array_Y0 = _mm_set_ps1 (Y0);

            for (int ix = 0; ix < SIZE_SCREEN_X; ix += NUMBER_POINTS_IN_PACK, X0 += NUMBER_POINTS_IN_PACK * dx * scale)
            {
                assert (ix < SIZE_SCREEN_X);

                __m128 array_X0 = _mm_set_ps1 (X0);

                __m128 X = _mm_set_ps (X0, X0 + dx * scale, X0 + 2 * dx * scale, X0 + 3 * dx * scale);
                __m128 Y = _mm_set_ps (Y0, Y0             , Y0                 , Y0                 );

                __m128 niteration = _mm_set_ps1 (0);

                __m128 cmp        = _mm_set_ps1 (1);

                for (;; niteration = _mm_add_ps (niteration, cmp))
                {
                    cmp = _mm_cmple_ps (niterationmax, niteration);

                    int    mask     = _mm_movemask_ps (cmp);

                    if (mask) break;

                    __m128 squared_X = _mm_mul_ps (X, X);
                    __m128 squared_Y = _mm_mul_ps (Y, Y);
                    __m128       X_Y = _mm_mul_ps (X, Y);

                    __m128 squared_r = _mm_add_ps (squared_X, squared_Y);

                    cmp  = _mm_cmple_ps (squared_r, squared_r_max);

                    mask = _mm_movemask_ps (cmp);

                    if (!mask) break;

                    X = _mm_sub_ps (squared_X, squared_Y);
                    X = _mm_add_ps (X        , array_X0);
                    X = _mm_add_ps (X        , array_dx_scale_index);

                    Y = _mm_add_ps (X_Y, X_Y);
                    Y = _mm_add_ps (Y, array_Y0);

                    const __m128 one = _mm_set1_ps(1.0f); // или _mm_set_ps1(1.0f)

                    cmp = _mm_and_ps(cmp, one);
                }

                alignas (16) float number_iteration_float[4] = {};

                _mm_store_ps (number_iteration_float, niteration);

                for (int index = 0; index < NUMBER_POINTS_IN_PACK; index++)
                {
                    assert (index < NUMBER_POINTS_IN_PACK);

                    (*points)[(size_t)(iy * SIZE_SCREEN_X + ix + index)].position = sf::Vector2f(static_cast<float>(ix + index), static_cast<float>(iy));
                    (*points)[(size_t)(iy * SIZE_SCREEN_X + ix + index)].color    = sf::Color((sf::Uint8)(256 - (int)number_iteration_float[index] * 16), 0, (sf::Uint8)(256 - (int)number_iteration_float[index] * 16));
                }
            }
        }
    }
    return;
}

//                 alignas (16) float debug_niteration[4] = {};
//                 _mm_store_ps (debug_niteration, niteration);
//
//                  printf (GRN "after loop nit: nit[0] = %f, nit[1] = %f, nit[2] = %f, nit[3] = %f\n" RESET,
//                                        debug_niteration[0], debug_niteration[1], debug_niteration[2], debug_niteration[3]);

void PrintArray (float array[NUMBER_POINTS_IN_PACK])
{
    for (int index = 0; index < NUMBER_POINTS_IN_PACK; index++)
    {
        printf (YEL "%f | " RESET, array[index]);
    }
    printf ("\n");
}

void SetArrayIndex (float array_index[NUMBER_POINTS_IN_PACK])
{
    for (int index= 0; index < NUMBER_POINTS_IN_PACK; index++)
    {
        array_index[index] = (float)index;
    }
    return;
}

inline void mm_set_ps (float dst[NUMBER_POINTS_IN_PACK], float par1, float par2, float par3, float par4)
{
    dst[0] = par1;
    dst[1] = par2;
    dst[2] = par3;
    dst[3] = par4;
    return;
}

inline int mm_movemask_ps (float array[NUMBER_POINTS_IN_PACK])
{
    int mask = 0;

    for (int index = 0; index < NUMBER_POINTS_IN_PACK; index++)
    {
        if (array[index]) mask |= 1 << index;
    }

    return mask;
}

inline void mm_cmple_ps (float dst[NUMBER_POINTS_IN_PACK], float first_array[NUMBER_POINTS_IN_PACK], float second_array[NUMBER_POINTS_IN_PACK])
{
    for (int index = 0; index < NUMBER_POINTS_IN_PACK; index++)
    {
        if (first_array[index]  >= second_array[index])
        {
            dst[index] = 0;
        }
    }
    return;
}

inline void mm_mul_ps (float dst[NUMBER_POINTS_IN_PACK], float first_array[NUMBER_POINTS_IN_PACK], float second_array[NUMBER_POINTS_IN_PACK])
{
    for (int index = 0; index < NUMBER_POINTS_IN_PACK; index++)
    {
        dst[index] = first_array[index] * second_array[index];
    }
    return;
}

inline void mm_add_ps (float dst[NUMBER_POINTS_IN_PACK], float first_array[NUMBER_POINTS_IN_PACK], float second_array[NUMBER_POINTS_IN_PACK])
{
    for (int index = 0; index < NUMBER_POINTS_IN_PACK; index++)
    {
        dst[index] = first_array[index] + second_array[index];
    }
    return;
}

inline void mm_sub_ps (float dst[NUMBER_POINTS_IN_PACK], float first_array[NUMBER_POINTS_IN_PACK], float second_array[NUMBER_POINTS_IN_PACK])
{
    for (int index = 0; index < NUMBER_POINTS_IN_PACK; index++)
    {
        dst[index] = first_array[index] - second_array[index];
    }
    return;
}

inline void mm_set_ps1 (float dst[NUMBER_POINTS_IN_PACK], float value)
{
    for (int index= 0; index < NUMBER_POINTS_IN_PACK; index++)
    {
        dst[index] = value;
    }
    return;
}

inline bool mm_cmple_ps111 (float first_array[NUMBER_POINTS_IN_PACK], float second_array[NUMBER_POINTS_IN_PACK])
{
    for (int index = 0; index < NUMBER_POINTS_IN_PACK; index++)
    {
        if (first_array[index] >= second_array[index]) return 0;
    }
    return 1;
}

void CommonCalculateMandelbrot (sf::VertexArray* points, int ntimes, float offset_x, float offset_y, float scale, float dx, float dy)
{
    for (int itest = 0; itest < ntimes; itest++)
    {
        assert (itest < ntimes);

        for (int iy = 0; iy < SIZE_SCREEN_Y; iy++)
        {
            //fprintf (stderr, BLU "iy = <%d>" RESET, iy);
            assert (iy < SIZE_SCREEN_Y);

            float X0 = -2 + offset_x ;                                      //; start from upper left cornel
            float Y0 =  1 + offset_y - (float)iy * dy * scale;

            for (int ix = 0; ix < SIZE_SCREEN_X; ix++, X0 += dx * scale)
            {
                //fprintf (stderr, BLU "ix = <%d>" RESET, ix);
                assert (ix < SIZE_SCREEN_X);

                float X = X0;
                float Y = Y0;

                int niteration = 0;

                for (; niteration < (int) NITERATIONMAX; niteration++)
                {
                    float squared_X = X * X;
                    float squared_Y = Y * Y;
                    float       X_Y = X * Y;

                    float squared_r = squared_X + squared_Y;

                    if (squared_r  >= SQUARED_R_MAX)
                    {
                        break;
                    }

                    X = squared_X - squared_Y + X0;

                    Y =       X_Y +       X_Y + Y0;
                }

                (*points)[(size_t)(iy * SIZE_SCREEN_X + ix)].position = sf::Vector2f(static_cast<float>(ix), static_cast<float>(iy));
                (*points)[(size_t)(iy * SIZE_SCREEN_X + ix)].color    = sf::Color((sf::Uint8)(256 - niteration * 16), 0, (sf::Uint8)(256 - niteration * 16));
            }
        }
    }
    return;
}
