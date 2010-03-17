#include "Frame.h"

Frame::Frame() {
	sideInfo.setFrame(this);
}

ostream& Frame::write(ostream& out) {
	header.write(out);
	sideInfo.write(out);
	return out;
}

const Granule& Frame::getGranule(int which) {
	return granules[which];
}
