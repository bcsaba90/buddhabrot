# Buddhabrot fractal renderer
Program to generate buddhabrot and nebulabrot fractals, also able to generate animation (Image sequence).
Many of the parameters are externally configurable from a fractal definition file. These file can be found under fractal_definitions folder.
Just copy one if these files if you want to create a custom one and play with the values.

Also check the help command line argument of the program for some more command line argument options.

You can compile the program in any C++11 compatible compiler, here is an example in g++ on Linux:

	g++ *.cpp -Wall -Ofast -ffast-math -std=c++11 -march=native

march and ffast-math might be platform-dependent, if you plan to use the program in different computer than you compiled it, you should leave those out.

### Here is a sample render, see the rest in the sample_renders folder:
![Sample render](https://github.com/helospark/buddhabrot/blob/master/sample_renders/nebulabrot.png?raw=true)

This program is also able to create animation by changing the power (so generating the bbrot distribution of any any n-brot set). Under Linux (or bash running OS) you can convert the generated bmp images to a gif or an mp4 film using the useful_scripts/makevideo.sh. See help of makevideo.sh to see options.

Multithreading is implemented by using C++11 threads.