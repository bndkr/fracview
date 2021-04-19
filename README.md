# fracview
Command-line interface for viewing fractal images

### Introduction
A simple c++ library useful in the generation of fracal images. The program features a command-line 
'console mode' that allows the user to locate the desired area of complex space to render an image.
Then the user can issue a render command that could render the current area into a .bmp file.

### Basic Usage
The application has almost complete functionality in console mode, with just a few settings
that can only be set by hard-coded into the main.cpp file. Future updates will expand
console mode functionality.

### Compile Instructions
A compiler equivalent to g++ is required to compile/build. The project can also
be loaded into Visual Studio, however the user will have to do this manually.
To compile, open a terminal in the src/ directory, and type:
<br> `make` <br>
This uses the provided makefile to compile the project includeing all dependancies.

The build .exe application should also be included in each commit.

## Console Mode
### Commands
Console mode's interface is a simple command line. The user simply types a command and presses
enter for the command to be executed. Commands can be chained by seperating them by spaces.
Some commands require follow-up parameters.
The following commands are currently supported:
 - up / down / left / right:
 Pans the view in the specified direction. Can be chained: 'up up up'
 - zoom / zoomo:
 Zooms in / out respectively from the center of the screen.
 - help:
 Prints all of the currently supported commands
 - exit / quit:
 Exits the program
 - save:
 Saves the current window location into a .frac file. This encodes the information required
 to re-navigate to that specific location again. Prompts the user for the name of the file.
 - load:
 Loads an existing .frac file, and moves the window to that area. Will prompt the user for 
 the name of the file after the command is ran. Can also be chained: "load valley.frac"
 - mandelbrot:
 Switches the current fractal to the mandelbrot set.
 - julia:
 Switches the current fractal to the julia set.
 - ship:
 Switches the current fractal to the burning ship set.
 - juliaset:
 Modifies the complex seed the julia set uses to generate an image. for example, (x:0, y:0) is a circle.
 If the current fractal is mandelbrot, uses the center of the screen to instead of prompting the user for
 the coordinates.
 - setsize:
 Prompts the user for a width and a height. This is the image dimentions of the output image the program will
 generate. Width will be rounded to the nearest multiple of four.
 - print:
 Using the current window settings, outputs a .bmp file with the fractal. The name of the file depends on the 
 current fractal.
 - iterations:
 Changes the maximum iterations the algorithm uses. Generally, the larger the number, the more detailed the image
 is, but the longer it takes to generate. Generally should't be more than 1024 except for really zoomed-in images.
 - sweep:
 This option is useful to locate interesting structures to render. In the console area, the white pixels are only 
 those who didn't escape before iterating the maximum number of times. This means that structures can be invisible 
 becuase the iteration count is too high. This animates the current image, changing the maximum iteration count
 every frame. Prompts the user for a starting value, and ending value, and a step. The program will start at 
 the specified iteration count and increment (or decrement) by the step value until the value passes the ending
 value. For an example of how it works, type `mandelbrot home sweep 1 30 2` as a command.
 - home:
 Moves the screen to the default viewing area.
 
 More commands are to come.
 
 
 
