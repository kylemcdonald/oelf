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

	static byte smallLookupA[81][2];
	static byte smallLookupB[81][2];

private:
	Frame* frame;

	short bigValues;
	short smallValues; // how many quadruples
	byte globalGain;
	byte slindex;

	byte blockType;
	bool mixedBlock;

	byte bigTableSelect[REGIONS];
	bool smallTableSelect;

	byte regionCount[REGIONS];
	byte subblockGain[REGIONS];

	bool preflag;
	bool scaleShift;

	byte sfi[BANDS];
	byte sfiShort[SUBBLOCKS][SHORT_BANDS];

	byte bigCodes[FREQUENCIES];
	byte smallCodes[FREQUENCIES / 4];
};
