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

/*
// alternative solution: cast to shorts. not flexible or multiplatform.
void setByte(byte* array, int& position, byte value, int length) {
	// determine byte and bit offsets
	unsigned int whichByte = position >> 3;
	byte whichBit = position - (whichByte << 3);

	// cast byte array to short array, reorder bytes
	unsigned short* sarray = (unsigned short*) &array[whichByte];
	byte swap = array[whichByte];
	array[whichByte] = array[whichByte + 1];
	array[whichByte + 1] = swap;

	// cast value as short, shift, make a mask
	unsigned short svalue = (unsigned short) value;
	byte shift = 16 - whichBit - length;
	svalue <<= shift;
	unsigned short mask = ((1 << length) - 1) << shift;

	// mask/combine everything
	*sarray = combine(svalue, *sarray, mask);

	// swap bytes back around
	swap = array[whichByte];
	array[whichByte] = array[whichByte + 1];
	array[whichByte + 1] = swap;

	position += length;
}
*/

void setBool(byte* array, int& position, bool value) {
	unsigned int whichByte = position >> 3;
	byte whichBit = position - (whichByte << 3);
	byte mask = 1 << whichBit;
	if(value)
		array[whichByte] |= mask;
	else
		array[whichByte] &= ~mask;
	position++;
}
