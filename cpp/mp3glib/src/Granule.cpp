#include "Granule.h"

#include "Frame.h"
#include "SideInfo.h"

Granule::Granule() :
		frame(NULL),
		globalGain(150),
		slindex(0),
		blockType(LONG_BLOCK),
		mixedBlock(false),
		scaleShift(false),
		preflag(false),
		smallValues(0),
		smallTableSelect(false) {
	memset(subblockGain, 0, SUBBLOCKS);
	memset(sfi, 0, BANDS);
	memset(sfiShort, 0, SUBBLOCKS * SHORT_BANDS * sizeof(short));

	memset(regionCount, 0, REGIONS);
	memset(bigTableSelect, 0, REGIONS);
	memset(bigCodes, 0, FREQUENCIES);

	memset(smallCodes, 40, FREQUENCIES / 4);
}

void Granule::setBlockType(byte blockType, bool mixedBlock) {
	this->blockType = blockType;
	this->mixedBlock = mixedBlock;
	if(blockType != LONG_BLOCK) {
		if(blockType == SHORT_BLOCK && !mixedBlock)
			regionCount[0] = 9;
		else
			regionCount[0] = 8;
		regionCount[2] = 0;
	}
}

void Granule::setFrame(Frame* frame) {
	this->frame = frame;
}

// total bit length of the main data
short Granule::getMainDataLength() const {
	short total = 0;

	const bool* scfsi = frame->sideInfo.getScfsi();
	byte slenLow = Tables::slength[slindex][0];
	byte slenHigh = Tables::slength[slindex][1];
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

	int cur = 0;
	int regionTotal = 0;
	for(int i = 0; i < REGIONS; i++) {
		int table = bigTableSelect[i];
		regionTotal += regionCount[i];
		if(table > 0) {
			int end = blockType == SHORT_BLOCK ?
				Tables::sfendShort[regionTotal] :
				Tables::sfend[regionTotal];
			while(cur < end) {
				char a = bigCodes[cur++];
				char b = bigCodes[cur++];
				const byte* h = Huffman::bigLookup(table, a, b);
				total += h[1];
				if(a != 0)
					total++;
				if(b != 0)
					total++;
			}
		}
	}

	short (*smallLookup)[2] = smallTableSelect ? Tables::smallLookupB : Tables::smallLookupA;
	for(int i = 0; i < smallValues; i++) {
		short* cur = smallLookup[smallCodes[i]];
		total += cur[1];
	}

	return total;
}

short Granule::getBigValues() const {
	int totalCount = regionCount[0] + regionCount[1] + regionCount[2];
	if(blockType == SHORT_BLOCK) {
		return Tables::sfendShort[totalCount];
	} else {
		return Tables::sfend[totalCount];
	}
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
	setShort(data, position, getBigValues() / 2, 9); // big value pairs
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
		setByte(data, position, regionCount[0] - 1, 4);
		setByte(data, position, regionCount[1] - 1, 3);
	}

	setBool(data, position, preflag);
	setBool(data, position, scaleShift);
	setBool(data, position, smallTableSelect);
}

void Granule::writeSideInfoMask(byte* data, int& position) const {
	/*
	 3176 bits for side info
	 74*2 of those bits can be scale factor bands
	 3028 / 2 = 1514 main data length maximum
	 could get an error if not enough small values
	 or if too many big values
	 so values should be set indirectly and then other parameters generated
	 in order to maintain coherency
	*/

	setShort(data, position, 0x000, 12); // main data length is off limits
	setShort(data, position, 0x000, 9); // big value pairs are off limits
	setByte(data, position, 0xff, 8); // global gain is free (maybe limited?)
	setByte(data, position, 0xf, 4); // slindex is free
	setBool(data, position, false); // window switching is fixed (for now)

	// long windows
	for(int i = 0; i < REGIONS; i++)
		setByte(data, position, 0xff, 5); // big table selection is free
	setByte(data, position, 0xf, 4); // region count 0 is free
	setByte(data, position, 0xf, 3); // region count 1 is free

	setBool(data, position, true); // preflag is free
	setBool(data, position, true); // scale shift is free
	setBool(data, position, true); // small table select is free
}

void Granule::writeMainData(byte* data, int& position) const {
	const bool* scfsi = frame->sideInfo.getScfsi();

	byte slenLow = Tables::slength[slindex][0];
	byte slenHigh = Tables::slength[slindex][1];
	if(isFirst() || frame->hasShort()) { // without sharing
		if(isShort()) {
			int start = 0;
			if(mixedBlock) { // mixed short
				for(int i = 0; i < 8; i++)
					setByte(data, position, sfi[i], slenLow);
				start = 3;
			}
			for(int i = start; i < 6; i++)
				for(int s = 0; s < SUBBLOCKS; s++)
					setByte(data, position, sfiShort[s][i], slenLow);
			for(int i = 6; i < 12; i++)
				for(int s = 0; s < SUBBLOCKS; s++)
					setByte(data, position, sfiShort[s][i], slenHigh);
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

	int cur = 0;
	int regionTotal = 0;
	for(int i = 0; i < REGIONS; i++) {
		int table = bigTableSelect[i];
		regionTotal += regionCount[i];
		if(table > 0) {
			int end = blockType == SHORT_BLOCK ?
				Tables::sfendShort[regionTotal] :
				Tables::sfend[regionTotal];
			while(cur < end) {
				char a = bigCodes[cur++];
				char b = bigCodes[cur++];
				const byte* h = Huffman::bigLookup(table, a, b);
				setByte(data, position, h[0], h[1]);
				if(a != 0)
					setBool(data, position, a < 0);
				if(b != 0)
					setBool(data, position, b < 0);
			}
		}
	}

	short (*smallLookup)[2] = smallTableSelect ? Tables::smallLookupB : Tables::smallLookupA;
	for(int i = 0; i < smallValues; i++) {
		short* cur = smallLookup[smallCodes[i]];
		setShort(data, position, cur[0], cur[1]);
	}
}
