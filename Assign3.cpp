#include <iostream>
#include <cassert>

long int PowerOfTwo(long int input, bool loud = false);

int main()
{
	long int input;
	std::cout << "Enter an integer > ";
	std::cin >> input;
	PowerOfTwo(input, true);
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
