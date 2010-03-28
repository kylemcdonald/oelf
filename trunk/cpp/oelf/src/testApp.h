#pragma once

#include <fstream>
#include "ofMain.h"
#include "BigInteger.h"
#include "Frame.h"
#include "Generator.h"

#define VIZFRAMES 16
#define DEBUG true

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void mousePressed(int x, int y, int button);
	void generateMp3();

	ofImage img;
	Generator generator;
};
