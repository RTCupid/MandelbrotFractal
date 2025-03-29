#include <stdio.h>
#include <string.h>
#include <SFML/Graphics.hpp>

#include "common/colors.h"
#include "Mandelbrot/MbFractal.h"

int main(int argc, char* argv[])
{
    printf (GRN "# My Fractal\n" RESET);
    printf (GRN "# (c) RTCupid, 2024\n\n" RESET);

    for (int i = 0; i < argc; i++)
    {
        printf ("argv[%d] = <%s>\n", i, argv[i]);
    }

    if (argc > 1)
    {
        RunMandelbrotFractal (atoi (argv[1]));
    }
    else
    {
        printf (RED "Enter mode of program to start\n" RESET);
        return 0;
    }



    return 0;
}
