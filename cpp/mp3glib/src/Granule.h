#pragma once
#include "Shared.h"

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

	static void buildLookup();
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

	static const byte slength[][2];

	byte bigCodes[FREQUENCIES];
	byte smallCodes[FREQUENCIES / 4];

	static byte smallLookupB[][2];
};
