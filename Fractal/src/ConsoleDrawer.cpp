#include "ConsoleDrawer.h"

void ConsoleDrawer::beginDraw()
{
	std::cout << "\x1b[30";
}

void ConsoleDrawer::endDraw()
{
	std::cout << "\x1b[0m";
}

void ConsoleDrawer::colorPixel(int x, int y, int color)
{
	std::string message = "\x1b[" + std::to_string(y) + ";" +
		std::to_string(x) + "f";
	std::cout << message;
	switch (color)
	{
	case 0:
		std::cout << "\x1b[47m ";
		break;
	case 1:
		std::cout << "\x1b[41m ";
		break;
	}
	
}
