#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <SFML/Graphics.hpp>

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
            CommonCalculateMandelbrot     (&points, ntimes, nvg.offset_x, nvg.offset_y, nvg.scale);
        }
        else if (strcmp (mode, "intrinsics") == 0)
        {
            IntrinsicsCalculateMandelbrot (&points, ntimes, nvg.offset_x, nvg.offset_y, nvg.scale);
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

void IntrinsicsCalculateMandelbrot (sf::VertexArray* points, int ntimes, float offset_x, float offset_y, float scale)
{
    float dx = (float)1 / FACTOR_D, dy = (float)1 / FACTOR_D;

    for (int itest = 0; itest < ntimes; itest++)
    {
        assert (itest < ntimes);

        for (int iy = 0; iy < SIZE_SCREEN_Y; iy++)
        {
            //fprintf (stderr, BLU "iy = <%d>" RESET, iy);
            assert (iy < SIZE_SCREEN_Y);

            float X0 = -2 + offset_x ;                                      //; start from upper left cornel
            float Y0 =  1 + offset_y - (float)iy * dy * scale;

            for (int ix = 0; ix < SIZE_SCREEN_X; ix += NUMBER_POINTS_IN_PACK, X0 += NUMBER_POINTS_IN_PACK * dx * scale)
            {
                //fprintf (stderr, BLU "ix = <%d>" RESET, ix);
                assert (ix < SIZE_SCREEN_X);

                float X[NUMBER_POINTS_IN_PACK] = {X0, X0 + dx * scale, X0 + 2 * dx * scale, X0 + 3 * dx * scale};
                float Y[NUMBER_POINTS_IN_PACK] = {Y0, Y0             , Y0                 , Y0                 };

                float niteration[NUMBER_POINTS_IN_PACK] = {};

                float        cmp[NUMBER_POINTS_IN_PACK] = {};
                mm_set_ps1  (cmp,          1);

                float niterationmax[NUMBER_POINTS_IN_PACK] = {};
                mm_set_ps1 (niterationmax, NITERATIONMAX);

                for (; mm_cmple_ps (niteration, niterationmax); mm_add_ps (niteration, niteration, cmp))
                {
                    //printf ("niteration = %d\n", (int) niteration[1]);
                    float squared_X[NUMBER_POINTS_IN_PACK] = {};
                    float squared_Y[NUMBER_POINTS_IN_PACK] = {};
                    float       X_Y[NUMBER_POINTS_IN_PACK] = {};
                    float squared_r[NUMBER_POINTS_IN_PACK] = {};

                    int index = 0;

                    mm_mul_ps (squared_X, X, X);
                    mm_mul_ps (squared_Y, Y, Y);
                    mm_mul_ps (      X_Y, X, Y);

                    mm_add_ps (squared_r, squared_X, squared_Y);

                    for (index = 0; index < NUMBER_POINTS_IN_PACK; index++)
                    {
                        if (squared_r[index]  >= SQUARED_R_MAX) cmp[index] = 0;
                    }

                    if (cmp[0] == 0 && cmp[1] == 0 && cmp[2] == 0 && cmp[3] == 0)
                    {
                        break;
                    }

                    for (index = 0; index < NUMBER_POINTS_IN_PACK; index++)
                    {
                        X[index] = squared_X[index] - squared_Y[index] + X0 + dx * (float)index * scale;
                    }

                    for (index = 0; index < NUMBER_POINTS_IN_PACK; index++)
                    {
                        Y[index] =       X_Y[index] +       X_Y[index] + Y0;
                    }
                }

                for (int index = 0; index < NUMBER_POINTS_IN_PACK; index++)
                {
                    (*points)[(size_t)(iy * SIZE_SCREEN_X + ix + index)].position = sf::Vector2f(static_cast<float>(ix + index), static_cast<float>(iy));
                    (*points)[(size_t)(iy * SIZE_SCREEN_X + ix + index)].color    = sf::Color((sf::Uint8)(256 - (int) niteration[index] * 16), 0, (sf::Uint8)(256 - (int) niteration[index] * 16));
                }
            }
        }
    }
    return;
}

void mm_mul_ps (float dst[NUMBER_POINTS_IN_PACK], float first_array[NUMBER_POINTS_IN_PACK], float second_array[NUMBER_POINTS_IN_PACK])
{
    for (int index = 0; index < NUMBER_POINTS_IN_PACK; index++)
    {
        dst[index] = first_array[index] * second_array[index];
    }
    return;
}

void mm_add_ps (float dst[NUMBER_POINTS_IN_PACK], float niteration[NUMBER_POINTS_IN_PACK], float cmp[NUMBER_POINTS_IN_PACK])
{
    for (int index = 0; index < NUMBER_POINTS_IN_PACK; index++)
    {
        dst[index] = niteration[index] + cmp[index];
    }
    return;
}

void mm_set_ps1 (float niterationmax[NUMBER_POINTS_IN_PACK], float value)
{
    for (int index= 0; index < NUMBER_POINTS_IN_PACK; index++)
    {
        niterationmax[index] = value;
    }
    return;
}


bool mm_cmple_ps (float niteration[NUMBER_POINTS_IN_PACK], float niterationmax[NUMBER_POINTS_IN_PACK])
{
    for (int index = 0; index < NUMBER_POINTS_IN_PACK; index++)
    {
        if (niteration[index] >= niterationmax[index]) return 0;
    }
    return 1;
}

void CommonCalculateMandelbrot (sf::VertexArray* points, int ntimes, float offset_x, float offset_y, float scale)
{
    float dx = (float)1 / FACTOR_D, dy = (float)1 / FACTOR_D;

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
