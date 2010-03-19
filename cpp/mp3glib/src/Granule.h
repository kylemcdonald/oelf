#pragma once
#include "Shared.h"

class Granule {
public:
	Granule();
	int writeSideInfo(byte* data, int offset) const;
	int writeMainData(byte* data, int offset) const;
private:
	short bigValues;
	short smallValues;
	byte globalGain;
	byte slength;

	byte blockType;
	bool mixedBlock;

	bool windowSwitching;

	byte bigTableSelect[REGIONS];
	bool smallTableSelect;

	byte regionCount[REGIONS];
	byte subblockGain[REGIONS];

	bool preflag;
	bool scaleShift;

	byte sfi[BANDS];

	// bigCodes[FREQUENCIES / 2];
	// smallCodes[FREQUENCIES / 2];
};
