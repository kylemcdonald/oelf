#include "Header.h"

/*
	12 syncword: xFFF
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

Header::Header() :
		padding(false) {
	header[0] = 0xff; // 1111 1111
	header[1] = 0xfb; // 1111 1011
	header[2] = 0x90; // 1001 00p0
	header[3] = 0xc4; // 1100 0100
}

bool Header::getPadding() const {
	return padding;
}

int Header::getBitrate() const {
	return 128000;
}

int Header::getSamplerate() const {
	return 44100;
}

void Header::setPadding(bool padding) {
	// at 128 kbps, 44.1 kHz, most frames will be padded
	this->padding = padding;
	int paddingPosition = 22;
	setBool(header, paddingPosition, padding);
}

void Header::writeHeader(byte* data, int& position) const {
	// as header is always byte-aligned, this could just be a memcpy
	for(int i = 0; i < 4; i++)
		setByte(data, position, header[i], 8);
}
