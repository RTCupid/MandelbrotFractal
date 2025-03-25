#include <stdio.h>
#include <SFML/Graphics.hpp>

#include "MbFractal.h"

int RunMandelbrotFractal ()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "Brilliant graphics, amazing fractal!");


    sf::VertexArray points(sf::Points, 800 * 800);

    //for (int iy = 0; iy < 800; iy++)
    //{


    //}

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(points);                            // draw all points
        window.display();
    }


    return 0;
}
