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

void BigInteger::write(ostream& out) const {
	out.write((char*) data, byteCount);
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

int BigInteger::getMagnitude() const {
	// this can be sped up with a bytewise LUT
	int magnitude = 0;
	for(int i = 0; i < bitCount; i++)
		if(testBit(i))
			magnitude++;
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
	BigInteger position;
	// this can be sped up by precomputing a LUT
	position.setup(INDEX);
	for(int i = 0; i < bitCount; i++) {
		if(testBit(i)) {
			position.set(i);
			position.reverse();
			shuffled.setBit(position.intValue());
		}
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
	for(int i = 0; i < byteCount; i++)
		data[i] |= x.data[i];
	return *this;
}

BigInteger& BigInteger::operator&=(const BigInteger& x) {
	for(int i = 0; i < byteCount; i++)
		data[i] &= x.data[i];
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
