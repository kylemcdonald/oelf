#pragma once

#include "Frame.h"

class Generator {
public:
	Generator() {
		frame.setPadding(true);
		int size = frame.getSize();
		mask = new byte[size];
		frameBuffer = new byte[size];
	}
	void
private:
	byte* mask;
	byte* frameBuffer;
	Frame frame;
};
