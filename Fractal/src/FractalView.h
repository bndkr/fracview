
#include "Complex.h"
#include "ConsoleDrawer.h"
#include <fstream>
#include <iostream>
#include <string>
#include <time.h>
#include <stdlib.h>
#include <math.h>

enum class Fractal
{
	Mandelbrot,
	Julia,
	Ship
};
enum class DrawMode
{
	File,
	Console
};
class FractalView 
{
public:
	FractalView();
	std::ofstream* create_Header(int width, int height, std::string filename);
	void setWindowBounderies(float x_min, float x_max, float y_min,
		float y_max);
	void setWindowCenter(float x_center, float y_center, float x_scale, 
		float y_scale);
	void setImageSize(int width, int height);
	bool drawFractal(DrawMode mode, std::string filename);
	void setMaxIterations(int max_iterations);
	void setInsideColor(int color);
	void setGradientColors(int begin_color, int end_color);
	void setGradientColors(int color1, int color2, int color3);
	bool storeWindowSettings(std::string name);
	bool getWindowSettings(std::string name);
	void enterConsoleMode();
	void printPalette();
private:
	bool handleInput(std::string input, float* centerx, float* centery);
	int default_width;
	int default_height;
	int image_width;
	int image_height;
	int total_pixels;
	int max_iterations;
	int colors = 99;
	char inner_color[3] = {};
	char palette[297] = {}; 
	float colorPixel(int x, int y);
	float x_min;
	float x_max;
	float y_min;
	float y_max;
	float centerx;
	float centery;
	float julia_x = -1.0f;
	float julia_y = -0.1f;
	ConsoleDrawer drawer;
	Fractal currentFractal;
	//Menu menu;
};