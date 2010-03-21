#include "SideInfo.h"

#include "Frame.h"

SideInfo::SideInfo() :
		frame(NULL) {
	for(int i = 0; i < GROUPS; i++)
		scfsi[i] = false;
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

	for(int i = 0; i < GROUPS; i++)
		setBool(data, position, scfsi[i]);

	frame->granules[0].writeSideInfo(data, position);
	frame->granules[1].writeSideInfo(data, position);

	cout << "done with granules at " << position << endl;

	out.write((char*) data, 17);
	return out;
}

const bool* SideInfo::getScfsi() const {
	return scfsi;
}
