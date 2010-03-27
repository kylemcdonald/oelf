#include "testApp.h"

void testApp::setup(){
	counter.setup(BITS);
	counter.set(0);

	gray.setup(counter);
	shuffled.setup(counter);

	img.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_GRAYSCALE);
}

void testApp::update(){
	//counter.clear();

	for(int i = 0; i < mouseY; i++)
		counter.chordIncrement();

	order = (int) ofMap(mouseX, 0, ofGetWidth(), 0, 128);

	byte* pixels = img.getPixels();
	int h = (int) img.getHeight();
	int i = 0;
	for(int j = 0; j < h; j++) {
		gray.set(counter);
		for(int k = 0; k < order; k++) {
			gray.convertToGray();
			shuffled.clear(); // clear shuffle buffer
			gray.shuffleInto(shuffled); // make shuffle
			gray.set(shuffled); // copy result
		}

		for(int k = 0; k < BITS; k++)
			pixels[i++] = shuffled.testBit(k) ? 255 : 0;

		//counter.chordIncrement();
		counter.binaryIncrement();
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

void testApp::mousePressed(int x, int y, int button) {
	counter.set(gray);
}
