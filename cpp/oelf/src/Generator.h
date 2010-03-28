#pragma once

#include "Frame.h"

class Generator {
public:
	Generator();
	~Generator();
	void reset();
	void reflect();
	void setOrder(int order);
	int getOrder();
	void desync();
	void makeNext();
	void write(ostream& out);
	int getMagnitude();
	int getSize();
	const BigInteger& getFrame();
	const BigInteger& getGray();
private:
	int size;
	BigInteger frameBuffer;

	int* swap;

	Frame frame;
	long double samples;
	long double bits;

	int order, maskMagnitude;
	BigInteger counter, gray, shuffled;
};
