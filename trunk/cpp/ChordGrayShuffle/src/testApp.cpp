#include "testApp.h"

void testApp::setup(){
	counter.setup(BITS);
	counter.set(0);

	shifted.setup(counter);
	gray.setup(counter);

	img.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_GRAYSCALE);
}

void testApp::update(){
	counter.clear();
	for(int i = 0; i < mouseY * 10; i++)
		counter.chordIncrement();

	byte* pixels = img.getPixels();
	int h = (int) img.getHeight();
	int i = 0;
	for(int j = 0; j < h; j++) {
		shifted.set(counter);
		gray.set(counter);
		gray ^= shifted.shiftRight();

		for(int k = 0; k < BITS; k++)
			pixels[i++] = gray.testBit(k) ? 255 : 0;

		counter.chordIncrement();
	}
	img.update();
}

void testApp::draw(){
	img.draw(0, 0);
}
