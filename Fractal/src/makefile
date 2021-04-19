FractalView: Complex.o ConsoleDrawer.o FractalView.o main.o
	g++ -o fractal Complex.o ConsoleDrawer.o FractalView.o main.o

Complex.o: Complex.h Complex.cpp
	g++ -c Complex.h Complex.cpp

ConsoleDrawer.o: ConsoleDrawer.h ConsoleDrawer.cpp
	g++ -c ConsoleDrawer.h ConsoleDrawer.cpp

FractalView.o: FractalView.cpp FractalView.h Complex.h ConsoleDrawer.h
	g++ -c FractalView.h FractalView.cpp

main.o: main.cpp FractalView.cpp FractalView.h Complex.h ConsoleDrawer.h
	g++ -c main.cpp

