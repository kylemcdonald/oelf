#include "BigInteger.h"

BigInteger::BigInteger() :
	bitCount(0),
	byteCount(0),
	data(NULL) {
}

void BigInteger::setup(const BigInteger& from) {
	setup(from.bitCount);
	set(from);
}

void BigInteger::setup(int bitCount) {
	this->bitCount = bitCount;
	this->byteCount = (bitCount >> 3) + (bitCount ? 1 : 0);
	this->max = byteCount - 1;
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

void BigInteger::set(byte x) {
	data[max] = x;
}

void BigInteger::set(short x) {
	data[max] = ((byte*) &x)[0];
	data[max - 1] = ((byte*) &x)[1];
}

void BigInteger::set(int x) {
	data[max] = ((byte*) &x)[0];
	data[max - 1] = ((byte*) &x)[1];
	data[max - 2] = ((byte*) &x)[2];
	data[max - 3] = ((byte*) &x)[3];
}

bool BigInteger::testBit(int i) const {
	int whichByte = i >> 3;
	int whichBit = i - (whichByte << 3);
	byte mask = 1 << whichBit;
	return data[max - whichByte] & mask;
}

void BigInteger::setBit(int i) {
	int whichByte = i >> 3;
	int whichBit = i - (whichByte << 3);
	byte mask = 1 << whichBit;
	data[max - whichByte] |= mask;
}

void BigInteger::clearBit(int i) {
	int whichByte = i >> 3;
	int whichBit = i - (whichByte << 3);
	byte mask = 1 << whichBit;
	data[max - whichByte] &= ~mask;
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

int BigInteger::getMagnitude() const {
	// this can be sped up with a bytewise lut
	int magnitude = 0;
	for(int i = 0; i < bitCount; i++)
		if(testBit(i))
			magnitude++;
	return magnitude;
}

void BigInteger::binaryIncrement() {
	data[max]++;
	for(int i = 0; i < byteCount; i++) {
		if(data[max - i] == 0)
			data[max - i - 1]++;
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

BigInteger& BigInteger::shiftRight() {
	data[max] >>= 1;
	for(int i = 1; i < byteCount; i++) {
		if(testBit(i << 3))
			setBit((i << 3) - 1);
		data[max - i] >>= 1;
	}
	return *this;
}

BigInteger& BigInteger::operator^=(const BigInteger& x) {
	for(int i = 0; i < byteCount; i++)
		data[i] ^= x.data[i];
	return *this;
}

BigInteger::~BigInteger() {
	if(data != NULL)
		delete [] data;
}
