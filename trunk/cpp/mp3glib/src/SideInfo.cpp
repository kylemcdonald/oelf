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

void SideInfo::writeSideInfoMask(byte* data, int& position) const {
	for(int i = 0; i < 9 + 5; i++)
		setBool(data, position, false); // main_data_begin and private_data

	position += 4; // scfsi is up for grabs

	frame->granules[0].writeSideInfoMask(data, position);
	frame->granules[1].writeSideInfoMask(data, position);
}

void SideInfo::writeSideInfo(byte* data, int& position) const {
	position += 9; // main_data_begin: negative position, 0 begins after side info
	position += 5; // private_data, all 0

	for(int i = 0; i < GROUPS; i++)
		setBool(data, position, scfsi[i]);

	frame->granules[0].writeSideInfo(data, position);
	frame->granules[1].writeSideInfo(data, position);
}

const bool* SideInfo::getScfsi() const {
	return scfsi;
}
