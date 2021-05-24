#include "FractalView.h"

FractalView::FractalView()
{
	// initialize member variables.
	default_width = 512;
	default_height = 512;
	image_height = default_height;
	image_width = default_width;
	total_pixels = image_height * image_height;
	max_iterations = 100;

	// set window variables with defaults
	x_min = -2.0f;
	x_max = 2.0f;
	y_min = -2.0f;
	y_max = 2.0f;

	// what color do we want to color the inside of the sets?
	inner_color[0] = 0x00;
	inner_color[1] = 0x00;
	inner_color[2] = 0x00;

	std::cout << std::endl;

	// initialize the color palette (default is black -> white)
	for (int i = 0; i < colors; i++)
	{
		this->palette[i * 3] = i * 3 * (256.0f / colors);
		this->palette[i * 3 + 1] = i * 3 * (256.0f / colors);
		this->palette[i * 3 + 2] = i * 3 * (256.0f / colors);
	}
	return;
}

void FractalView::setWindowBounderies(float x_min, float x_max, float y_min,
	float y_max)
{
	this->x_min = x_min;
	this->x_max = x_max;
	this->y_min = y_min;
	this->y_max = y_max;
}

void FractalView::setWindowCenter(float x_center, float y_center, 
	float x_scale, float y_scale)
{
	this->x_min = x_center - (x_scale / 2.0);
	this->x_max = x_center + (x_scale / 2.0);
	this->y_min = y_center - (y_scale / 2.0);
	this->y_max = y_center + (y_scale / 2.0);
	this->centerx = x_center;
	this->centery = y_center;

}

void FractalView::setImageSize(int width, int height)
{
	if (width < 1 || height < 1)
	{
		std::cout << "Error: image must be size 1x1 or greater"
			<< std::endl;
	}
	while (width % 4 != 0) width++;
	this->default_height= height;
	this->default_width = width;
	this->total_pixels = height * width;
}

std::ofstream* FractalView::create_Header(int width, int height, std::string filename)
{
	// create the file object stream and set up the buffer.
	std::ofstream* file;
	file = new std::ofstream(filename + ".bmp", std::ios::out | std::ios::binary);
	int total_pixels = width * height;
	/*-------BMP HEADER--------*/
	char id_field[2] = { 66, 77 }; // ascii: "BM" for bitmap
	char file_size[4] =
	{// file size in bytes. pretty obvious, the 56 is the byte size of the header
			 (total_pixels * 3 + 54) & 0x000000ff,
			((total_pixels * 3 + 54) & 0x0000ff00) >> 8,
			((total_pixels * 3 + 54) & 0x00ff0000) >> 16,
			((total_pixels * 3 + 54) & 0xff000000) >> 24
	};
	char app_1[2] = { 0, 0 }; //Application specific
	char app_2[2] = { 0, 0 }; //Application specific
	char offset[4] = { 54, 0, 0, 0 }; // Offset of raw data

	/*-------DIB HEADER--------*/
	char dib_size[4] = { 40, 0, 0, 0 }; //size of the dib header in bytes
	char w[4] =
	{	// width of image in pixels
		 width & 0x000000ff,
		(width & 0x0000ff00) >> 8,
		(width & 0x00ff0000) >> 16,
		(width & 0xff000000) >> 24
	};
	char h[4] =
	{	// height of image in pixels
		 height & 0x000000ff,
		(height & 0x0000ff00) >> 8,
		(height & 0x00ff0000) >> 16,
		(height & 0xff000000) >> 24
	};
	char color_planes[2] = { 1, 0 }; // number of color planes
	char bits_per_pixel[2] = { 24, 0 }; // bits per pixel
	char compression[4]{ 0, 0, 0, 0 }; // no compression.
	char raw_size[4] =
	{ // this is the number of bytes of the raw image data.
		 (total_pixels * 3) & 0x000000ff,
		((total_pixels * 3) & 0x0000ff00) >> 8,
		((total_pixels * 3) & 0x00ff0000) >> 16,
		((total_pixels * 3) & 0xff000000) >> 24
	};
	char print_resolution_1[4] =
	{ // this is really arbitrary, we don't plan on printing the bitmap.
		 2835 & 0x000000ff,
		(2835 & 0x0000ff00) >> 8,
		(2835 & 0x00ff0000) >> 16,
		(2835 & 0xff000000) >> 24
	};
	char print_resolution_2[4] =
	{ // this is really arbitrary, we don't plan on printing the bitmap.
		 2835 & 0x000000ff,
		(2835 & 0x0000ff00) >> 8,
		(2835 & 0x00ff0000) >> 16,
		(2835 & 0xff000000) >> 24
	};
	char palette_colors[4] = { 0, 0, 0, 0 }; //no palette
	// all colors are important.
	char important_colors[4] = { 0, 0, 0, 0 };

	// now, we write the headers to the file.
	file->write(id_field, 2);
	file->write(file_size, 4);
	file->write(app_1, 2);
	file->write(app_2, 2);
	file->write(offset, 4);
	file->write(dib_size, 4);
	file->write(w, 4);
	file->write(h, 4);
	file->write(color_planes, 2);
	file->write(bits_per_pixel, 2);
	file->write(compression, 4);
	file->write(raw_size, 4);
	file->write(print_resolution_1, 4);
	file->write(print_resolution_2, 4);
	file->write(palette_colors, 4);
	file->write(important_colors, 4);
	return file;
}

bool FractalView::drawFractal(DrawMode mode = DrawMode::File, std::string filename = "unknown")
{
	if (mode == DrawMode::Console)
	{
		int c_sizey = 30;
		int c_sizex = 120;
		this->image_height = 30;
		this->image_width = 120;
		drawer.beginDraw();
		for (int i = 0; i < c_sizey; i++) // i == y
		{
			for (int j = 0; j < c_sizex; j++) // j == x
			{
				if (colorPixel(j, i) == -1.0f)
				{
					drawer.colorPixel(j, c_sizey - i, 0);
				}
				else
				{
					drawer.colorPixel(j, c_sizey - i, 1);
				}
				
			}
		}

		drawer.colorPixel(0, 0, 2);
		drawer.endDraw();
		image_width = default_width;
		image_height = default_height;
		return true;
	}
	
	// print out some useful information for the user
	std::cout << std::endl << "Image size: " << image_width <<
		" x " << image_height << std::endl;
	std::cout << "Total pixels: " << total_pixels << std::endl;
	std::cout << "Iterations: " << max_iterations << std::endl;
	std::cout << std::endl;
	
	// grab the file pointer so we can continue printing to the binary file
	std::ofstream* file = create_Header(image_width, image_height, filename);

	// now we begin writing the pixel data of the image to the file.
	if (currentFractal == Fractal::Mandelbrot)
	{
		std::cout << "Generating mandelbrot set..." << std::endl;
	}
	if (currentFractal == Fractal::Julia)
	{
		std::cout << "Generating julia set..." << std::endl;
	}
	if (currentFractal == Fractal::Ship)
	{
		std::cout << "Generating burning ship set..." << std::endl;
	}
	//grab the time so we know how long it takes to generate
	//int t = std::time(NULL);

	// now the progress bar. there will be 40 steps.
	int steps = 40;
	int divider = image_height / steps; // roughly the number of lines per step
	int plusses = 0;
	int minuses = steps; // we begin with no progress done
	
	for (int y = 0; y < image_height; y++)
	{
		// once we hit a multiple of the divider,
		if (y % divider == 0)
		{
			// clear the line with ANSI escape char
			std::cout << "\xD";
			// print the progress bar
			std::cout << "Progress:  ";

			plusses++;
			minuses = steps - plusses;
			std::cout << "[";
			// for every plus, print a hash
			for (int i = 0; i < plusses; i++)
			{
				std::cout << "\x1b[32m#";
			}
			// for every minus, print a minus.
			for (int j = 0; j < minuses; j++)
			{
				std::cout << "\x1b[31m-";
			}
			// finish it up.
			std::cout << "\x1b[0m]";
		}
		
		for (int x = 0; x < image_width; x++)
		{
			int color = -1;
			float gradient = colorPixel(x, y); // the color number
			int step = (int)gradient; // step is the last color step in the palette to add gradient too
			gradient = gradient - step; // how 'inbetween' the color is.
			step = step % colors;

			int blue = palette[step * 3];
			int green = palette[step * 3 + 1];
			int red = palette[step * 3 + 2];

			// we have to normalize the rgb values to ranges from 0 - 255 to work properly.
			if (blue < 0) blue += 256;
			if (green < 0) green += 256;
			if (red < 0) red += 256;

			int bluediff = palette[((step + 1) * 3) % (colors * 3)];
			int greendiff = palette[((step + 1) * 3 + 1) % (colors * 3)];
			int reddiff = palette[((step + 1) * 3 + 2) % (colors * 3)];

			if (bluediff < 0) bluediff += 256;
			if (greendiff < 0) greendiff += 256;
			if (reddiff < 0) reddiff += 256;

			// with all relevant colors normalized, we can now add the gradient
			blue = blue + (bluediff - blue) * gradient;
			green = green + (greendiff - green) * gradient;
			red = red + (reddiff - red) * gradient;

			// before we print the color to the file, we need to revert the normalization we did before.
			if (blue > 127) blue -= 256;
			if (green > 127) green -= 256;
			if (red > 127) red -= 256;

			if (bluediff > 127) bluediff -= 256;
			if (greendiff > 127) greendiff -= 256;
			if (reddiff > 127) reddiff -= 256;


			// if the point is inside the set, color it appropriatly.
			if (gradient == -1.0f)
			{
				file->write(inner_color, 3);
			}
			else // otherwise, color it acording to the palette.
			{
				char realblue = char(blue);
				char realgreen = char(green);
				char realred = char(red);
				file->write(&realblue, 1);
				file->write(&realgreen, 1);
				file->write(&realred, 1);
			}
		}
	}
	std::cout << std::endl << std::endl;

	//int duration = std::time(NULL) - t;
	//std::cout << "Done!" << std::endl;
	//std::cout << "Fractal generated in about " << duration / 60 <<
	//	" minute(s) and " << duration % 60 <<
	//	" second(s)" << std::endl << std::endl;

	// Good etiquette: Close the file when you're done!
	file->close();
	return true;
}

void FractalView::setMaxIterations(int max_iterations)
{
	if (max_iterations < 1)
	{
		std::cout << "Error: max iterations must be more than one." <<
			std::endl;
		return;
	}
	this->max_iterations = max_iterations;
}

void FractalView::setInsideColor(int color)
{
	if (color > 0xffffff || color < 0x0)
	{
		std::cout << "Error: invalid color. Must be between 0x0 and 0xffffff"
			<< " in the form 0xRRGGBB" << std::endl;
		return;
	}
	inner_color[2] =  color & 0x0000ff;
	inner_color[1] = (color & 0x00ff00) >> 8;
	inner_color[0] = (color & 0xff0000) >> 16;
}

void FractalView::setGradientColors(int begin_color, int end_color)
{
	if ((begin_color > 0xffffff || begin_color < 0x0) ||
		(end_color > 0xffffff || end_color < 0x0))
	{
		std::cout << "Error: invalid color. Must be between 0x0 and 0xffffff"
			<< " in the form 0xRRGGBB" << std::endl;
		return;
	}

	// extract the rgb values from both colors
	int blue_begin = begin_color & 0x0000ff;
	int blue_end = end_color & 0x0000ff;
	int green_begin = (begin_color & 0x00ff00) >> 8;
	int green_end = (end_color & 0x00ff00) >> 8;
	int red_begin = (begin_color & 0xff0000) >> 16;
	int red_end = (end_color & 0xff0000) >> 16;

	// using linear interpolation, create a gradient from the first
	// color, to the end color, and back to the first color.
	float half = colors / 2;
	for (int i = 0; i < colors / 2; i++)
	{
		// blue channel
		palette[i * 3] = (((half - i) / half) * blue_begin) +
			((i / half) * blue_end);
		palette[(colors - 1) - (i * 3 + 2)] = (((half - i) / half) * blue_begin) +
			((i / half) * blue_end);

		// green channel
		palette[i * 3 + 1] = (((half - i) / half) * green_begin) +
			((i / half) * green_end);
		palette[(colors - 1) - (i * 3 + 1)] = (((half - i) / half) * green_begin) +
			((i / half) * green_end);

		// red channel
		palette[i * 3 + 2] = (((half - i) / half) * red_begin) +
			((i / half) * red_end);
		palette[(colors - 1) - (i * 3)] = (((half - i) / half) * red_begin) +
			((i / half) * red_end);
	}
}

void FractalView::setGradientColors(int color1, int color2, int color3)
{
	if ((color1 > 0xffffff || color1 < 0x0) ||
		(color2 > 0xffffff || color2 < 0x0)	||
		(color3 > 0xffffff || color3 < 0x0))
	{
		std::cout << "Error: invalid color. Must be between 0x0 and 0xffffff"
			<< " in the form 0xRRGGBB" << std::endl;
		return;
	}

	float third = colors / 3; // 33? there are 100 total colors, with 300 data values

	// blue channel
	int blue1 = color1 & 0x0000ff;
	int blue2 = color2 & 0x0000ff;
	int blue3 = color3 & 0x0000ff;

	// green channel
	int green1 = (color1 & 0x00ff00) >> 8;
	int green2 = (color2 & 0x00ff00) >> 8;
	int green3 = (color3 & 0x00ff00) >> 8;

	// red channel
	int red1 = (color1 & 0xff0000) >> 16;
	int red2 = (color2 & 0xff0000) >> 16;
	int red3 = (color3 & 0xff0000) >> 16;
	int i = 0;
	// interpolate between the first and second colors
	for (i; i < third; i++) // third is 33 for now
	{
		palette[i * 3] = blue1 * ((third - i) / third) + blue2 * (i / third);
		palette[i * 3 + 1] = green1 * ((third - i) / third) + green2 * (i / third);
		palette[i * 3 + 2] = red1 * ((third - i) / third) + red2 * (i / third);

		palette[i * 3 + colors] = blue2 * ((third - i) / third) + blue3 * (i / third);
		palette[i * 3 + 1 + colors] = green2 * ((third - i) / third) + green3 * (i / third);
		palette[i * 3 + 2 + colors] = red2 * ((third - i) / third) + red3 * (i / third);

		palette[i * 3 + colors * 2] = blue3 * ((third - i) / third) + blue1 * (i / third);
		palette[i * 3 + 1 + colors * 2] = green3 * ((third - i) / third) + green1 * (i / third);
		palette[i * 3 + 2 + colors * 2] = red3 * ((third - i) / third) + red1 * (i / third);
	}

}

bool FractalView::storeWindowSettings(std::string name)
{
	name = name + ".frac";
	std::ofstream file;
	file.open(name, std::ios::out);
	if (currentFractal == Fractal::Mandelbrot)
	{
		file << "mandelbrot" << std::endl;
	}
	else if (currentFractal == Fractal::Julia)
	{
		file << "julia" << std::endl;
		file << julia_x;
		file << julia_y;
	}
	else if (currentFractal == Fractal::Ship)
	{
		file << "ship" << std::endl;
	}
	file << x_max << std::endl;
	file << x_min << std::endl;
	file << y_max << std::endl;
	file << y_min << std::endl;
	file.close();
	return true;
}

bool FractalView::getWindowSettings(std::string name)
{
	name = name + ".frac";
	std::ifstream file;
	file.open(name, std::ios::in);
	if (!file)
	{
		std::cout << "Error reading file: invaild config file name." << std::endl;
		return false;
	}
	std::string fractalName;
	file >> fractalName;
	if (fractalName == "mandelbrot")
	{
		currentFractal = Fractal::Mandelbrot;
	}
	else if (fractalName == "julia")
	{
		currentFractal = Fractal::Julia;
		file >> julia_x;
		file >> julia_y;
	}
	else if (fractalName == "ship")
	{
		currentFractal = Fractal::Ship;
	}
	file >> x_max;
	file >> x_min;
	file >> y_max;
	file >> y_min;
	file.close();
	return true;
}
// TODO: fix the pointers going into handleInput()
void FractalView::enterConsoleMode()
{
	std::string input;
	default_width *= 2;
	float zoom = 4;
	float x = -1.5f, y = 0.0f;
	setWindowCenter(x, -y, 2 * zoom, zoom);
	currentFractal = Fractal::Mandelbrot;
	while (true)
	{
		centerx = (x_max + x_min) / 2;
		centery = (y_max + y_min) / 2;

		drawFractal(DrawMode::Console, "");
		std::cout << "x: " << centerx << "  y: " << centery << "  zoom: " << x_max - centerx << "  Command: ";
		std::cin >> input;
		if (!this->handleInput(input, &centerx, &centery))
		{
			break;
		}
	}
}

void FractalView::printPalette()
{
	for (int i = 0; i < colors; i++)
	{
		std::cout << std::dec << " COLOR " << i << std::hex << "  ";
		std::cout << "r: " <<  ((int) palette[i * 3 + 2]  & 0xff) << "   ";
		std::cout << "g: " <<  ((int) palette[i * 3 + 1]  & 0xff) << "   ";
		std::cout << "b: " <<  ((int) palette[i * 3]      & 0xff) << "   " << std::endl;
	}
	for (int i = 0; i < 35; i++)
	{
		std::cout << std::endl;
	}
}

bool FractalView::handleInput(std::string input, float* centerx, float* centery)
{
	bool done = false;
	int sweepstate = -1;
	int lower, upper, step;
	while (!done)
	{
		if (input == "zoom")
		{
			x_max = (x_max + *centerx) / 2;
			x_min = (x_min + *centerx) / 2;
			y_max = (y_max + *centery) / 2;
			y_min = (y_min + *centery) / 2;
			done = true;
		}
		else if (input == "zoomo")
		{
			x_max += (x_max - *centerx);
			x_min -= (*centerx - x_min);
			y_max += (y_max - *centery);
			y_min -= (*centery - y_min);
			done = true;
		}
		else if (input == "up")
		{
			y_max += (y_max - *centery);
			y_min += (*centery - y_min);
			done = true;
		}
		else if (input == "down")
		{
			y_max -= (y_max - *centery);
			y_min -= (*centery - y_min);
			done = true;
		}
		else if (input == "left")
		{
			x_max -= (x_max - *centerx);
			x_min -= (*centerx - x_min);
			done = true;
		}
		else if (input == "right")
		{
			x_max += (x_max - *centerx);
			x_min += (*centerx - x_min);
			done = true;
		}
		else if (input == "save")
		{
			std::cout << std::endl << "Enter the config. name: ";
			std::string name;
			std::cin >> name;
			storeWindowSettings(name);
			done = true;
		}
		else if (input == "load")
		{
			std::cout << std::endl << "Enter the config. name: ";
			std::string name;
			std::cin >> name;
			if (!getWindowSettings(name))
			{
				std::cout << "Error reading file: invaild config file name." << std::endl;
			}
			std::cout << "Sucessfully loaded config file: " << name << std::endl;
			done = true;

		}
		else if (input == "help")
		{
			std::cout << "Commands: up down left right zoom zoomo load" << std::endl <<
				"save exit help mandelbrot julia juliaset setsize interations" << std::endl <<
				"sweep home" << std::endl;
			std::cin >> input;
		}
		else if (input == "mandelbrot")
		{
			currentFractal = Fractal::Mandelbrot;
			done = true;
		}
		else if (input == "julia")
		{
			currentFractal = Fractal::Julia;
			done = true;
		}
		else if (input == "ship")
		{
			currentFractal = Fractal::Ship;
			done = true;
		}
		else if (input == "print")
		{
			std::cout << "Enter Filename: ";
			std::string filename;
			std::cin >> filename;
			if (filename.empty())
			{
				filename = "unknown";
			}
			drawFractal(DrawMode::File, filename);
			std::cout << "Press enter to continue: " << std::endl;
			std::cin.get();
			std::cin.get();
			done = true;
		}
		else if (input == "juliaset")
		{
			if (currentFractal == Fractal::Mandelbrot)
			{
				julia_x = *centerx;
				julia_y = *centery;
			}
			else
			{
				std::cout << "x: ";
				float inputx, inputy;
				std::cin >> inputx;
				std::cout << " y: ";
				std::cin >> inputy;
				this->julia_x = inputx;
				this->julia_y = inputy;
			}
			done = true;
		}
		else if (input == "setsize")
		{
			int width, height;
			std::cout << "width: ";
			std::cin >> width;
			std::cout << std::endl << "height :";
			std::cin >> height;
			setImageSize(width, height);
			done = true;
		}
		else if (input == "iterations")
		{
			std::cout << "iterations: ";
			int iterations;
			std::cin >> iterations;
			setMaxIterations(iterations);
			done = true;
		}
		else if (input == "sweep")
		{
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
				setMaxIterations(sweepstate);
				drawFractal(DrawMode::Console, "");
				if (sweepstate >= upper)
				{
					done = true;
					break;
				}
			}
		}
		else if (input == "home")
		{
			if (currentFractal == Fractal::Julia || currentFractal == Fractal::Ship)
			{
				setWindowCenter(0, 0, 4, 2);
			}
			else
			{
				float zoom = 4;
				float x = -1.5f, y = 0.0f;
				setWindowCenter(x, -y, 2 * zoom, zoom);
			}
			done = true;
		}
		else if (input == "palette")
		{
		std::cout << "how many steps?" << std::endl;
			int steps = -1;
			while (steps > 5 || steps < 0)
			{
				std::cout << "enter a number between 1 and 4" << std::endl;
				std::cin >> steps;
			}
			// perform the color linear interpolation. Ive got one step figured out.
			done = true;
		}
		else if (input == "exit" || input == "quit")
		{
			return false;
		}
		else
		{
			std::cout << "Invalid command. Commands are: " << std::endl <<
			"Commands: up down left right zoom zoomo load" << std::endl <<
			"save exit help mandelbrot julia juliaset setsize interations" << std::endl <<
			"sweep home" << std::endl;
			std::cin >> input;
		}

	}
	
	return true;
}

float FractalView::colorPixel(int a, int b)
{
	// the pixel we're testing
	Complex z(a, b);
	// scale the coordinate to the real / imaginary space
	z.real = x_min + (a * (x_max - x_min) / image_width);
	z.imaginary = (y_min + (b * (y_max - y_min) / image_height));

	if (currentFractal == Fractal::Julia)
	{
		Complex z(a, b);
		Complex temp(a, b);
		// scale the coordinate to the real / imaginary space
		z.real = x_min + (a * (x_max - x_min) / image_width);
		z.imaginary = (y_min + (b * (y_max - y_min) / image_height));
		Complex c(julia_x, julia_y);

		temp.real = z.real;
		temp.imaginary = z.imaginary;
		int iteration = 0;
		while ((z.real * z.real) + (z.imaginary * z.imaginary)
			< 4 && iteration < max_iterations)
		{
			// iterate the function
			temp.real = z.real;
			temp.imaginary = z.imaginary;
			z = (z * z) + c;
			iteration++;
		}
		if (iteration == max_iterations)
		{
			// if the point is part of the set, paint it the inner color
			return -1;
		}
		// otherwise, just return the number of iterations it took to escape
		// and we'll paint it accordingly.
		float min = temp.distanceFromOrigin();
		float max = z.distanceFromOrigin();
		float gradient = (2 - min) / (max - min);

		return iteration + gradient;
	}
	else if (currentFractal == Fractal::Mandelbrot)
	{
		// the pixel we're testing
		Complex z(0, 0);
		Complex temp(0, 0);
		Complex c(x_min + (a * (x_max - x_min) / image_width),
			(y_min + (b * (y_max - y_min) / image_height)));
		// scale the coordinate to the real / imaginary space

		int iteration = 0;
		while ((z.real * z.real) + (z.imaginary * z.imaginary)
			< 4 && iteration < max_iterations)
		{
			// iterate the function
			temp.real = z.real;
			temp.imaginary = z.imaginary;
			z = (z * z) + c;
			iteration++;
		}
		if (iteration == max_iterations)
		{
			// if the point is part of the set, paint it the inner color
			return -1.0f;
		}
		// otherwise, just return the number of iterations it took to escape
		// and we'll paint it accordingly.
		float min = temp.distanceFromOrigin();
		float max = z.distanceFromOrigin();
		float gradient = (2 - min) / (max - min);

		return iteration + gradient;
	}
	else if (currentFractal == Fractal::Ship)
	{
		// the pixel we're testing
		Complex z(0, 0);
		Complex temp(0, 0);
		Complex c(x_min + (a * (x_max - x_min) / image_width),
			(y_min + (b * (y_max - y_min) / image_height)));
		// scale the coordinate to the real / imaginary space

		int iteration = 0;
		c.imaginary *= -1;
		while ((z.real * z.real) + (z.imaginary * z.imaginary)
			< 4 && iteration < max_iterations)
		{
			// iterate the function
			temp.real = z.real;
			temp.imaginary = z.imaginary;
			z = (z.abs().square()) + c;
			iteration++;
		}
		if (iteration == max_iterations)
		{
			// if the point is part of the set, paint it the inner color
			return -1;
		}
		// otherwise, just return the number of iterations it took to escape
		// and we'll paint it accordingly.
		float min = temp.distanceFromOrigin();
		float max = z.distanceFromOrigin();
		float gradient = (2 - min) / (max - min);

		return iteration + gradient;
	}
	else
	{
		return 0;
	}
	
}



