#include <stdio.h>
#include <assert.h>
#include <SFML/Graphics.hpp>

#include "MbFractal.h"
#include "../common/colors.h"

int RunMandelbrotFractal ()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "Brilliant graphics, amazing fractal!");

    sf::VertexArray points(sf::Points, 800 * 800);

    float dx = 1 / 800, dy = 1 / 800;

    for (int iy = 0; iy < 800; iy++)
    {
        //printf ("iy = <%d>", iy);
        assert (iy < 800);

        float X0 = 0;
        float Y0 = 0 + (float)iy * dy;

        for (int ix = 0; ix < 800; ix++, X0 += dx)
        {
            //printf ("ix = <%d>", ix);
            assert (ix < 800);

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
            if (niteration == NITERATIONMAX)
        }

    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear();
        window.draw(points);                            //; draw all points
        window.display();
    }


    return 0;
}
