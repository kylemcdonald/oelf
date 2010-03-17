#pragma once
#include "Shared.h"

class Header {
public:
	Header();
	ostream& write(ostream& out);
private:
	unsigned char header[4];
};

/*
	12 syncword: xFFF (or, x7FF for v2.5)
	1 id: 1, indicates v1 rather than v2
	2 layer: 01 for layer 3
	1 protection: 1, no error checking
	4 bitrate: 4 bits, 1001 for 128 kbit/s
	2 samplerate: frequency_index, 00 is 44100 kHz
	1 padding: whether there is an extra byte at the end or not
	1 private: unused, 0
	2 mode: 11 (mono)
	2 mode extension: unused, 00
	1 copyright: unused, always export 0
	1 original: unused, always export 1
	2 emphasis: no emphasis, 00
*/
