#include "Generator.h"
Generator::Generator() :
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
	maskMagnitude = mask.getMagnitude();
	cout << "Magnitude of mask is " << maskMagnitude << " bits." << endl;

	// set up the counter for enumeration
	counter.setup(maskMagnitude);
	counter.set(0);
	gray.setup(counter);
	shuffled.setup(counter);
}

void Generator::setOrder(int order) {
	this->order = order;
}

void Generator::desync() {
	int consecutive = 0;
	for(int i = 0; i < gray.size(); i++) {
		if(gray.testBit(i))
			consecutive++;
		else
			consecutive = 0;
		if(consecutive == 12) {
			gray.clearBit(i);
			consecutive = 0;
		}
	}
}

void Generator::makeNext() {
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
	gray &= mask;
	desync();

	frameBuffer.clear();
	frame.write(frameBuffer.getData());
	for(int i = 0; i < maskMagnitude; i++) {

	}
}

void Generator::write(ostream& out) {
	out.write((char*) frameBuffer.getData(), frame.getSize());
}

int Generator::getMagnitude() {
	return maskMagnitude;
}
