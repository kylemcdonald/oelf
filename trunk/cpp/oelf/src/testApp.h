#pragma once

#include "ofMain.h"
#include "BigInteger.h"
#include "Frame.h"
#include "Generator.h"

#define BITS 1024

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
