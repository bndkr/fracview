# Documentation

## Introduction

### FractalView
The FractalView class provides most of the functionality of the program. It has the tools
to generate all images, resize windows, and enter the console mode.
### Complex
Complex.h is contains the Complex class intended to deal with complex numbers. It is used
for the fractal generation iterative functions.
### Menu
Currently unused, the plan is to move the console mode to this class eventually.
### ConsoleDrawer
Prints the colored 'pixels' to the console.
### main.cpp
The entry point of the program.

## FractalView Methods

###  FractalView() (constructor)
Initializes internal variables. Current defaults are:
inner color (the color of the points that don't escape): black
image dimensions: 512 x 512
maximum iterations: 100
color palette: from black to white and back
window: from -2 to 2 in both real and imaginary axes.

### std::ofstream* create_Header(int width, int height)
Vreates the .bmp image header before any image data is written.
returns a pointer to the ofstream object to further write to the file.

### void setWindowBounderies(float x_min, float x_max, float y_min, float y_max)
Sets the current window. x is the real axis and y is the imaginary.

### void setWindowCenter(float x_center, float y_center, float x_scale, float y_scale)
Sets the current window by the center point and each axis scale.

### void setImageSize(int width, int height)
Sets the dimensions of the image to be printed. The width will be rounded to the next highest multiple of four.

### bool drawFractal(DrawMode mode);
Prints the current window of the current fractal to a .bmp file or console window, returns true on success.
Drawmode is an enum with the following possible values:
`DrawMode::Console` - prints the fractal to the console window
`DrawMode::File` - prints the fractal to a .bmp file

### void setMaxIterations(int max_iterations)
Sets the maximum times the recursive function will run while the value stays bounded.

### void setInsideColor(int color)
Sets the color of the pixels inside the fractal sets (ie. the mandelbrot set). Default is black.
`color` is a 24-bit color in RGB with the form 0xRRGGBB

### void setGradientColors(int begin_color, int end_color)
Creates a gradient bewtween the two specified colors and applies it to color pixels that escape.
`begin_color` and `end_color` are 24-bit colors in RGB with the form 0xRRGGBB

### void setGradientColors(int color1, int color2, int color3)
Work in progress. Intended to interpolate between three colors instead of two. see setGradientColors() for more info.

### bool storeWindowSettings(std::string name)
Creates a .frac config file with the name `name`. returns true on success.

### bool getWindowSettings(std::string name)
Loads an existing .frac file of the name `name`. returns true if the file is found and successfully loaded.

### void enterConsoleMode()
Uses the console to display fractal images. Upon calling this method, the program will remain in console mode
until the uses types the commands `exit` or `quit`. Console mode is a 'viewfinder' that quickly locates an 
area with features the user wants to fully render. see [README.md](../../README.md) for a comprehensive command 
list.

