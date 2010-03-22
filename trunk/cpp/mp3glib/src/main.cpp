#include "mp3glib.h"

int main() {
	buildLookup();

	ofstream file;
	file.open("out.mp3", std::ios::binary | std::ios::out);
	long double samples = 0;
	long double bits = 0;
	while(bits / 8 < 1 * 16000) {
		Frame frame;
		if(samples != 0) {
			long double curBitrate = (bits * frame.header.getSamplerate()) / samples;
			if(curBitrate < frame.header.getBitrate())
				frame.header.setPadding(true);
			else
				frame.header.setPadding(false);
		}

		Granule& gr = frame.granules[0];
		gr.blockType = SHORT_BLOCK;
		gr.mixedBlock = false;
		gr.slindex = 14;
		for(int i = 0; i < BANDS; i++)
			gr.sfi[i] = i;
		for(int i = 0; i < SUBBLOCKS; i++)
			for(int j = 0; j < SHORT_BANDS; j++)
				gr.sfiShort[i][j] = i;

		frame.write(file);
		samples += FRAME_SAMPLES;
		bits += frame.getSize() * 8;
	}
	file.close();
}
