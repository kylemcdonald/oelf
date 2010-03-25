#pragma once

#include <string>
typedef unsigned char byte;
using namespace std;

class BigInteger {
public:
	BigInteger();
	void setup(const BigInteger& from);
	void setup(int bitCount);

	void set(const BigInteger& from);
	void set(byte x);
	void set(short x);
	void set(int x);
	void setBit(int i);
	void clearBit(int i);
	void clear();

	bool testBit(int i) const;
	int getLowestMovable() const;
	int getMagnitude() const;
	string toString() const;

	void binaryIncrement();
	void chordIncrement();

	BigInteger& shiftRight();
	BigInteger& operator^=(const BigInteger& x);
	// shuffle

	~BigInteger();
private:
	int bitCount, byteCount, max;
	byte* data;
};
