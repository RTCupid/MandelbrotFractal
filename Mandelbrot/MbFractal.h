#ifndef MANDELBROTFRACTAL_H
#define MANDELBROTFRACTAL_H

const int NITERATIONMAX = 256;

const float SQUARED_R_MAX = 10 * 10;

int  RunMandelbrotFractal (char* mode, int ntimes = 1);

void CalculateMandelbrot  (sf::VertexArray* points, float offset_x, float offset_y, float scale);

#endif
