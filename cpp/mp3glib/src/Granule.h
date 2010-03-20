#pragma once
#include "Shared.h"

class Granule {
public:
	Granule();
	void writeSideInfo(byte* data, int& position) const;
	void writeMainData(byte* data, int& position) const;
	short getMainDataLength() const;
	bool getWindowSwitching() const;
private:
	short bigValues;
	short smallValues;
	byte globalGain;
	byte slength;

	byte blockType;
	bool mixedBlock;

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
