#pragma once

#include "Frame.h"

class Generator {
public:
	Generator();
	void setOrder(int order);
	void desync();
	void makeNext();
	void write(ostream& out);
	int getMagnitude();
private:
	BigInteger frameBuffer, mask;

	Frame frame;
	long double samples;
	long double bits;

	int order, maskMagnitude;
	BigInteger counter, gray, shuffled;
};
