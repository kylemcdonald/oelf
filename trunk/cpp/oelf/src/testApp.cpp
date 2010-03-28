#include "testApp.h"

void testApp::setup() {
	ofSetVerticalSync(true);
	ofSetFrameRate((int) (38.28 / (float) VIZFRAMES));
	img.allocate(generator.getSize(), VIZFRAMES, OF_IMAGE_COLOR);
}

void testApp::update(){
	if(true) {
		byte* pixels = img.getPixels();
		int i = 0;
		for(int j = 0; j < VIZFRAMES; j++) {
			generator.makeNext();
			const BigInteger& ref = generator.getFrame();
			const BigInteger& mask = generator.getMask();
			for(int k = 0; k < ref.size(); k++) {
				bool curBit = ref.testBit(k);
				if(mask.testBit(k)) {
					pixels[i++] = curBit ? 255 : 0;
					pixels[i++] = curBit ? 255 : 0;
					pixels[i++] = curBit ? 255 : 0;
				} else {
					pixels[i++] = curBit ? 255 : 64;
					pixels[i++] = curBit ? 0 : 0;
					pixels[i++] = curBit ? 0 : 0;
				}
			}
			img.update();
		}
	}
}

void testApp::draw(){
	int offset = (int) ofMap(mouseX, 0, ofGetWidth(), 0, ofGetWidth() - img.getWidth());
	img.draw(offset, 0, img.getWidth(), ofGetHeight());

	if(DEBUG) {
		ofSetColor(0, 0, 0);
		ofRect(2, 2, 30, 12);
		ofSetColor(255, 255, 255);
		ofDrawBitmapString(ofToString(generator.getOrder()), 6, 12);
	}
}

void testApp::keyPressed(int key) {
	if(key == ' ' && DEBUG)
		generateMp3();
}

void testApp::mousePressed(int x, int y, int button) {
	if(DEBUG) {
		if(x < ofGetWidth() / 2) {
			generator.setOrder((int) ofMap(mouseX, 0, ofGetWidth(), 0, 128));
		} else {
			generator.reflect();
		}
	}
}

void testApp::generateMp3() {
	system("move *.mp3 backup");

	ostringstream filename;
	filename << time(NULL) << "-" << generator.getOrder() << "-" << SKIP_AMOUNT << ".mp3";

	ofstream file;
	file.open(filename.str().c_str(), std::ios::binary | std::ios::out);
	//generator.reset();
	for(int i = 0; i < (15 * 60 * 44100) / 1152; i++) {
	//for(int i = 0; i < 512; i++) {
		generator.makeNext();
		generator.write(file);
	}
	file.close();
	cout << "Wrote " << filename.str() << " out." << endl;
}
