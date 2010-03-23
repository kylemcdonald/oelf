#pragma once

#include "Shared.h"

class Huffman {
public:
	static const byte bigLookup1[4][2];
	static const byte bigLookup2[9][2];
	static const byte bigLookup3[9][2];
	static const byte bigLookup5[16][2];
	static const byte bigLookup6[16][2];
	static const byte bigLookup7[36][2];
	static const byte bigLookup8[36][2];
	static const byte bigLookup9[36][2];
	static const byte bigLookup10[64][2];
	static const byte bigLookup11[64][2];
	static const byte bigLookup12[64][2];
	static const byte bigLookup13[256][2];
	static const byte bigLookup15[256][2];
	static const byte* bigLookup(int table, char x, char y);
};
