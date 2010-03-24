#pragma once
#include "Shared.h"
#include "Tables.h"

class Frame;

class Granule {
public:
	Granule();

	void setBlockType(byte blockType, bool mixedBlock = false);
	void setFrame(Frame* frame);

	void writeSideInfo(byte* data, int& position) const;
	void writeMainData(byte* data, int& position) const;
	short getMainDataLength() const;
	bool getWindowSwitching() const;
	bool isShort() const;
	bool isFirst() const;
	short getBigValues() const;

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

	byte regionCount[REGIONS]; // how many bands
	byte bigTableSelect[REGIONS];
	char bigCodes[FREQUENCIES]; // in the case of SHORT, these are interleaved

	short smallValues; // how many quadruples
	bool smallTableSelect;
	byte smallCodes[FREQUENCIES / 4];
};
