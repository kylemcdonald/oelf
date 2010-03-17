#include "SideInfo.h"

SideInfo::SideInfo() :
	frame(NULL) {
}

void SideInfo::setFrame(Frame* frame) {
	this->frame = frame;
}

ostream& SideInfo::write(ostream& out) {
	byte bits[sideInfoLength];
	memset(bits, 0, sideInfoLength);

	/*
		9 main_data_begin: "0" means main data begins directly after side info
		5 private_bits: all 0, not used
	*/

	/*
		starting at position 11:
		4 scfsi: are scalefactors transferred for both granules, or not?
			if short windows (block_type = 10) all scalefactors are sent

		granule 0 info
		granule 1 info
	*/
	out.write((char*) bits, 17);
	return out;
}
