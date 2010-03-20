#include "Granule.h"

Granule::Granule() :
		bigValues(0),
		smallValues(0),
		globalGain(0),
		slength(0),
		blockType(LONG_BLOCK),
		mixedBlock(false),
		windowSwitching(false),
		smallTableSelect(false),
		preflag(false),
		scaleShift(false) {
	for(int i = 0; i < REGIONS; i++) {
		bigTableSelect[i] = 0;
		regionCount[i] = 0;
		subblockGain[i] = 0;
	}
}

int Granule::writeSideInfo(byte* data, int offset) const {
	// part2_3_length / main_data_bit is computed
	// based on scale factor length + big value length + small value length
	offset += 12;

	// bigValues; // where big values end and small values begin
	offset += 9;

	// globalGain; // global_gain
	offset += 8;

	// slength; // scalefac_compress / bit allocation for scale factors
	offset += 4;

	// windowSwitching is true only when blockType is long
	set(data, offset++, windowSwitching);

	if(windowSwitching) { // start, stop, short (special)
		// blockType; // either start, stop, or short
		offset += 2;

		// switch_point / mixed_blockflag
		set(data, offset++, mixedBlock);

		for(int i = 0; i < REGIONS - 1; i++) {
			// bigTableSelect[i]; / bigtable huffman table selection
			offset += 5;
		}
		for(int i = 0; i < REGIONS; i++) { // is this WINDOWS or REGIONS?
			// subblockGain[i]; // window i subblock_gain
			offset += 3;
		}
	} else { // long (normal)
		for(int i = 0; i < REGIONS; i++) {
			// bigTableSelect[i]; / bigtable huffman table selection
			offset += 5;
		}
		//regionCount[0] - 1; // region 0 bands
		offset += 4;
		//regionCount[1] - 1; // region 1 bands
		offset += 3;
	}

	set(data, offset++, preflag); // preemphasis flag
	set(data, offset++, scaleShift); // sf quantize, scalefac_scale, scale_shift
	set(data, offset++, smallTableSelect); // small values huffman table, count1table_select

	return offset;
}

int Granule::writeMainData(byte* data, int offset) const {
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
	return offset;
}
