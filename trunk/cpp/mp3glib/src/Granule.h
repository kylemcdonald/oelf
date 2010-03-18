#pragma once
#include "Shared.h"

class Granule {
public:
	Granule();
	int writeInfo(byte* data, int offset) const;
private:
	bool windowSwitching;
	byte globalGain;
	byte blockType;
	bool mixedBlock;
	byte bigTableSelect[3];
	bool smallTableSelect;
	bool preflag;
	bool scaleShift;
	byte regionCount[3];
	byte subblockGain[3];
};
