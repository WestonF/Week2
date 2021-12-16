#include <iostream>
#include <cassert>

//PROBLEMS ENCOUNTERED:
//First, this takes a very long time to execute, as was apparent due to the excessive delay between the console's run instruction and the time where any output was visible. Considered adding an output to each run to see progress, but decided against due to the absurd number of displays that would occur. The slowness is unsurprising, as every call of fib with an argument value higher than 1 must call two more instances of fib, which each must call two more (assuming their arguments are still higher than 1), and so on. Initial impression is that this calls somewhere around 2**48 + 2 instances of fib, but I've not done the math to verify or refute that.

//Second, term 50 of the fibbonaci sequence is apparently larger than the value 31 bits of a signed integer can hold without overflow. Fortunately, I predicted this possibility and added a step to check for this possibility. Considered using an assert, but decided I wanted to be informed of the occurrance, but not necessarily interupt program operation, particularly given the amount of time the program was expected to take per run

//No errors were announced at compile or runtime except the note from the above comment.


int fib(unsigned int i);

int main()
{
	fib(50);

	return 0;
}



int fib(unsigned int i)
{
	int out = 0;
	if( i > 1)
	{
		out = fib(i-2) + fib(i-1);
	}
	else if( i == 0)
	{
		out = 1;
	}
	else if( i == 1)
	{
		out = 1;
	}
	
	if( out <= 0) //any negative output would indicate an overflow, which would mean the results were now garbage
	{
		//from previous run: term 46 onward exceed the capacity of a signed int
		std::cout << "Term " << i << "exceeds the capacity of signed int" <<std::endl;
	}

	return out;
}
