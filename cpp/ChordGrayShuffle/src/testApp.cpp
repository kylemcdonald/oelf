#include "testApp.h"

void testApp::setup(){
	counter.setup(BITS);
	counter.set(0);

	shifted.setup(counter);
	gray.setup(counter);
	shuffled.setup(counter);

	img.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_GRAYSCALE);
}

void testApp::update(){
	counter.clear();
	/*
	for(int i = 0; i < mouseY * 32; i++)
		counter.chordIncrement();*/

	order = (int) ofMap(mouseX, 0, ofGetWidth(), 0, 128);

	byte* pixels = img.getPixels();
	int h = (int) img.getHeight();
	int i = 0;
	for(int j = 0; j < h; j++) {
		shifted.set(counter);
		gray.set(counter);
		for(int k = 0; k < order; k++) {
			gray ^= shifted.shiftRight(); // make gray code
			shuffled.clear(); // clear shuffle buffer
			gray.shuffleInto(shuffled); // make shuffle

			shifted.set(shuffled); // copy result
			gray.set(shuffled); // copy result
		}

		for(int k = 0; k < BITS; k++)
			pixels[i++] = shuffled.testBit(k) ? 255 : 0;

		counter.chordIncrement();
	}
	img.update();
}

void testApp::draw(){
	img.draw(0, 0);
}

void testApp::keyPressed(int key) {
	ostringstream filename;
	filename << BITS << "x" << (int) ofGetHeight() << " " << order << " order.png";
	img.saveImage(filename.str());
}
