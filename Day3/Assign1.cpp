#include <iostream>
#include <cassert>


int add(int arg1, int arg2);
int subtract(int arg1, int arg2);
int multiply(int arg1, int arg2);

int (*pfunc[])(int, int) = {&add, &subtract, &multiply};

enum math_op
{
	ADD,
	SUBTRACT,
	MULTIPLY
};

int compute(int arg1, int arg2, unsigned char funcmask);


int main()
{
	int a = 15, b = 4, c;
	unsigned char mask = 1;

	c = compute(a, b, mask);	//add
	assert(c == a + b);
	
	mask = 2;
	c = compute(a, b, mask);	//subtract
	assert(c == a - b);

	mask = 4;
	c = compute(a, b, mask);	//multiply
	assert(c == a * b);

	mask = 3;
	c = compute(a, b, mask);	//invalid
	assert(c == -1);

	return 0;
}


int compute(int arg1, int arg2, unsigned char funcmask)
{
	
	if( funcmask == 1)
	{
		return pfunc[math_op::ADD](arg1, arg2);
	}
	else if( funcmask == 2)
	{
		return pfunc[math_op::SUBTRACT](arg1, arg2);
	}
	if( funcmask == 4)
	{
		return pfunc[math_op::MULTIPLY](arg1, arg2);
	}
	else
	{
		std::cout << "Invalid function selection" << std::endl;
	}

	return -1;
}

int add(int arg1, int arg2)
{
	return arg1 + arg2;
}

int subtract(int arg1, int arg2)
{
	return arg1 - arg2;
}

int multiply(int arg1, int arg2)
{
	return arg1 * arg2;
}
