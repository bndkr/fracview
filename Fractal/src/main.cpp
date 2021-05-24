#include "FractalView.h"

int main()
{
	FractalView f;
	f.setGradientColors(0xffffff, 0xff0000, 0x0000ff);
	f.printPalette();

	f.enterConsoleMode();
}
