#pragma once

#include "Enumerator.h"

#define BITS 2048
#include "BigInteger.h"

class MaskEnumerator : public Enumerator {
public:
	MaskEnumerator() :
			order(0) {
		counter.setup(BITS);
		counter.set(0);

		shifted.setup(counter);
		gray.setup(counter);
		shuffled.setup(counter);
	}
protected:
	void incrementCounter() {
		shifted.set(counter);
		gray.set(counter);
		for(int k = 0; k < order; k++) {
			gray ^= shifted.shiftRight(); // make gray code
			shuffled.clear(); // clear shuffle buffer
			gray.shuffleInto(shuffled); // make shuffle
			shifted.set(shuffled); // copy result
			gray.set(shuffled); // copy result
		}
		counter.chordIncrement();
	}
	void makeNext() {
		incrementCounter();
	}
private:
	BigInteger counter, shifted, gray, shuffled;
	int order;
};
