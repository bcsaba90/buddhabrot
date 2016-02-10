# Buddhabrot fractal renderer
Program to generate buddhabrot and nebulabrot fractals, also able to generate animation (Image sequence).
Many of the parameters are externally configurable from a fractal definition file. These file can be found under fractal_definitions folder.
Just copy one if these files if you want to create a custom one and play with the values.

Also check the help command line argument of the program for some more command line argument options.

You can compile the program in any C++11 compatible compiler, here is an example in g++ on Linux:

	g++ *.cpp -Wall -Ofast -ffast-math -std=c++11 -static -march=native

march and ffast-math might be platform-dependent, if you plan to use the program in different computer than you compiled it, you should leave those out.

### Here is a sample render, see the rest in the sample_renders folder:
![Sample render](https://github.com/helospark/buddhabrot/blob/master/sample_renders/nebulabrot.png?raw=true)

If you have created an image sequency you can use ffmpeg to create a video or gif out of it.
