#include "Shared.h"

#define combine(a, b, mask) ((a & mask) | (b & ~mask))

void set(byte* array, unsigned int position, byte value, unsigned int length) {
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
		length -= filled;
		whichByte++;
		whichBit = 0;
	}
	shift = 8 - whichBit - length;
	value <<= shift;
	mask = ((1 << length) - 1) << shift;
	array[whichByte] = combine(value, array[whichByte], mask);
}

void set(byte* array, unsigned int position, bool value) {
	unsigned int whichByte = position >> 3;
	byte whichBit = position - (whichByte << 3);
	byte mask = 1 << whichBit;
	if(value)
		array[whichByte] |= mask;
	else
		array[whichByte] &= ~mask;
}
