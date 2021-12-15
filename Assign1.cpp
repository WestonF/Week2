#include <iostream>
#include <cmath>
#include <cassert>
#include <iomanip>

long int PowerOfTwo(long int input, bool loud = false);

int main()
{
	float input = 1;
	std::cout << "Please enter a fractional value > ";
	std::cin >> input;

	long two_to_N = 1; //N = 2**0;
	two_to_N <<= 56; // N = 2**56

 	long int J = round(input * two_to_N);
	//simplify the fraction
	while(J/2.0 ==	J/2)
	{
		J/=2;
		two_to_N >>= 1;
		assert(J != 0);
	}

	std::cout << "J = " << J << std::endl;
	std::cout << "N = " << PowerOfTwo(two_to_N) << std::endl;
	std::cout << std::setprecision(32) << std::fixed;
	std::cout << input << std::endl;
	return 0;
}


long int PowerOfTwo(long int input, bool loud)
{
	long int comparitor = 1; // 2**0
	long int power = 0;
	while (input > comparitor)
	{
		comparitor <<= 1;
		power++;
	}

	if(input == comparitor)
	{
		if(loud)
		std::cout << input << " is equal to 2**" << power << std::endl;
		return power;
	}
	if(loud)
	std::cout << input << " is not equal to a power of two" << std::endl;
	return -1;
}
