#include "Generator.h"
Generator::Generator() :
		samples(0),
		bits(0),
		order(16) {
	frame.setPadding(true);
	size = frame.getSize() * 8;
	cout << "Frame size is " << size << " bits." << endl;

	// load in mask
	mask.setup(size);
	frameBuffer.setup(size);
	frame.writeMask(mask.getData());
	maskMagnitude = mask.getMagnitude();
	cout << "Magnitude of mask is " << maskMagnitude << " bits." << endl;

	swap = new int[size];
	int cur = 0;
	for(int i = 0; i < size; i++)
		if(mask.testBit(i))
			swap[cur++] = i;

	// more fine revisions could be made here
	swapRegions(1024, maskMagnitude - 1 - 512, 128);
	reverseRegion(600, 512);

	// set up the counter for enumeration
	counter.setup(maskMagnitude);
	counter.set(0);
	gray.setup(counter);
	shuffled.setup(counter);
}

Generator::~Generator() {
	delete [] swap;
}

void Generator::setOrder(int order) {
	this->order = order;
}

int Generator::getOrder() {
	return order;
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

void Generator::reset() {
	counter.clear();
}

void Generator::reflect() {
	counter.set(gray);
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
	//counter.chordIncrement();
	desync();

	frameBuffer.clear();
	frame.write(frameBuffer.getData());
	for(int i = 0; i < maskMagnitude; i++)
		if(gray.testBit(i))
			frameBuffer.setBit(swap[i]);
}

void Generator::write(ostream& out) {
	out.write((char*) frameBuffer.getData(), frame.getSize());
}

int Generator::getMagnitude() {
	return maskMagnitude;
}

int Generator::getSize() {
	return size;
}

const BigInteger& Generator::getFrame() {
	return frameBuffer;
}

const BigInteger& Generator::getGray() {
	return gray;
}

const BigInteger& Generator::getMask() {
	return mask;
}

void Generator::swapRegions(int from, int to, int length) {
	for(int i = 0; i < length; i++) {
		int cur = swap[from + i];
		swap[from + i] = swap[to + i];
		swap[to + i] = cur;
	}
}

void Generator::reverseRegion(int start, int length) {
	for(int i = 0; i < length; i++) {
		int cur = swap[start + i];
		swap[start + i] = swap[start + (length - 1 - i)];
		swap[start + (length - 1 - i)] = cur;
	}
}
