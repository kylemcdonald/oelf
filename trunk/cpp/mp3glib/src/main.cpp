#include "mp3glib.h"
#include <time.h>
#include <sstream>

int main() {
	Tables::buildLookup();

	system("move *.mp3 backup");

	ostringstream filename;
	filename << time(NULL);
	filename << ".mp3";

	ofstream file;
	file.open(filename.str().c_str(), std::ios::binary | std::ios::out);
	long double samples = 0;
	long double bits = 0;
	int frames = 0;
	while(bits / 8 < 4 * 16000) {
	//while(frames < 1) {
		Frame frame;

		if(samples != 0) {
			long double curBitrate = (bits * frame.header.getSamplerate()) / samples;
			if(curBitrate < frame.header.getBitrate())
				frame.header.setPadding(true);
			else
				frame.header.setPadding(false);
		}

		Granule& gr = frame.granules[0];
		gr.globalGain = 190;
		gr.blockType = LONG_BLOCK;
		gr.scaleShift = false;
		gr.slindex = 15;
		for(int i = 0; i < BANDS; i++)
			gr.sfi[i] = 4;

		for(int i = 0; i < REGIONS; i++) {
			gr.regionCount[i] = 5;
			gr.bigTableSelect[i] = 15;
		}

		int bv = gr.getBigValues();
		for(int i = 0; i < bv; i++) {
			gr.bigCodes[i] = ((i * 20) / bv) - 10;
		}

		frame.write(file);
		samples += FRAME_SAMPLES;
		bits += frame.getSize() * 8;
		frames++;
	}
	file.close();
}
