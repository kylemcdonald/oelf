#pragma once

#include <string>
#include <iostream>
typedef unsigned char byte;
using namespace std;

#define INDEX 10 // this controls the resolution of shuffle()

/*
	Bits are internally stored in a big-endian format, as individual bytes are
	big-endian. This means a BigInteger can be used to manage a bitstream.

	The advantage of little-endian storage would be that arbitrary datatypes
	can be quickly set and retrieved using mempcy.
*/

class BigInteger {
public:
	BigInteger();
	void setup(const BigInteger& from);
	void setup(int bitCount);

	void set(const BigInteger& from);
	void set(byte x);
	void set(short x);
	void set(int x);
	void set(const byte* x);
	void set(bool x, int i = 0);
	void setBit(int i);
	void clearBit(int i);
	void clear();

	bool testBit(int i) const;
	int getLowestMovable() const;
	int getMagnitude() const;
	int intValue() const;
	int size() const;
	byte* getData();
	string toString() const;
	void write(ostream& out, int byteCount) const;

	void binaryIncrement();
	void chordIncrement();

	void reverse();
	void shuffleInto(BigInteger& shuffled);
	BigInteger& shiftRight();
	BigInteger& operator|=(const BigInteger& x);
	BigInteger& operator&=(const BigInteger& x);
	BigInteger& operator^=(const BigInteger& x);

	~BigInteger();
private:
	void buildShuffleLut();
	static void buildMagnitudeLut();

	int bitCount, byteCount;
	int maxBit, maxByte;
	byte* data;

	int* shuffleLut;
	static byte* magnitudeLut;
};
