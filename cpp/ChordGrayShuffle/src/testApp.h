#pragma once

#include "ofMain.h"

#define BITS 1024

#include "BigInteger.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);

	BigInteger counter, gray, shuffled;
	int order;

	ofImage img;
};
