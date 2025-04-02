# Laboratory work 3.3.3.<br>Optimization of calculations for points in Mandelbrot fractal with help of intrinsics. Making beautiful graphics with help of SFML

## Contents
- [0. Annotation](#annotation)
- [1. Introduction](#introduction)
- [2. Methods](#methods)
- [3. Results](#results)
- [4. Conclusion](#conclusion)
- [5. Appendix](#appendix)

## Annotation

I made two version for calculations points in Mandelbrot fractal. First version included common cycle of calculations, second version used intrinsics to optimize program. My version with intrinsics showed a 1.19x increase in FPS. However increase of fps after using intrinsics less than expected.

## Introduction

  In graphics and games, it is often necessary to calculate position and color for a large number of points using different algorithms. Optimizing this calculations is badly optimized by the compiler if they are implemented by looping calculations for each point separately. This is due to the fact that the compiler doesn't know that the calculations don't depend on each other. So, the programmer has a task to provide this information to the compiler or to realize the calculations using a more effective method. For example, a good way is to combine several numbers in a special register and use special functions built into the compiler to convert all of them. These functions are called intrinsics.

## Methods

  To compare common methods with a method that uses intrinsics, you can perform a few calculations and find the time it takes. To find this time you can use SFML class sf::Time. Also you should consider that time can vary with the state of the system and measure it under the same conditions. 

## Results

### Model

  I used the Mandelbrot fractal to test the effectiveness of the intrinsics method because it contains a large number of calculations and allows you to verify the correctness of the calculations using graphics, and because it is easy to write.

  <img src="/img/Fractal.png">
  <div align="center"> Fig. 1. My graphics of Mandelbrot fractal. Black points is points that belong Mandelbrot fractal, purple points don't belong Mandelbrot fractal and have any shade for beauty visual effect.</div><br>

  Also model have command line argument "ntests" which shows how many times to calculate all points in one screen update. This is so that you can find the time with less error if you have more calculations in one test. By default ntests = 1.
  
  Some amazing places of my Mandelbrot fractal you can see in Appendix C.

### Measurements

  I made two variants of program for calculate Mandelbrot fractal. First method included common calculations for each point. But with optimization key -O2 compiler optimized it using xmm registers to vectorization some calculations. To compare I made second method where I independently realized vectorization and used intrinsics for calculations. Progress of making program you can see in Appendix A.
  Time measured using the Time class in the sfml library. Each value of time included 120 complete calculations. This is made to make less random error from the experiment. Was used optimization key -O2. The measurements were made at the same time with the same system condition. The obtained values are given in Appendix B. Visual picture of this values you can see in the Figure 2.

  <img src="/img/Measurings.png">
  <div align="center"> Fig. 2. graph of calculation time dependence for the common variant and for the variant using intrinsics. It is made to visually show the difference in the effectiveness of the two methods. Green points are experimental values which were obtained in the experiment with the common cycle calculation. Blue points for the cycle calculation with intrinsics. Time was measured using the Time class in the sfml library. Each point shows the time of 120 complete calculations. This is made to make less random error from the experiment. The measurements were made at the same time with the same system condition. Was used optimization key -O2.</div><br>

  You can compare fps in Figure 3 with fps in Figure 4. Fps when using intrinsics is higher than in common code.

  <img src="/img/FPSINTRINSICS.png">
  <div align="center"> Fig. 3. Mandelbrot fractal with using intrinsics. The fps is higher than in common version. The compilation was performed with optimization key -O2. FPS calculated in relize version of program with ntests = 1.</div><br>

   <img src="/img/FPSCOMMON.png">
  <div align="center"> Fig. 4. Common Mandelbrot fractal. Measurements were made at the same time and with the same system state. The compilation was performed with optimization key -O2. FPS calculated in relize version of program with ntests = 1.</div><br>

  I found fps using average time. Average time for the common version is 29.170368 ± 10<sup>-6</sup>, for the version with intrinsics is 24.623226 ± 10<sup>-6</sup>. Then fps in the intrinsic version is 0.0406, in the common version is 0.0342. Total, the increase in fps is 19%.

  However fps in version with intrinsics less than expected. I checked it in debugger radare 2 and saw that processor read some values from memory. This is probably due to the fact that there is an excessive number of variables in the program code for calculations.

## Conclusion

The results show that the use of intrinsics allows to optimize programs, especially it is effective when the same calculations are performed for a lot of data. My version with intrinsics showed a 1.19x increase in FPS. However increase of fps after using intrinsics less than expected. This is probably due to the fact that with optimization key -O2 compiler optimized common version using xmm registers to vectorization some calculations. Also it could be due to a flaw in my intrinsics function, for example bigger number of variables, which caused the processor to read some data from memory during calculations.

## Appendix

### Appendix A. Progress of work

  At first, I make a 4-point loop instead of 1-point loop, to say to compiler that status of this points independent of each other. Then I changed operations with any points to loops and made inline functions to work with array of four float numbers from it. After it I changed my functions for actions with array of four points to intrinsics, that work with type of numbers __m128. It have 128 bit and can include four numbers of type float. The next block of code shows what the main loop of the Mandelbrot calculation looks like using intrinsics.

```c++
for (;; niteration = _mm_add_ps (niteration, cmp))
{
    cmp = _mm_cmple_ps (niterationmax, niteration);

    int    mask      = _mm_movemask_ps (cmp);

    if (mask) break;

    __m128 squared_X = _mm_mul_ps (X, X);
    __m128 squared_Y = _mm_mul_ps (Y, Y);
    __m128       X_Y = _mm_mul_ps (X, Y);

    __m128 squared_r = _mm_add_ps (squared_X, squared_Y);

    cmp  = _mm_cmple_ps (squared_r, squared_r_max);

    mask = _mm_movemask_ps (cmp);

    if (!mask) break;

    X = _mm_sub_ps (squared_X, squared_Y);
    X = _mm_add_ps (X        , array_X0);
    X = _mm_add_ps (X        , array_dx_scale_index);

    Y = _mm_add_ps (X_Y, X_Y);
    Y = _mm_add_ps (Y, array_Y0);

    cmp = _mm_and_ps(cmp, _mm_set_ps1(1.0f));
}
```

### Appendix B. Experimental values

  | N      | Common time, s ± 10<sup>-6</sup> | Intrinsics time, s ± 10<sup>-6</sup>  | N      |  Common time, s ± 10<sup>-6</sup> | Intrinsics time, s ± 10<sup>-6</sup>|
  |:------:|:--------------:|:-------------------:|:------:|:----------------:|:------------------:|
  | 1      | 29.172709      | 24.573540           | 11     | 29.151709        | 25.225914          |
  | 2      | 29.149599      | 23.816408           | 12     | 29.261366        | 24.619513          |
  | 3      | 29.113285      | 23.892267           | 13     | 29.269333        | 24.875324          |
  | 4      | 29.067999      | 24.606806           | 14     | 29.143482        | 24.437838          |
  | 5      | 29.329220      | 24.567129           | 15     | 29.156736        | 24.767895          |
  | 6      | 29.291000      | 24.913528           | 16     | 29.186344        | 24.719604          |
  | 7      | 29.097939      | 24.651991           | 17     | 29.096285        | 24.585064          |
  | 8      | 29.290800      | 24.828905           | 18     | 29.154882        | 25.225779          |
  | 9      | 29.157974      | 24.869411           | 19     | 29.247868        | 25.853722          |
  | 10     | 29.026993      | 24.694475           | 20     | 29.231627        | 26.203354          |
  
  <div align="center"> Tab. 1. Experimental values of time dependence for the common variant (columns "common") and for the variant using intrinsics (columns "intrinsics"). Time was measured using the Time class in the sfml library. Each point shows the time of 120 complete calculations. This is made to make less random error from the experiment. The measurements were made at the same time with the same system condition. Columns "N" - number of measurment. For each methods made 20 measurments</div><br>

SFML class time has instrumental error about 1 ms. 

### Appendix C. Some amazing places

  <img src="/img/BEAUTYPICTUREFIRST.png">
  <div align="center"> Fig. 5</div><br>

  <img src="/img/BEAUTYPICTURETHIRD.png">
  <div align="center"> Fig. 6</div><br>

  <img src="/img/BEAUTYPICTURESECOND.png">
  <div align="center"> Fig. 7</div><br>

  <img src="/img/BEAUTYPICTURE4TH.png">
  <div align="center"> Fig. 8</div><br>
