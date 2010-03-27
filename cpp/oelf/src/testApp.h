#pragma once

#include "ofMain.h"

#define BITS 800

#include "BigInteger.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void mousePressed(int x, int y, int button);

	BigInteger counter, gray, shuffled;
	int order;

	ofImage img;
};
