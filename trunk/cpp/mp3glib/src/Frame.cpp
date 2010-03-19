#include "Frame.h"

Frame::Frame() {
	sideInfo.setFrame(this);
}

ostream& Frame::write(ostream& out) {
	header.writeHeader(out);
	sideInfo.writeSideInfo(out);
	return out;
}

const Granule& Frame::getGranule(int which) {
	return granules[which];
}
