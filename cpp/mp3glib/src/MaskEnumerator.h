#pragma once

#include "Enumerator.h"

#define BITS 2048
#include "BigInteger.h"

class MaskEnumerator : public Enumerator {
public:
	MaskEnumerator() :
			order(49) {
		counter.setup(BITS);
		counter.set(0);

		shifted.setup(counter);
		gray.setup(counter);
		shuffled.setup(counter);

		frameMask.setup(418 * 8);
		byte* mask = frameMask.getData();
		memset(mask, 0xff, 418); // assume everything is allowed
		frame.getMask(mask);

		frameBuffer.setup(418 * 8);
	}
	void write(ostream& out) {
		frameBuffer.write(out);
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
		frame.write(frameBuffer.getData());
		gray &= frameMask;
		//frameBuffer |= gray;
	}
private:
	BigInteger frameBuffer, frameMask;
	BigInteger counter, shifted, gray, shuffled;
	int order;
};
