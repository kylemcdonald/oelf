#include "Frame.h"

Frame::Frame() {
	sideInfo.setFrame(this);
}

ostream& Frame::write(ostream& out) {
	header.writeHeader(out);
	sideInfo.writeSideInfo(out);
	for(int i = 0; i < GRANULES; i++) {
		//granules[i].writeMainData(out);
	}
	return out;
}

const Granule& Frame::getGranule(int which) {
	return granules[which];
}
