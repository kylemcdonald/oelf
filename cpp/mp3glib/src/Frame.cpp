#include "Frame.h"

Frame::Frame() {
	sideInfo.setFrame(this);
	for(int i = 0; i < GRANULES; i++)
		granules[i].setFrame(this);
}

int Frame::getSize() const {
	int bitrate = header.getBitrate();
	int samplerate = header.getSamplerate();
	return (FRAME_SAMPLES * bitrate) / (samplerate * 8) +
		(header.getPadding() ? 1 : 0);
}

ostream& Frame::write(ostream& out) const {
	byte* data = new byte[getSize()];
	cout << "making a new frame of size " << getSize() << endl;
	header.writeHeader(out);
	sideInfo.writeSideInfo(out);
	int position = 0;
	for(int i = 0; i < GRANULES; i++) {
		granules[i].writeMainData(data, position);
		cout << "position " << i << " is " << position << " bytes" << endl;
	}
	out.write((char*) data, getSize());
	return out;
}

bool Frame::hasShort() const {
	return granules[0].isShort() || granules[1].isShort();
}
