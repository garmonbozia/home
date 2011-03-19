#ifndef WCOMPLEX_H
#define WCOMPLEX_H

#include <stdio.h>

// sample class of complex-like numbers with weird `==` operator
class wComplex{
private:
	double realPart;
	double imagePart;

public:
	wComplex();
	wComplex(double R);
	wComplex(double R, double I);
	void operator=(wComplex);
	bool operator==(wComplex &);
	void print();
};

wComplex::wComplex(){
	realPart  = 0;
	imagePart = 0;
}

wComplex::wComplex(double R){
	realPart  = R;
	imagePart = 0;
}

wComplex::wComplex(double R, double I)
{
	realPart  = R;
	imagePart = I;
}

void wComplex::operator=(wComplex El)
{
	realPart  = El.realPart;
	imagePart = El.imagePart;
}

bool wComplex::operator==(wComplex &El)
{
	double diff = realPart*realPart + imagePart*imagePart - 
	El.realPart*El.realPart - El.imagePart*El.imagePart;
	return (diff == 0);
}

void wComplex::print()
{
	printf("(%g) + (%g)i\n", realPart, imagePart);
}

#endif
