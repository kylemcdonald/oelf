#include "Frame.h"

Frame::Frame() {
	sideInfo.setFrame(this);
	for(int i = 0; i < GRANULES; i++)
		granules[i].setFrame(this);
}

ostream& Frame::write(ostream& out) const {
	header.writeHeader(out);
	sideInfo.writeSideInfo(out);
	for(int i = 0; i < GRANULES; i++) {
		//granules[i].writeMainData(out);
	}
	return out;
}

const Granule& Frame::getGranule(int which) const {
	return granules[which];
}

bool Frame::hasShort() const {
	return granules[0].isShort() || granules[1].isShort();
}

const SideInfo& Frame::getSideInfo() const {
	return sideInfo;
}
