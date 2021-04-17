#include "FractalView.h"
#include <chrono>
#include <time.h>

int main()
{


	// TODO: make color config files also, to specify a color scheme
	FractalView f;
	f.setGradientColors(0x000000, 0xff0000);
	f.setMaxIterations(500);
	f.enterConsoleMode();
	double b = time(NULL);
	std::cout << b << std::endl;
	
}
