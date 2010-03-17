#include "Granule.h"

Granule::Granule() {
}

int Granule::writeInfo(byte* data, int offset) const {
	// 0-11 part2_3_length
	offset += 12; // part2_3_length / main_data_bit
	offset += 9; // big_values
	offset += 8; // global_gain
	offset += 4; // slength / scalefac_compress / bit allocation for scale factors

	set(data, offset, windowSwitching);
	offset++;
	if(windowSwitching) { // start, stop, short
		/*
			2 block_type
			1 switch_point / mixed_blockflag
			5 region0 table_select
			5 region1 table_select
			3 window0 subblock_gain
			3 window1 subblock_gain
			3 window2 subblock_gain
		*/
	} else {
		/*
			5 region0 table_select
			5 region1 table_select
			5 region2 table_select
			4 region_address1 / region0_count
			3 region_address2 / region1_count
		*/
	}
	/*
		1 preflag
		1 scalefac_scale
		1 count1table_select
	*/
	return offset;
}
