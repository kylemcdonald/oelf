#pragma once
#include "Shared.h"
#include "Huffman.h"

string binary(short x, int length);
string binary(byte x, int length);

class Tables {
public:
	static void buildLookup();

	static const byte slength[16][2];
	static short smallLookupA[81][2];
	static short smallLookupB[81][2];
	static int sfend[BANDS];
	static int sfendShort[SHORT_BANDS];

private:
	static const int bandwidth[BANDS];
	static const int bandwidthShort[SHORT_BANDS];
	static const byte htabA[16][2];
};
