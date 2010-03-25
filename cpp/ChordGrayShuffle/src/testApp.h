#pragma once

#include "ofMain.h"
#include "BigInteger.h"

#define BITS 512

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();

	BigInteger counter, shifted, gray;

	ofImage img;
};
