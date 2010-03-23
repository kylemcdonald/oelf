#include "Tables.h"

#include "Granule.h"

void Tables::buildLookup() {
	// build end tables for sfbands
	int band = 0;
	for(int i = 0; i < BANDS; i++) {
		band += bandwidth[i];
		sfend[i] = band;
	}

	band = 0;
	for(int i = 0; i < SHORT_BANDS; i++) {
		band += bandwidthShort[i];
		sfendShort[i] = band;
	}

	// build lookup table for 3^4=81 small values in table a+b
	int i = 0;
	for(int a = -1; a <= +1; a++) {
		for(int b = -1; b <= +1; b++) {
			for(int c = -1; c <= +1; c++) {
				for(int d = -1; d <= +1; d++) {
					int vals[4] = {a, b, c, d};
					byte quad = 0;
					int quadPosition = 0;
					byte signs = 0;
					int signsPosition = 0;

					// build the 4-bit index and signs
					for(int j = 0; j < 4; j++) {
						setBool(&quad, quadPosition, vals[j] != 0);
						if(vals[j] != 0)
							setBool(&signs, signsPosition, vals[j] == -1);
					}

					// right align both
					quad >>= 4;
					signs >>= 8 - signsPosition;

					byte htabAValue = htabA[quad][0];
					int htabALength = htabA[quad][1] + signsPosition;
					byte htabBValue = (~quad) & 0x0f;
					int htabBLength = 4 + signsPosition;

					short* sla = Tables::smallLookupA[i];
					sla[0] = htabAValue;
					sla[0] <<= signsPosition;
					sla[0] |= signs;
					sla[1] = htabALength;

					short* slb = Tables::smallLookupB[i];
					slb[0] = htabBValue;
					slb[0] <<= signsPosition;
					slb[0] |= signs;
					slb[1] = htabBLength;

					i++;
				}
			}
		}
	}
}

const byte Tables::slength[16][2] = {
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

const int Tables::bandwidth[BANDS] = {
	4, 4, 4, 4, 4, 4, 6, 6, 8, 8, 10, 12, 16, 20, 24, 28, 34, 42, 50, 54, 76
};

const int Tables::bandwidthShort[SHORT_BANDS] = {
	4, 4, 4, 4, 6, 8, 10, 12, 14, 18, 22, 30
};

const byte Tables::htabA[16][2] = {
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

short Tables::smallLookupA[81][2];
short Tables::smallLookupB[81][2];
int Tables::sfend[BANDS];
int Tables::sfendShort[SHORT_BANDS];
