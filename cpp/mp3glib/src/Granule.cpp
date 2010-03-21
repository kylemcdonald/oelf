#include "Granule.h"

#include "Frame.h"
#include "SideInfo.h"

const byte Granule::slength[16][2] = {
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


Granule::Granule() :
		bigValues(0),
		smallValues(0),
		globalGain(0),
		slindex(0),
		blockType(LONG_BLOCK),
		mixedBlock(false),
		smallTableSelect(false),
		preflag(false),
		scaleShift(false) {
	for(int i = 0; i < REGIONS; i++) {
		bigTableSelect[i] = 0;
		regionCount[i] = 0;
	}
	for(int i = 0; i < SUBBLOCKS; i++)
		subblockGain[i] = 0;
}

void Granule::setFrame(Frame* frame) {
	this->frame = frame;
}

short Granule::getMainDataLength() const {
	// based on scale factor length + big value length + small value length
	return 0;
}

bool Granule::getWindowSwitching() const {
	return blockType != LONG_BLOCK;
}

bool Granule::isShort() const {
	return blockType == SHORT_BLOCK;
}

void Granule::writeSideInfo(byte* data, int& position) const {
	setShort(data, position, getMainDataLength(), 12);
	setShort(data, position, bigValues, 9);
	setByte(data, position, globalGain, 8);
	setByte(data, position, slindex, 4);

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
	bool first = this == &(frame->getGranule(0));
	const bool* scfsi = frame->getSideInfo().getScfsi();

	if(first || frame->hasShort()) { // no sharing

	} else { // use sharing
		byte slen = slength[slindex][0];
		if(!scfsi[0])
			for(int i = 0; i < 6; i++)
				setByte(data, position, sfi[i], slen);
		if(!scfsi[1])
			for(int i = 6; i < 11; i++)
				setByte(data, position, sfi[i], slen);
		slen = slength[slindex][1];
		if(!scfsi[2])
			for(int i = 11; i < 16; i++)
				setByte(data, position, sfi[i], slen);
		if(!scfsi[3])
			for(int i = 16; i < 21; i++)
				setByte(data, position, sfi[i], slen);
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
