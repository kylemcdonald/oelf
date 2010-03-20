#include "SideInfo.h"

#include "Frame.h"

SideInfo::SideInfo() :
	frame(NULL) {
}

void SideInfo::setFrame(Frame* frame) {
	this->frame = frame;
}

ostream& SideInfo::writeSideInfo(ostream& out) const {
	byte data[sideInfoLength];

	int position = 0;
	memset(data, 0, sideInfoLength);
	position += 9; // main_data_begin: negative position, 0 begins after side info
	position += 5; // private_data, all 0

	/*
		4 scfsi (scale factor sharing information)
			are scalefactors transferred for both granules, or not?
			if short windows (block_type = 10) all scalefactors are sent
		0 0-5
		1 6-10
		2 11-15
		3 16-20
	*/
	for(int i = 0; i < 4; i++)
		setBool(data, position, scfsi[i]);

	frame->getGranule(0).writeSideInfo(data, position);
	frame->getGranule(1).writeSideInfo(data, position);

	cout << "done with granules at " << position << endl;

	out.write((char*) data, 17);
	return out;
}
