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

    sf::RenderWindow window (sf::VideoMode(1200, 800), "Brilliant graphics, amazing fractal!");

    sf::VertexArray points (sf::Points, 1200 * 800);

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

        for (int iy = 0; iy < 800; iy++)
        {
            //fprintf (stderr, BLU "iy = <%d>" RESET, iy);
            assert (iy < 800);

            float X0 = -2 + offset_x ;                                      //; start from upper left cornel
            float Y0 =  1 + offset_y - (float)iy * dy * scale;

            for (int ix = 0; ix < 1200; ix++, X0 += dx * scale)
            {
                //fprintf (stderr, BLU "ix = <%d>" RESET, ix);
                assert (ix < 1200);

                float X = X0;
                float Y = Y0;

                int niteration = 0;

                for (; niteration < NITERATIONMAX; niteration++)
                {
                    float squared_X = X * X;
                    float squared_Y = Y * Y;
                    float       X_Y = X * Y;

                    float squared_r = squared_X + squared_Y;

                    if (squared_r >= SQUARED_R_MAX)
                    {
                        break;
                    }

                    X = squared_X - squared_Y + X0;

                    Y =       X_Y +       X_Y + Y0;
                }

                (*points)[(size_t)(iy * 1200 + ix)].position = sf::Vector2f(static_cast<float>(ix), static_cast<float>(iy));
                (*points)[(size_t)(iy * 1200 + ix)].color    = sf::Color((sf::Uint8)(256 - niteration * 16), 0, (sf::Uint8)(256 - niteration * 16));
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

        for (int iy = 0; iy < 800; iy++)
        {
            //fprintf (stderr, BLU "iy = <%d>" RESET, iy);
            assert (iy < 800);

            float X0 = -2 + offset_x ;                                      //; start from upper left cornel
            float Y0 =  1 + offset_y - (float)iy * dy * scale;

            for (int ix = 0; ix < 1200; ix++, X0 += dx * scale)
            {
                //fprintf (stderr, BLU "ix = <%d>" RESET, ix);
                assert (ix < 1200);

                float X = X0;
                float Y = Y0;

                int niteration = 0;

                for (; niteration < NITERATIONMAX; niteration++)
                {
                    float squared_X = X * X;
                    float squared_Y = Y * Y;
                    float       X_Y = X * Y;

                    float squared_r = squared_X + squared_Y;

                    if (squared_r >= SQUARED_R_MAX)
                    {
                        break;
                    }

                    X = squared_X - squared_Y + X0;

                    Y =       X_Y +       X_Y + Y0;
                }

                (*points)[(size_t)(iy * 1200 + ix)].position = sf::Vector2f(static_cast<float>(ix), static_cast<float>(iy));
                (*points)[(size_t)(iy * 1200 + ix)].color    = sf::Color((sf::Uint8)(256 - niteration * 16), 0, (sf::Uint8)(256 - niteration * 16));
            }
        }
    }
    return;
}
