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

            for (int ix = 0; ix < SIZE_SCREEN_X; ix += 4, X0 += 4 * dx * scale)
            {
                //fprintf (stderr, BLU "ix = <%d>" RESET, ix);
                assert (ix < SIZE_SCREEN_X);

                float X[4] = {X0, X0 + dx * scale, X0 + 2 * dx * scale, X0 + 3 * dx * scale};
                float Y[4] = {Y0, Y0             , Y0                 , Y0                 };

                int niteration[4] = {};
                int        cmp[4] = {1, 1, 1, 1};

                for (; niteration[0] < NITERATIONMAX &&
                       niteration[1] < NITERATIONMAX &&
                       niteration[2] < NITERATIONMAX &&
                       niteration[3] < NITERATIONMAX; niteration[0] += cmp[0],
                                                      niteration[1] += cmp[1],
                                                      niteration[2] += cmp[2],
                                                      niteration[3] += cmp[3])
                {
                    float squared_X[4] = {X[0] * X[0], X[1] * X[1], X[2] * X[2], X[3] * X[3]};
                    float squared_Y[4] = {Y[0] * Y[0], Y[1] * Y[1], Y[2] * Y[2], Y[3] * Y[3]};
                    float       X_Y[4] = {X[0] * Y[0], X[1] * Y[1], X[2] * Y[2], X[3] * Y[3]};;

                    float squared_r[4] = {squared_X[0] + squared_Y[0], squared_X[1] + squared_Y[1], squared_X[2] + squared_Y[2], squared_X[3] + squared_Y[3]};

                    if (squared_r[0]  >= SQUARED_R_MAX) cmp[0] = 0;
                    if (squared_r[1]  >= SQUARED_R_MAX) cmp[1] = 0;
                    if (squared_r[2]  >= SQUARED_R_MAX) cmp[2] = 0;
                    if (squared_r[3]  >= SQUARED_R_MAX) cmp[3] = 0;

                    if (cmp[0] == 0 && cmp[1] == 0 && cmp[2] == 0 && cmp[3] == 0)
                    {
                        break;
                    }

                    X[0] = squared_X[0] - squared_Y[0] + X0 + dx * 0;
                    X[1] = squared_X[1] - squared_Y[1] + X0 + dx * 1 * scale;
                    X[2] = squared_X[2] - squared_Y[2] + X0 + dx * 2 * scale;
                    X[3] = squared_X[3] - squared_Y[3] + X0 + dx * 3 * scale;

                    Y[0] =       X_Y[0] +       X_Y[0] + Y0;
                    Y[1] =       X_Y[1] +       X_Y[1] + Y0;
                    Y[2] =       X_Y[2] +       X_Y[2] + Y0;
                    Y[3] =       X_Y[3] +       X_Y[3] + Y0;
                }

                (*points)[(size_t)(iy * SIZE_SCREEN_X + ix + 0)].position = sf::Vector2f(static_cast<float>(ix + 0), static_cast<float>(iy));
                (*points)[(size_t)(iy * SIZE_SCREEN_X + ix + 1)].position = sf::Vector2f(static_cast<float>(ix + 1), static_cast<float>(iy));
                (*points)[(size_t)(iy * SIZE_SCREEN_X + ix + 2)].position = sf::Vector2f(static_cast<float>(ix + 2), static_cast<float>(iy));
                (*points)[(size_t)(iy * SIZE_SCREEN_X + ix + 3)].position = sf::Vector2f(static_cast<float>(ix + 3), static_cast<float>(iy));


                (*points)[(size_t)(iy * SIZE_SCREEN_X + ix + 0)].color    = sf::Color((sf::Uint8)(256 - niteration[0] * 16), 0, (sf::Uint8)(256 - niteration[0] * 16));
                (*points)[(size_t)(iy * SIZE_SCREEN_X + ix + 1)].color    = sf::Color((sf::Uint8)(256 - niteration[1] * 16), 0, (sf::Uint8)(256 - niteration[1] * 16));
                (*points)[(size_t)(iy * SIZE_SCREEN_X + ix + 2)].color    = sf::Color((sf::Uint8)(256 - niteration[2] * 16), 0, (sf::Uint8)(256 - niteration[2] * 16));
                (*points)[(size_t)(iy * SIZE_SCREEN_X + ix + 3)].color    = sf::Color((sf::Uint8)(256 - niteration[3] * 16), 0, (sf::Uint8)(256 - niteration[3] * 16));

            }
        }
    }
    return;
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

                for (; niteration < NITERATIONMAX; niteration++)
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
