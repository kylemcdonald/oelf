#include "Frame.h"

Frame::Frame() {
	sideInfo.setFrame(this);
	for(int i = 0; i < GRANULES; i++)
		granules[i].setFrame(this);
}

void Frame::getFrame(byte* out) const {
}

void Frame::getMask(byte* out) const {
}

int Frame::getSize() const {
	int bitrate = header.getBitrate();
	int samplerate = header.getSamplerate();
	return (FRAME_SAMPLES * bitrate) / (samplerate * 8) +
		(header.getPadding() ? 1 : 0);
}

ostream& Frame::write(ostream& out) const {
	int size = getSize();
	byte* data = new byte[size];
	memset(data, 0, size);
	int position = 0;
	header.writeHeader(data, position);
	sideInfo.writeSideInfo(data, position);
	for(int i = 0; i < GRANULES; i++)
		granules[i].writeMainData(data, position);
	out.write((char*) data, size);
	delete [] data;
	return out;
}

bool Frame::hasShort() const {
	return granules[0].isShort() || granules[1].isShort();
}
