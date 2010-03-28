#pragma once

#include "Frame.h"

class Generator {
public:
	Generator() :
			samples(0),
			bits(0),
			order(50) {
		frame.setPadding(true);
		int size = frame.getSize() * 8;
		cout << "Frame size is " << size << " bits." << endl;

		// load in mask
		mask.setup(size);
		frameBuffer.setup(size);
		frame.writeMask(mask.getData());
		int maskMagnitude = size;//mask.getMagnitude(); // temporarily use entire mask
		cout << "Magnitude of mask is " << maskMagnitude << " bits." << endl;

		// set up the counter for enumeration
		counter.setup(maskMagnitude);
		counter.set(0);
		gray.setup(counter);
		shuffled.setup(counter);
	}
	void setOrder(int order) {
		this->order = order;
	}
	void makeNext() {
		if(samples != 0) {
			long double curBitrate = (bits * frame.getSamplerate()) / samples;
			if(curBitrate < frame.getBitrate())
				frame.setPadding(true);
			else
				frame.setPadding(false);
		}

		samples += FRAME_SAMPLES;
		bits += frame.getSize() * 8;

		gray.set(counter);
		for(int k = 0; k < order; k++) {
			gray.convertToGray();
			shuffled.clear(); // clear shuffle buffer
			gray.shuffleInto(shuffled); // make shuffle
			gray.set(shuffled); // copy result
		}
		counter.binaryIncrement();

		frameBuffer.clear();
		frame.write(frameBuffer.getData());
		gray &= mask;
		frameBuffer |= gray;
	}
	void write(ostream& out) {
		out.write((char*) frameBuffer.getData(), frame.getSize());
	}
	int getMagnitude() {
		return maskMagnitude;
	}
private:
	BigInteger frameBuffer, mask;

	Frame frame;
	long double samples;
	long double bits;

	int order, maskMagnitude;
	BigInteger counter, gray, shuffled;
};
