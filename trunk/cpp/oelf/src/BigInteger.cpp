#include "BigInteger.h"

byte* BigInteger::magnitudeLut = NULL;

BigInteger::BigInteger() :
	bitCount(0),
	byteCount(0),
	data(NULL),
	shuffleLut(NULL) {
}

void BigInteger::setup(const BigInteger& from) {
	setup(from.bitCount);
	set(from);
}

void BigInteger::setup(int bitCount) {
	this->bitCount = bitCount;
	this->byteCount = (bitCount >> 3) + (bitCount ? 1 : 0);
	this->maxByte = byteCount - 1;
	this->maxBit = bitCount - 1;
	data = new byte[byteCount];
	clear();
}

void BigInteger::set(const BigInteger& from) {
	memcpy(data, from.data, byteCount);
}

string BigInteger::toString() const {
	string out = "";
	for(int i = 0; i < bitCount; i++)
		out += testBit(i) ? "1" : "0";
	return out;
}

void BigInteger::write(ostream& out, int byteCount) const {
	out.write((char*) data, byteCount);
}

void BigInteger::write(ostream& out) const {
	out.write((char*) data, byteCount);
}

void BigInteger::read(istream& in) {
	in.read((char*) data, byteCount);
}

byte* BigInteger::getData() {
	return data;
}

void BigInteger::set(byte x) {
	data[maxByte] = x;
}

void BigInteger::set(short x) {
	data[maxByte] = ((byte*) &x)[0];
	data[maxByte - 1] = ((byte*) &x)[1];
}

void BigInteger::set(int x) {
	data[maxByte] = ((byte*) &x)[0];
	data[maxByte - 1] = ((byte*) &x)[1];
	data[maxByte - 2] = ((byte*) &x)[2];
	data[maxByte - 3] = ((byte*) &x)[3];
}

void BigInteger::set(const byte* x) {
	memcpy(data, x, byteCount);
}

bool BigInteger::testBit(int i) const {
	int whichByte = i >> 3;
	int whichBit = i - (whichByte << 3);
	byte mask = 1 << whichBit;
	return data[maxByte - whichByte] & mask;
}

void BigInteger::set(bool x, int i) {
	if(x)
		setBit(i);
	else
		clearBit(i);
}

void BigInteger::setBit(int i) {
	int whichByte = i >> 3;
	int whichBit = i - (whichByte << 3);
	byte mask = 1 << whichBit;
	data[maxByte - whichByte] |= mask;
}

void BigInteger::clearBit(int i) {
	int whichByte = i >> 3;
	int whichBit = i - (whichByte << 3);
	byte mask = 1 << whichBit;
	data[maxByte - whichByte] &= ~mask;
}

void BigInteger::clear() {
	memset(data, 0, byteCount);
}

int BigInteger::getLowestMovable() const {
	for(int i = 0; i < bitCount; i++)
		if(testBit(i) && !testBit(i + 1))
			return i;
	return bitCount;
}

void BigInteger::buildMagnitudeLut() {
	if(magnitudeLut == NULL) {
		magnitudeLut = new byte[256];
		for(int i = 0; i < 256; i++) {
			byte cur = i;
			byte count;
			for (count = 0; cur; count++)
				cur &= cur - 1;
			magnitudeLut[i] = count;
		}
	}
}

int BigInteger::getMagnitude() const {
	buildMagnitudeLut();
	int magnitude = 0;
	for(int i = 0; i < byteCount; i++)
		magnitude += magnitudeLut[data[i]];
	return magnitude;
}

int BigInteger::intValue() const {
	int out;
	// test if this can be done with a single memcpy
	byte* x = (byte*) &out;
	x[0] = data[maxByte];
	x[1] = data[maxByte - 1];
	x[2] = data[maxByte - 2];
	x[3] = data[maxByte - 3];
	return out;
}

int BigInteger::size() const {
	return bitCount;
}

void BigInteger::binaryIncrement() {
	data[maxByte]++;
	for(int i = 0; i < byteCount; i++) {
		if(data[maxByte - i] == 0)
			data[maxByte - i - 1]++;
		else
			break;
	}
}

void BigInteger::chordIncrement() {
	int low = getLowestMovable();
	if(low + 1 < bitCount) {
		clearBit(low);
		setBit(low + 1);
		int lower = 0;
		for(int i = 0; i < low; i++) {
			if(testBit(i)) {
				clearBit(i);
				setBit(lower++);
			}
		}
	} else {
		int total = getMagnitude() + 1;
		clear();
		for(int i = 0; i< total; i++)
			setBit(i);
	}
}

void BigInteger::convertToGray() {
	byte original;
	for(int i = maxByte; i > 0; i--) { // low to high
		original = data[i]; // copy original value
		data[i] >>= 1; // right shift
		if(data[i - 1] & 0x01) // if the next bit up is available
			data[i] |= 0x80; // shift it over
		data[i] ^= original; // xor against original
	}
	data[0] = (data[0] >> 1) ^ data[0]; // last byte by itself
}

void BigInteger::reverse() {
	for(int i = 0; i < bitCount / 2; i++) {
		bool low = testBit(i);
		int high = maxBit - i;
		set((bool) testBit(high), i);
		set((bool) low, high);
	}
}

// only copies true bits, so shuffled should be cleared first
void BigInteger::shuffleInto(BigInteger& shuffled) {
	buildShuffleLut();
	for(int i = 0; i < bitCount; i++)
		if(testBit(i))
			shuffled.setBit(shuffleLut[i]);
}

void BigInteger::buildShuffleLut() {
	if(shuffleLut == NULL) {
		shuffleLut = new unsigned int[bitCount];
/*
	#define INDEX 10 // this controls the resolution of shuffle()

		BigInteger cur;
		cur.setup(INDEX);
		for(int i = 0; i < bitCount; i++) {
			cur.set(i);
			cur.reverse();
			shuffleLut[i] = cur.intValue();
			cout << i << ": " << (int) shuffleLut[i] << endl;
		}
*/

		int skipAmount = SKIP_AMOUNT;
		bool* swapped = new bool[bitCount];
		memset(swapped, 0, bitCount);
		for(int lowest = 0; lowest < bitCount; lowest++) {
			if(!swapped[lowest]) {
				int start = lowest;
				int skip = 1;
				while(start < bitCount) {
					skip += skipAmount;
					int end = start + skip;
					while(end < bitCount && swapped[end]) {
						skip += skip; // skipAmount
						end += skip; // skipAmount
					}
					if(end < bitCount) {
						swapped[start] = true;
						shuffleLut[start] = end;
						swapped[end] = true;
						shuffleLut[end] = start;
					}
					start = end;
					while(start < bitCount && swapped[start])
						start++;
				}
			}
		}
		delete [] swapped;

		for(int i = 0; i < bitCount; i++)
			if(!swapped[i])
				shuffleLut[i] = i;
	}
}

BigInteger& BigInteger::shiftRight() {
	data[maxByte] >>= 1;
	for(int i = 1; i < byteCount; i++) {
		if(testBit(i << 3))
			setBit((i << 3) - 1);
		data[maxByte - i] >>= 1;
	}
	return *this;
}

BigInteger& BigInteger::operator|=(const BigInteger& x) {
	int maxByteCount = byteCount > x.byteCount ? byteCount : x.byteCount;
	for(int i = 0; i < maxByteCount; i++)
		data[i] |= x.data[i];
	return *this;
}

BigInteger& BigInteger::operator&=(const BigInteger& x) {
	int maxByteCount = byteCount > x.byteCount ? byteCount : x.byteCount;
	for(int i = 0; i < maxByteCount; i++)
		data[i] &= x.data[i];
	return *this;
}

BigInteger& BigInteger::operator^=(const BigInteger& x) {
	int maxByteCount = byteCount > x.byteCount ? byteCount : x.byteCount;
	for(int i = 0; i < maxByteCount; i++)
		data[i] ^= x.data[i];
	return *this;
}

BigInteger::~BigInteger() {
	if(data != NULL)
		delete [] data;
	if(shuffleLut != NULL)
		delete [] shuffleLut;
}

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
