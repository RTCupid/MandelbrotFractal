# Laboratory work 3.3.3.<br>Optimization of calculations for points in Mandelbrot fractal with help of intrinsics. Making beautiful graphics with help of SFML

## Contents
- [0. Annotation](#annotation)
- [1. Introduction](#introduction)
- [2. Methods](#methods)
- [3. Results](#results)
- [4. Conclusion](#conclusion)
- [5. Appendix](#appendix)

## Annotation

## Introduction

  In graphics and games, it is often necessary to calculate position and color for a large number of points using different algorithms. Optimizing this calculations is badly optimized by the compiler if they are implemented by looping calculations for each point separately. This is due to the fact that the compiler doesn't know that the calculations don't depend on each other. So, the programmer has a task to provide this information to the compiler or to realize the calculations using a more effective method. For example, a good way is to combine several numbers in a special register and use special functions built into the compiler to convert all of them. These functions are called intrinsics.

## Methods

  To compare common methods with a method that uses intrinsics, you can perform a few calculations and find the time it takes. To find this time you can use SFML class sf::Time. Also you should consider that time can vary with the state of the system and measure it under the same conditions. For check this factor you can make basic version, find time it takes, and compare it before each measurement. 

## Results

### Model

  I used the Mandelbrot fractal to test the effectiveness of the intrinsics method because it contains a large number of calculations and allows you to verify the correctness of the calculations using graphics, and because it is easy to write.

  <img src="/img/Fractal.png">
  <div align="center"> Fig. 1. My graphics of Mandelbrot fractal. Black points is points that belong Mandelbrot fractal, purple points don't belong Mandelbrot fractal and have any shade for beauty visual effect.</div><br>

  Also model have command line argument "ntests" which shows how many times to calculate all points in one screen update. This is so that you can find the time with less error if you have more calculations in one test. By default ntests = 1.

### Measurements

  

  

## Conclusion

## Appendix

### Progress of work

  At first, I make a 4-point loop instead of 1-point loop, to say to compiler that status of this points independent of each other.

  <img src="/img/LoopForFourPoints.png">
  <div align="center"> Fig. 2. Code of Mandelbrot fractal with using loop for four points.</div><br>

  <img src="/img/LoopForFourPointsFps.png">
  <div align="center"> Fig. 3. Mandelbrot fractal with using loop for four points. You can see that the fps is higher than in common version. The compilation was performed without using optimization keys</div><br>

  You can compare fps in Figure 3 with fps in Figure 4. Fps when using a 4-point loop is higher than in common code.

   <img src="/img/CommonFpsForCompareWithLoopFourPoints.png">
  <div align="center"> Fig. 4. Common Mandelbrot fractal. Measurements were made at the same time and with the same system state. The compilation was performed without using optimization keys</div><br>
