#include <iostream>
#include <cassert>

struct basicthing
{
	public:
	float myfloat = 0;

	void myfunct(float f);
	void myfunct(float f) const;
};

int main()
{
 	const basicthing A;
	basicthing B;
	float c = 0.125;

	assert (A.myfloat == 0);
	assert (B.myfloat == 0);
	
	A.myfunct(c);
	B.myfunct(c);

	assert (A.myfloat == 0);
	assert (B.myfloat == c);

	return 0;
}

void basicthing::myfunct (float f)
{
	myfloat = f;
	std::cout << "Input: " << f << std::endl;
	std::cout << "Output: " << myfloat << std::endl;
}

void basicthing::myfunct (float f) const
{
	std::cout << "Input: " << f << std::endl;
	std::cout << "Output: " << myfloat << std::endl;
}
