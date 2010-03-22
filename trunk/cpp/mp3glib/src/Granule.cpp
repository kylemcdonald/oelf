#include "Granule.h"

#include "Frame.h"
#include "SideInfo.h"

short Granule::smallLookupA[81][2];
short Granule::smallLookupB[81][2];

Granule::Granule() :
		bigValues(0),
		smallValues(0),
		globalGain(150),
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
	short total = 0;

	const bool* scfsi = frame->sideInfo.getScfsi();
	byte slenLow = slength[slindex][0];
	byte slenHigh = slength[slindex][1];
	if(isFirst() || frame->hasShort()) { // without sharing
		if(isShort()) {
			if(mixedBlock) {
				total += slenLow * 8; // long subblock
				total += SUBBLOCKS * slenLow * 3; // short-low subblocks
				total += SUBBLOCKS * slenHigh * 6; // short-high subblocks
			} else { // pure short
				total += SUBBLOCKS * slenLow * 6;
				total += SUBBLOCKS * slenHigh * 6;
			}
		} else { // long, start, stop
			total += slenLow * 11;
			total += slenHigh * 10;
		}
	} else { // with sharing (long, start, stop)
		if(!scfsi[0])
			total += slenLow * 6;
		if(!scfsi[1])
			total += slenLow * 5;
		if(!scfsi[2])
			total += slenHigh * 5;
		if(!scfsi[3])
			total += slenHigh * 5;
	}
	return 0;
}

bool Granule::getWindowSwitching() const {
	return blockType != LONG_BLOCK;
}

bool Granule::isShort() const {
	return blockType == SHORT_BLOCK;
}

bool Granule::isFirst() const {
	return this == &(frame->granules[0]);
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
	const bool* scfsi = frame->sideInfo.getScfsi();

	byte slenLow = slength[slindex][0];
	byte slenHigh = slength[slindex][1];
	if(isFirst() || frame->hasShort()) { // without sharing
		if(isShort()) {
			if(mixedBlock) { // mixed short
				for(int i = 0; i < 8; i++)
					setByte(data, position, sfi[i], slenLow);
				for(int i = 3; i < 6; i++)
					for(int s = 0; s < SUBBLOCKS; s++)
						setByte(data, position, sfiShort[s][i], slenLow);
				for(int i = 6; i < 12; i++)
					for(int s = 0; s < SUBBLOCKS; s++)
						setByte(data, position, sfiShort[s][i], slenHigh);
			} else { // pure short
				for(int i = 0; i < 6; i++)
					for(int s = 0; s < SUBBLOCKS; s++)
						setByte(data, position, sfiShort[s][i], slenLow);
				for(int i = 6; i < 12; i++)
					for(int s = 0; s < SUBBLOCKS; s++)
						setByte(data, position, sfiShort[s][i], slenHigh);
			}
		} else { // long, start, stop
			for(int i = 0; i < 11; i++)
				setByte(data, position, sfi[i], slenLow);
			for(int i = 11; i < 21; i++)
				setByte(data, position, sfi[i], slenHigh);
		}
	} else { // with sharing (long, start, stop)
		if(!scfsi[0])
			for(int i = 0; i < 6; i++)
				setByte(data, position, sfi[i], slenLow);
		if(!scfsi[1])
			for(int i = 6; i < 11; i++)
				setByte(data, position, sfi[i], slenLow);
		if(!scfsi[2])
			for(int i = 11; i < 16; i++)
				setByte(data, position, sfi[i], slenHigh);
		if(!scfsi[3])
			for(int i = 16; i < 21; i++)
				setByte(data, position, sfi[i], slenHigh);
	}

	for(int i = 0; i < REGIONS; i++) {
		int table = bigTableSelect[i];
		if(table == 0) {
			// output zeros
		} else if(table <= 12) {
			// output big
		} else {
			// output very big
		}
	}

	if(smallTableSelect) {
		for(int i = 0; i < smallValues; i++) {
			//byte* cur = smallLookupB[smallCodes[i]];
			//setByte(data, position, cur[0], cur[1]);
		}
	} else {
		// small values with table a
		// ditto as small table b, but use huffman codes instead
		// a bunch of huffman coded quadruples followed by up to 4 bit of signs
	}
}
