#include "SideInfo.h"

#include "Frame.h"

SideInfo::SideInfo() :
	frame(NULL) {
}

void SideInfo::setFrame(Frame* frame) {
	this->frame = frame;
}

ostream& SideInfo::write(ostream& out) const {
	byte bits[sideInfoLength];
	memset(bits, 0, sideInfoLength);

	int offset = 0;
	// 9 main_data_begin: "0" means main data begins directly after side info
	offset += 9;
	// 5 private_bits: all 0, not used
	offset += 5;

	/*
		4 scfsi: are scalefactors transferred for both granules, or not?
			if short windows (block_type = 10) all scalefactors are sent
	*/
	// TODO: add scfsi check
	offset += 4;

	frame->getGranule(0).writeInfo(bits, offset);
	frame->getGranule(1).writeInfo(bits, offset);

	out.write((char*) bits, 17);
	return out;
}
