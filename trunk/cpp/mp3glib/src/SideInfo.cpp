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

	int offset = 0;
	memset(bits, 0, sideInfoLength);
	offset += 9; // 9 main_data_begin: negative offset, 0 begins after side info
	offset += 5; // 5 private_bits (padding): all 0 (only 3 for stereo)

	/*
		4 scfsi (scale factor sharing information)
			are scalefactors transferred for both granules, or not?
			if short windows (block_type = 10) all scalefactors are sent
		0 0-5
		1 6-10
		2 11-15
		3 16-20
	*/
	// TODO: add scfsi
	offset += 4;

	frame->getGranule(0).writeInfo(bits, offset);
	frame->getGranule(1).writeInfo(bits, offset);

	out.write((char*) bits, 17);
	return out;
}
