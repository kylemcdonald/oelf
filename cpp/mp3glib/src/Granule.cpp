#include "Granule.h"

Granule::Granule() :
		bigValues(0),
		smallValues(0),
		globalGain(0),
		slength(0),
		blockType(LONG_BLOCK),
		mixedBlock(false),
		smallTableSelect(false),
		preflag(false),
		scaleShift(false) {
	for(int i = 0; i < REGIONS; i++) {
		bigTableSelect[i] = 0;
		regionCount[i] = 0;
		subblockGain[i] = 0;
	}
}

short Granule::getMainDataLength() const {
	// based on scale factor length + big value length + small value length
	return 0;
}

bool Granule::getWindowSwitching() const {
	return blockType != LONG_BLOCK;
}

void Granule::writeSideInfo(byte* data, int& position) const {
	setShort(data, position, getMainDataLength(), 12);
	setShort(data, position, bigValues, 9);
	setByte(data, position, globalGain, 8);
	position += 4; // bit allocation for scale factors (slength/slimit)


	bool windowSwitching = getWindowSwitching();
	setBool(data, position, windowSwitching);
	if(windowSwitching) {
		setByte(data, position, blockType, 2);

		setBool(data, position, mixedBlock);

		for(int i = 0; i < REGIONS - 1; i++)
			setByte(data, position, bigTableSelect[i], 5);
		for(int i = 0; i < SUBBLOCKS; i++)
			setByte(data, position, subblockGain[i], 3);
	} else {
		for(int i = 0; i < REGIONS; i++)
			setByte(data, position, bigTableSelect[i], 5);
		setByte(data, position, regionCount[0], 4);
		setByte(data, position, regionCount[1], 3);
	}

	setBool(data, position, preflag);;
	setBool(data, position, scaleShift);
	setBool(data, position, smallTableSelect);
}

void Granule::writeMainData(byte* data, int& position) const {
	for(int i = 0; i < BANDS; i++) {
		// use slength and sideInfo->scfsi to determine
		// which bands and how long they are
	}
	/*
	for(int i = 0; i < bigValues; i++) {
		// all of the big value codes
	}
	for(int i = 0; i < smallValues; i++) {
		// all of the small value codes
	}
	*/
}
