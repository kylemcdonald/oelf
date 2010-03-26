#pragma once

#include "Frame.h"

class Enumerator {
public:
	Enumerator() :
			samples(0), bits(0) {
	}
	virtual ~Enumerator() {
	}
	void getNext() {
		makeNext();

		if(samples != 0) {
			long double curBitrate = (bits * frame.header.getSamplerate()) / samples;
			if(curBitrate < frame.header.getBitrate())
				frame.header.setPadding(true);
			else
				frame.header.setPadding(false);
		}

		samples += FRAME_SAMPLES;
		bits += frame.getSize() * 8;
	}
	virtual void write(ostream& out) = 0;
protected:
	virtual void makeNext() = 0;

	Frame frame;
	long double samples;
	long double bits;
};
