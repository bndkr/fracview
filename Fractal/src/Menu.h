#pragma once
#include "FractalView.h"

class Menu
{
public:
	FractalView* view;
	
	const char *names[20] = 
	{ "up", "down", "left", "right", "zoom", "zoomo",
		"load", "save", "exit", "help", "mandelbrot", "julia", "ship",
		"juliaset", "setsize", "iterations", "sweep", "home", "print" };
	Menu(FractalView* view)
	{
		this->view = view;
	}
	Menu() {}
	int processInput(std::string input)
	{
		int choice = -1;
		bool found = false;
		for (int i = 0; i < 20; i++)
		{
			if (input.compare(names[i]))
			{
				choice = i;
				found = true;
			}
		}
		std::string name;
		switch (choice)
		{
		case 0: // up
			view->y_max += (view->y_max - view->centery);
			view->y_min += (view->centery - view->y_min);
			break;
		case 1: // down
			view->y_max -= (view->y_max - view->centery);
			view->y_min -= (view->centery - view->y_min);
			break;
		case 2: // left
			view->x_max -= (view->x_max - view->centerx);
			view->x_min -= (view->centerx - view->x_min);
			break;
		case 3: // right
			view->x_max += (view->x_max - view->centerx);
			view->x_min += (view->centerx - view->x_min);
			break;
		case 4: // zoom
			view->x_max = (view->x_max + view->centerx) / 2;
			view->x_min = (view->x_min + view->centerx) / 2;
			view->y_max = (view->y_max + view->centery) / 2;
			view->y_min = (view->y_min + view->centery) / 2;
			break;
		case 5: // zoomo (zoom out)
			view->x_max += (view->x_max - view->centerx);
			view->x_min -= (view->centerx - view->x_min);
			view->y_max += (view->y_max - view->centery);
			view->y_min -= (view->centery - view->y_min);
			break;
		case 6: // load
			std::cout << std::endl << "Enter the config. name: ";
			std::cin >> name;
			if (!view->getWindowSettings(name))
			{
				std::cout << "Error reading file: invaild config file name." << std::endl;
			}
			std::cout << "Sucessfully loaded config file: " << name << std::endl;
			break;
		case 7: // save
			std::cout << std::endl << "Enter the config. name: ";
			std::cin >> name;
			view->storeWindowSettings(name);
			break;
		case 8: // exit
			return -1;
			break;
		case 9: // help
			break;
		case 10: // mandelbrot
			view->currentFractal = Fractal::Mandelbrot;
			break;
		case 11: // julia
			view->currentFractal = Fractal::Julia;
			break;
		case 12: // ship
			view->currentFractal = Fractal::Ship;
			break;
		case 13: // juliaset
			if (view->currentFractal == Fractal::Mandelbrot)
			{
				view->julia_x = view->centerx;
				view->julia_y = view->centery;
			}
			else
			{
				std::cout << "x: ";
				float inputx, inputy;
				std::cin >> inputx;
				std::cout << " y: ";
				std::cin >> inputy;
				view->julia_x = inputx;
				view->julia_y = inputy;
			}
			break;
		case 14: // setsize
			int width, height;
			std::cout << "width: ";
			std::cin >> width;
			std::cout << std::endl << "height :";
			std::cin >> height;
			view->setImageSize(width, height);
			break;
		case 15: // iterations
			std::cout << "iterations: ";
			int iterations;
			std::cin >> iterations;
			view->setMaxIterations(iterations);
			break;
		case 16: // sweep
			int lower, upper, step, sweepstate;
			std::cout << "sweep iterations from: ";
			std::cin >> lower;
			std::cout << "to: ";
			std::cin >> upper;
			std::cout << "by: ";
			std::cin >> step;
			sweepstate = lower;
			while (true)
			{
				sweepstate += step;
				view->setMaxIterations(sweepstate);
				view->drawFractal(DrawMode::Console);
				if (sweepstate >= upper)
				{
					return;
				}
			}
			break;
		case 17: // home
			view->setWindowCenter(-1.5f, 0.0f, 8, 4);
			break;
		case 18: // print
			view->drawFractal(DrawMode::File);
			std::cout << "Press enter to continue: " << std::endl;
			std::cin.get();
			std::cin.get();
			break;
		}
	}

};