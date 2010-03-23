#pragma once
#include "Shared.h"
#include "Tables.h"

class Frame;

class Granule {
public:
	Granule();
	void setFrame(Frame* frame);
	void writeSideInfo(byte* data, int& position) const;
	void writeMainData(byte* data, int& position) const;
	short getMainDataLength() const;
	bool getWindowSwitching() const;
	bool isShort() const;
	bool isFirst() const;

	static short smallLookupA[81][2];
	static short smallLookupB[81][2];

	Frame* frame;

	byte globalGain;
	byte slindex;

	byte blockType;
	bool mixedBlock;
	byte subblockGain[SUBBLOCKS];
	byte sfiShort[SUBBLOCKS][SHORT_BANDS];

	bool scaleShift;
	byte sfi[BANDS];

	bool preflag;

	short bigValues;
	byte bigTableSelect[REGIONS];
	byte bigCodes[REGIONS][FREQUENCIES];
	byte regionCount[REGIONS];

	short smallValues; // how many quadruples
	bool smallTableSelect;
	byte smallCodes[FREQUENCIES / 4];
};
