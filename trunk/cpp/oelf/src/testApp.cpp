#include "testApp.h"

void testApp::setup() {
	system("move *.mp3 backup");

	ostringstream filename;
	filename << time(NULL);
	filename << ".mp3";

	ofstream file;
	file.open(filename.str().c_str(), std::ios::binary | std::ios::out);
	Generator generator;
	//for(int i = 0; i < (10 * 44100) / 1152; i++) {
	for(int i = 0; i < 512; i++) {
		generator.makeNext();
		generator.write(file);
	}
	file.close();

	cout << "Wrote " << filename.str() << " out." << endl;

	img.allocate(generator.getMagnitude(), ofGetHeight(), OF_IMAGE_GRAYSCALE);
}

void testApp::update(){
	/*
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
	*/
}

void testApp::draw(){
	img.draw(0, 0, ofGetWidth(), ofGetHeight());
}

void testApp::keyPressed(int key) {
}

void testApp::mousePressed(int x, int y, int button) {
}
