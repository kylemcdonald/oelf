#include "Granule.h"

Granule::Granule() :
	windowSwitching(false) {
}

int Granule::writeInfo(byte* data, int offset) const {
	// part2_3_length / main_data_bit is computed
	offset += 12;

	// bigValues
	offset += 9;

	// globalGain; // global_gain
	offset += 8;

	// slength / scalefac_compress / bit allocation for scale factors
	offset += 4;

	// windowSwitching is true only when blockType is long, so why store separate?
	set(data, offset, windowSwitching);
	offset++;
	if(windowSwitching) { // start, stop, short (special)
		// blockType; // either start, stop, or short
		offset += 2;

		// mixedBlock; // switch_point / mixed_blockflag
		offset += 1;

		for(int i = 0; i < 2; i++) {
			// bigTableSelect[i]; / bigtable huffman table selection
			offset += 5;
		}
		for(int i = 0; i < 3; i++) {
			// subblockGain[i]; // window i subblock_gain
			offset += 3;
		}
	} else { // long (normal)
		for(int i = 0; i < 3; i++) {
			// bigTableSelect[i]; / bigtable huffman table selection
			offset += 5;
		}
		//regionCount[0] - 1; // region 0 bands
		offset += 4;
		//regionCount[1] - 1; // region 1 bands
		offset += 3;
	}

	set(data, offset, preflag); // preemphasis flag
	offset++;

	set(data, offset, scaleShift); // sf quantize, scalefac_scale, scale_shift
	offset++;

	set(data, offset, smallTableSelect); // small values huffman table, count1table_select
	offset++;

	return offset;
}
