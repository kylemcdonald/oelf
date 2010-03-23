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
	if(length == 0)
		return;
	unsigned int whichByte = position >> 3;
	byte whichBit = position - (whichByte << 3);
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
