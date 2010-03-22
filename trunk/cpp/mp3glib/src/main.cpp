#include "mp3glib.h"

int main() {
	Granule::buildLookup();

	ofstream file;
	file.open("out.mp3", std::ios::binary | std::ios::out);
	long double samples = 0;
	long double bits = 0;
	while(bits / 8 < 60 * 16000) {
		Frame frame;
		if(samples != 0) {
			long double curBitrate = (bits * frame.header.getSamplerate()) / samples;
			if(curBitrate < frame.header.getBitrate())
				frame.header.setPadding(true);
			else
				frame.header.setPadding(false);
		}
		frame.write(file);
		samples += FRAME_SAMPLES;
		bits += frame.getSize() * 8;
	}
	file.close();
}
