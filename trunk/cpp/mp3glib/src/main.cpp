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
	while(bits / 8 < 1 * 16000) {
	//while(frames < 1) {
		Frame frame;

		if(samples != 0) {
			long double curBitrate = (bits * frame.header.getSamplerate()) / samples;
			if(curBitrate < frame.header.getBitrate())
				frame.header.setPadding(true);
			else
				frame.header.setPadding(false);
		}

		for(int g = 0; g < 2; g++) {
			Granule& gr = frame.granules[g];
			gr.setBlockType(LONG_BLOCK);

			// independent parameters
			gr.globalGain = 200; // general volume
			gr.scaleShift = false; // louder/quieter

			// choice of slindex determines the quantization of the sfi
			gr.slindex = 15;
			for(int i = 0; i < BANDS; i++)
				gr.sfi[i] = 4;

			for(int i = 0; i < REGIONS; i++) {
				// region division is just about using different huffman tables
				// for different regions of the big values
				gr.bigTableSelect[i] = 15;
				// region count is used to determine how many big values there are
				gr.regionCount[i] = 4;
			}

			// picking the big values describes the "meat" of the sound
			int bv = gr.getBigValues();
			for(int i = 0; i < bv; i++)
				gr.bigCodes[i] = i == frames ? 10 : 0;

			// the number of small values gives an upper bound on frequency
			int smallValues = 576 - bv;
			smallValues /= 4;
			gr.smallValues = smallValues;
			// the small value choices describes the texture of the upper frequencies
			for(int i = 0; i < smallValues; i++)
				gr.smallCodes[i] = (i + bv) == frames ? 41 : 40;

			frames++;
		}

		frame.write(file);
		samples += FRAME_SAMPLES;
		bits += frame.getSize() * 8;
	}
	file.close();
}
