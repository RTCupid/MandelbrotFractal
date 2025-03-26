#include <stdio.h>
#include <assert.h>
#include <SFML/Graphics.hpp>

#include "MbFractal.h"
#include "../common/colors.h"

int RunMandelbrotFractal ()
{
    sf::VertexArray points(sf::Points, 800 * 800);

    sf::RenderWindow window(sf::VideoMode(800, 800), "Brilliant graphics, amazing fractal!");

    sf::Clock fpsClock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    int frameCount = 0;
    float averagedFps = 0;

    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/opentype/urw-base35/NimbusRoman-Bold.otf"))
    {
    }

    sf::Text fpsText;
    fpsText.setFont(font);
    fpsText.setCharacterSize(20);
    fpsText.setFillColor(sf::Color::Black);
    fpsText.setPosition(10, 10);

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

        float dx = (float)1 / 200, dy = (float)1 / 200;             //; to check all points from (-2,-2) to (2, 2)
                                                                    //; with step 1/200
        //printf ("dx = %f, dy = %f\n", dx, dy);

        for (int iy = 0; iy < 800; iy++)
        {
            //fprintf (stderr, BLU "iy = <%d>" RESET, iy);
            assert (iy < 800);

            float X0 = -2;                                          //; start from lower right cornel
            float Y0 =  2 - (float)iy * dy;

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

                    //printf (BLU "iy = %d, ix = %d, for niteration = %d, squared_r = %f, "
                    //           "Y0 = %f, X0 = %f, Y = %f, X = %f\n"
                    //            "-----------------------------------------------------\n", iy, ix, niteration, squared_r, Y0, X0, Y, X);

                    if (squared_r >= SQUARED_R_MAX)
                    {
                        break;
                    }

                    X = squared_X - squared_Y + X0;

                    Y =       X_Y +       X_Y + Y0;
                }
                if (niteration == NITERATIONMAX)
                {
                    points[(size_t)(iy * 800 + ix)].position = sf::Vector2f(static_cast<float>(ix), static_cast<float>(iy));
                    points[(size_t)(iy * 800 + ix)].color    = sf::Color::Magenta;
                }
                else
                {
                    points[(size_t)(iy * 800 + ix)].position = sf::Vector2f(static_cast<float>(ix), static_cast<float>(iy));
                    points[(size_t)(iy * 800 + ix)].color    = sf::Color::White;
                }
            }
        }

        timeSinceLastUpdate += fpsClock.restart();
        frameCount++;

        if (timeSinceLastUpdate >= sf::seconds(1.f))
        {
            averagedFps = (float)frameCount / timeSinceLastUpdate.asSeconds();
            timeSinceLastUpdate = sf::Time::Zero;
            frameCount = 0;
        }
        fpsText.setString("FPS: " + std::to_string((int)averagedFps));

        window.clear();

        window.draw(points);                                        //; draw all points
        window.draw(fpsText);

        window.display();
    }

    return 0;
}
