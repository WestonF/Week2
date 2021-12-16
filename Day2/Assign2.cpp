#include <iostream>
#include <cmath>
#include <cassert>

//trial of first few terms suggests that this formula only works reliably for n >= 2 AND assumes a 1-index on the fib sequence, not 0-index.
//last program suggested that 46 was too high for a signed int, but that was zero-indexed, so term 47 should be the smallest "too high" with this formula.
// Other than the brute force method, the two remaining practical methods would be guess-and-check or to invert the formula, solve for n, and solve the inequality for the maximum value of the int container, in this case long.
//I took a stab at manually solving Binet's formula for n, but it was beyond the scope of practicality, so I decided to give a guess-check-and refine function a go. This worked, so I didn't go back to the solving the equation plan.
//This program was written with ints for the return of fibterm and guess::fibout because I knew what the limit was there, then switched them to longs to fit the assignment's goal.

//92 is the largest fibbonaci term that fits in a long int.

long fibterm(int n)
{
	double root5 = pow(5, 0.5);
	double one_ov_root5 = 1. / root5;
	double posterm = (1 + root5) / 2.;
	double negterm = (1 - root5) / 2.;
	long fibterm = (one_ov_root5 * pow(posterm, n)) - (one_ov_root5 * pow(negterm, n));
	if(fibterm < 0)
	{
	//	std::cout << "Value cannot be held in this container" << std::endl;	//for prototyping only
	}
	return fibterm;	

}

int guess (int g)
{
	int lastg = g;
	int upperlim = 9999999; //arbitrarily selected high number
	int lowerlim = 2; 	//the function doesnt work below this number
	long fibout;
	while(1)
	{
		fibout = fibterm(g);
		if(fibout < 0)
		{
			//guess was too high
			upperlim = g;	//don't ever go this high again

			g = g - ( g / 2);
		
			if ( g <= lowerlim)
			{
				g = lowerlim + 1; //we already had g at lowerlim, get higher
			}
			
		}
		else
		{
			//guess was too low
			lowerlim = g;	//dont ever go this low again

			g = g + ( g / 2);
			
			if (g >= upperlim)
			{
				g = upperlim - 1;	//we already had g at upperlim, get lower
			}
		}


		if (g == lastg)
		{
			if (fibout < 0)
			{
				g--; //if the last round overshot, decrement from lowest invalid to highest valid
			}

			return g;
		}
		else
		{
			lastg = g;
//			std::cout <<"g = " << lastg << '\n' << "upperlim = " << upperlim << '\n' << "lowerlim = " << lowerlim << std::endl;	//for prototyping only
		}
	}
}


int main()
{
	int input;
	
	std::cout << "Select a Fibbonaci term guess >" << std::endl;
	std::cin >> input;
	std::cout << "The largest Fibbonaci term that fits in a long int is: " << guess(input) << std::endl;
	

	return 0;
}
