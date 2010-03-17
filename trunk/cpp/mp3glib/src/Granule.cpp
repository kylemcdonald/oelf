#include "Granule.h"

Granule::Granule() {
}

ostream& Granule::writeInfo(ostream& out) {
	byte bits[granuleLength];
	memset(bits, 0, granuleLength);
	// first, fill out the bits aligned to 0
	/*
		12 part2_3_length
		9 big_values
		8 global_gain
		4 scalefac_compress
		1 windows_switching_flag / blocksplit_flag
		if(blocksplit_flag) {
			2 block_type
			1 switch_point / mixed_blockflag
			5 region0 table_select
			5 region1 table_select
			3 window0 subblock_gain
			3 window1 subblock_gain
			3 window2 subblock_gain
		} else {
			5 region0 table_select
			5 region1 table_select
			5 region2 table_select
			4 region_address1 / region0_count
			3 region_address2 / region1_count
		} // 22
		1 preflag
		1 scalefac_scale
		1 count1table_select
	*/ // 59
	// then copy to unaligned input pointer
	return out;
}
