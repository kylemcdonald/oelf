#include "Shared.h"

void set(byte* array, unsigned int bit) {
	unsigned int offset = bit >> 3;
	unsigned int remain = bit - (offset << 3);
	array[offset] |= 1 << remain;
}

void clear(byte* array, unsigned int bit) {
	unsigned int offset = bit >> 3;
	unsigned int remain = bit - (offset << 3);
	array[offset] &= ~(1 << remain);
}
