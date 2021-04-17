#include <iostream>
#include <string>
class ConsoleDrawer
{
public:
	void beginDraw();

	void endDraw();
	void colorPixel(int x, int y, int color);
};