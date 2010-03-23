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
	memset(bigTableSelect, 0, REGIONS);
	memset(regionCount, 0, REGIONS);
	memset(subblockGain, 0, SUBBLOCKS);
	memset(sfi, 0, BANDS);
	memset(sfiShort, 0, SUBBLOCKS * SHORT_BANDS * sizeof(short));
}

void Granule::setFrame(Frame* frame) {
	this->frame = frame;
}

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

	return total;
}

short Granule::getBigValues() const {
	// this value is actually based on the sum of the regionCounts
	// but drawn from the sfEnd table
	// and then divided by two (i think)
	return regionCount[0] + regionCount[1] + regionCount[2];
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
	setShort(data, position, getBigValues(), 9);
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

	setBool(data, position, preflag);;
	setBool(data, position, scaleShift);
	setBool(data, position, smallTableSelect);
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

	for(int i = 0; i < REGIONS; i++) {
		int table = bigTableSelect[i];
		if(table > 12) {
			// output very big
		} else if(table > 0) {
			int cur = 0;
			int end = blockType == SHORT_BLOCK ?
				Tables::sfendShort[regionCount[i]] :
				Tables::sfend[regionCount[i]];
			while(cur < end) {
				char a = bigCodes[i][cur++];
				char b = bigCodes[i][cur++];
				const byte* h = Huffman::bigLookup(table, a, b);
				setByte(data, position, h[0], h[1]);
				if(a != 0)
					setBool(data, position, a < 0);
				if(b != 0)
					setBool(data, position, b < 0);
			}
		}
	}

	// write small values in quadruples using lookup tables
	short (*smallLookup)[2] = smallTableSelect ? Tables::smallLookupB : Tables::smallLookupA;
	for(int i = 0; i < smallValues; i++) {
		short* cur = smallLookup[smallCodes[i]];
		setShort(data, position, cur[0], cur[1]);
	}
}
