#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <SFML/Graphics.hpp>
#include <xmmintrin.h>

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
    if (Measuring_FILE == NULL)
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
            CommonCalculateMandelbrot     (&points, ntimes, nvg);
        }
        else if (strcmp (mode, "intrinsics") == 0)
        {
            IntrinsicsCalculateMandelbrot (&points, ntimes, nvg);
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

    fclose (Measuring_FILE);

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

void IntrinsicsCalculateMandelbrot (sf::VertexArray* points, int ntimes, navigation_t nvg)
{
    float dx = (float)1 / FACTOR_D, dy = (float)1 / FACTOR_D;

    const __m128 array_dx_scale_index  = _mm_mul_ps   (_mm_set_ps1  (dx * nvg.scale), _mm_set_ps   (3.0f, 2.0f, 1.0f, 0.0f));

    const __m128 niterationmax         = _mm_set_ps1  (NITERATIONMAX);

    const __m128 squared_r_max         = _mm_set_ps1  (SQUARED_R_MAX);

          __m128 cmp                   = _mm_set_ps1  (1);

    for (int itest = 0; itest < ntimes; itest++)
    {
        for (int iy = 0; iy < SIZE_SCREEN_Y; iy++)
        {
            float X0 = START_X + nvg.offset_x ;                                      //; start from upper left cornel

            __m128 array_X0_dx_scale_index  = _mm_add_ps (_mm_set_ps1 (X0), array_dx_scale_index);

            __m128 array_Y0                 = _mm_set_ps1 (START_Y + nvg.offset_y - (float)iy * dy * nvg.scale);

            for (int ix = 0; ix < SIZE_SCREEN_X; ix += NUMBER_POINTS_IN_PACK, array_X0_dx_scale_index = _mm_add_ps (array_X0_dx_scale_index, _mm_set_ps1  (NUMBER_POINTS_IN_PACK * dx * nvg.scale)))
            {
                __m128 niteration = _mm_set_ps1 (0);

                __m128 X          = array_X0_dx_scale_index;
                __m128 Y          = array_Y0;

                while (true)
                {
                    __m128 squared_X = _mm_mul_ps (X, X);
                    __m128 squared_Y = _mm_mul_ps (Y, Y);
                    __m128       X_Y = _mm_mul_ps (X, Y);

                    __m128 squared_r = _mm_add_ps (squared_X, squared_Y);

                    cmp              = _mm_cmple_ps (squared_r, squared_r_max);

                    int mask         = _mm_movemask_ps (cmp);

                    if (!mask) break;

                    X          = _mm_add_ps (_mm_sub_ps (squared_X, squared_Y), array_X0_dx_scale_index);

                    Y          = _mm_add_ps (_mm_add_ps (X_Y, X_Y), array_Y0);

                    cmp        = _mm_and_ps(cmp, _mm_set_ps1(1.0f));

                    niteration = _mm_add_ps (niteration, cmp);

                    cmp        = _mm_cmple_ps (niterationmax, niteration);

                    mask       = _mm_movemask_ps (cmp);

                    if (mask) break;
                }

                alignas (16) float number_iteration_float[4] = {};

                _mm_store_ps (number_iteration_float, niteration);

                for (int index = 0; index < NUMBER_POINTS_IN_PACK; index++)
                {
                    (*points)[(size_t)(iy * SIZE_SCREEN_X + ix + index)].position = sf::Vector2f(static_cast<float>(ix + index), static_cast<float>(iy));
                    (*points)[(size_t)(iy * SIZE_SCREEN_X + ix + index)].color    = sf::Color((sf::Uint8)(256 - ((int)number_iteration_float[index]) * 16), 0, (sf::Uint8)(256 - ((int)number_iteration_float[index]) * 16));
                }
            }
        }
    }
    return;
}

void PrintArray (float array[NUMBER_POINTS_IN_PACK])
{
    for (int index = 0; index < NUMBER_POINTS_IN_PACK; index++)
    {
        printf (YEL "%f | " RESET, array[index]);
    }
    printf ("\n");
}

void CommonCalculateMandelbrot (sf::VertexArray* points, int ntimes, navigation_t nvg)
{
    float dx = (float)1 / FACTOR_D, dy = (float)1 / FACTOR_D;


    for (int itest = 0; itest < ntimes; itest++)
    {
        assert (itest < ntimes);

        for (int iy = 0; iy < SIZE_SCREEN_Y; iy++)
        {
            //fprintf (stderr, BLU "iy = <%d>" RESET, iy);
            assert (iy < SIZE_SCREEN_Y);

            float X0 = -2 + nvg.offset_x ;                                      //; start from upper left cornel
            float Y0 =  1 + nvg.offset_y - (float)iy * dy * nvg.scale;

            for (int ix = 0; ix < SIZE_SCREEN_X; ix++, X0 += dx * nvg.scale)
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
