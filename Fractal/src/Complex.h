#include <iostream>
#include <math.h>
class Complex
{
public:

	float real, imaginary;
	Complex(float rp, float ip)
	{
		//set the real and imaginary parts to the arguments
		this->imaginary = ip;
		this->real = rp;
	}

	Complex()
	{
		//set the real and imaginary parts to zero
		this->imaginary = 0.0f;
		this->real = 0.0f;
	}

	void printComplex()
	{
		//if the imaginary part is negative:
		if (imaginary < 0)
		{
			//print the number with a negative sign
			std::cout << real << " " << imaginary << "i" << std::endl;
		}
		//if the imaginary part is zero:
		else if (imaginary == 0)
		{
			//only print the real part, there is no imaginary part.
			std::cout << real << std::endl;
		}
		//if the imaginary part is positive:
		else
		{
			//print the number with a plus sign
			std::cout << real << " + " << imaginary << "i" << std::endl;
		}
	}

	Complex operator+(Complex& other)
	{
		//return the sum of the complex numbers
		return Complex(real + other.real, imaginary + other.imaginary);
	}

	Complex operator-(Complex& other)
	{
		//return the difference of the complex numbers
		return Complex(real - other.real, imaginary - other.imaginary);
	}

	Complex operator*(Complex& other)
	{
		//return the product of the complex numbers
		return Complex(
			(real * other.real) - (imaginary * other.imaginary),
			(real * other.imaginary) + (imaginary * other.real)
		);
	}

	Complex abs()
	{
		return Complex(std::abs(real), std::abs(imaginary));
	}
	Complex square()
	{
		return Complex(
			(real * real) - (imaginary * imaginary),
			(real * imaginary) * 2
		);
	}
};
