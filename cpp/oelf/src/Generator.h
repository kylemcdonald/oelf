#pragma once

#include "Frame.h"

//#define USE_MIDDLE

#ifdef USE_MIDDLE
	#include <stdio.h>
	#include <stdlib.h>
	#include <time.h>
#endif

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
	void writeState(ostream& out);
	void readState(istream& in);
	int getMagnitude();
	int getSize();
	const BigInteger& getFrame();
	const BigInteger& getGray();
	const BigInteger& getMask();
	const BigInteger& getCounter();
	void swapRegions(int from, int to, int length);
	void reverseRegion(int start, int length);
private:
	int size;
	BigInteger frameBuffer;

	int* swap;

	Frame frame;
	long double samples;
	long double bits;

	int order, maskMagnitude;
	BigInteger mask, counter, gray, shuffled;
};
