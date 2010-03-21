#include "Frame.h"

Frame::Frame() {
	sideInfo.setFrame(this);
	for(int i = 0; i < GRANULES; i++)
		granules[i].setFrame(this);
}

ostream& Frame::write(ostream& out) const {
	header.writeHeader(out);
	sideInfo.writeSideInfo(out);
	int mainDataLength = 0;
	for(int i = 0; i < GRANULES; i++) {
		mainDataLength += granules[i].getMainDataLength();
		cout << "granule " << i << " is " << granules[i].getMainDataLength() << " bytes" << endl;
	}
	mainDataLength /= 8;
	byte* data = new byte[mainDataLength];
	int position = 0;
	for(int i = 0; i < GRANULES; i++) {
		granules[i].writeMainData(data, position);
		cout << "position " << i << " is " << position << " bytes" << endl;
	}
	out.write((char*) data, mainDataLength);
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
