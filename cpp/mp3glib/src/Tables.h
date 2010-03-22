#pragma once
#include "Shared.h"

const byte slength[16][2] = {
	{0, 0},
	{0, 1},
	{0, 2},
	{0, 3},
	{3, 0},
	{1, 1},
	{1, 2},
	{1, 3},
	{2, 1},
	{2, 2},
	{2, 3},
	{3, 1},
	{3, 2},
	{3, 3},
	{4, 2},
	{4, 3}
};

const byte htabA[16][2] = {
	{1, 1}, // 1
	{5, 4}, // 0101
	{4, 4}, // 0100
	{5, 5}, // 00101
	{6, 4}, // 0110
	{5, 6}, // 000101
	{4, 5}, // 00100
	{4, 6}, // 000100
	{7, 4}, // 0111
	{3, 5}, // 00011
	{6, 5}, // 00110
	{0, 6}, // 000000
	{7, 5}, // 00111
	{2, 6}, // 000010
	{3, 6}, // 000011
	{1, 6}  // 000001
};

void buildLookup();
