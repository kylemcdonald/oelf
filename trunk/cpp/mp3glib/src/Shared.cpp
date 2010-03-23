#include "Shared.h"

#define combine(a, b, mask) ((a & mask) | (b & ~mask))

void setShort(byte* array, int& position, unsigned short value, int length) {
	byte* bvalue = (byte*) &value;
	if(length > 8) {
		byte filled = length - 8;
		setByte(array, position, bvalue[1], filled);
		length = 8;
	}
	setByte(array, position, bvalue[0], length);
}

void setByte(byte* array, int& position, byte value, int length) {
	// nothing to copy
	if(length == 0)
		return;

	// for 0-padded bitstrings stored as bytes
	if(length > 8) {
		int extra = length - 8;
		setByte(array, position, 0, extra);
		length -= extra;
	}

	// get byte and bit offset
	unsigned int whichByte = position >> 3;
	byte whichBit = position - (whichByte << 3);

	// handle the left hand side if it crosses a boundary
	byte shift, mask;
	if(whichBit + length > 8) {
		shift = whichBit + length - 8;
		byte shifted = value;
		shifted >>= shift;
		byte filled = 8 - whichBit;
		byte mask = ((1 << filled) - 1);
		array[whichByte] = combine(shifted, array[whichByte], mask);
		position += filled;
		length -= filled;
		whichByte++;
		whichBit = 0;
	}

	// handle the remaining portion
	shift = 8 - whichBit - length;
	value <<= shift;
	mask = ((1 << length) - 1) << shift;
	array[whichByte] = combine(value, array[whichByte], mask);
	position += length;
}

void setBool(byte* array, int& position, bool value) {
	unsigned int whichByte = position >> 3;
	byte whichBit = position - (whichByte << 3);
	byte mask = 1 << (7 - whichBit);
	if(value)
		array[whichByte] |= mask;
	else
		array[whichByte] &= ~mask;
	position++;
}

string binary(short x, int length) {
	string out = "";
	byte* y = (byte*) &x;
	if(length > 8) {
		out += binary(y[1], length - 8);
		length = 8;
	}
	out += binary(y[0], length);
	return out;
}

string binary(byte x, int length) {
	string out = "";
	for(int i = 0; i < length; i++) {
		int position = length - i - 1;
		byte mask = 1 << position;
		out += mask & x ? "1" : "0";
	}
	return out;
}

string binary(byte* x, int length) {
	string out = "";
	for(int i = 0; i < length; i++) {
		out += binary(x[i], 8);
		if(i != length - 1)
			out += " ";
	}
	return out;
}
