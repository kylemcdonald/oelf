#pragma once

#include "Enumerator.h"

class BasicEnumerator : public Enumerator {
public:
	BasicEnumerator() :
			position(0) {
	}
	void write(ostream& out) {
		frame.write(out);
	}
protected:
	void incrementSfi(Granule& gr) {
		int max1 = (1 << Tables::slength[gr.slindex][0]) - 1;
		int max2 = (1 << Tables::slength[gr.slindex][1]) - 1;

		gr.sfi[0]++;
		for(int i = 0; i < 11; i++) {
			if(gr.sfi[i] > max1) {
				gr.sfi[i + 1]++;
				gr.sfi[i] = 0;
			}
		}
		for(int i = 11; i < 20; i++) {
			if(gr.sfi[i] > max2) {
				gr.sfi[i + 1]++;
				gr.sfi[i] = 0;
			}
		}
		if(gr.sfi[20] > max2) {
			gr.slindex++;
			gr.sfi[20] = 0;
		}

		if(gr.slindex > 15)
			gr.slindex = 0;
	}

	void incrementBigValues(Granule& gr) {
		int bv = gr.getBigValues();
		gr.bigCodes[0]++;
		for(int i = 0; i < bv - 1; i++) {
			if(gr.bigCodes[i] > gr.bigTableSelect[0]) {
				gr.bigCodes[i] = -gr.bigTableSelect[0];
				gr.bigCodes[i + 1]++;
			}
		}
		if(gr.bigCodes[bv - 1] > gr.bigTableSelect[0])
			gr.bigCodes[bv - 1] = -gr.bigTableSelect[0];
	}

	void incrementSmallValues(Granule& gr) {
		int smallValues = gr.smallValues;
		gr.smallCodes[0]++;
		for(int i = 0; i < smallValues - 1; i++) {
			if(gr.smallCodes[i] > 81) {
				gr.smallCodes[i] = 0;
				gr.smallCodes[i + 1]++;
			}
		}
		if(gr.smallCodes[smallValues - 1] > 81)
			gr.smallCodes[smallValues - 1] = 0;
	}

	void makeNext() {
		for(int g = 0; g < 2; g++) {
			Granule& gr = frame.granules[g];
			gr.setBlockType(LONG_BLOCK);

			// independent parameters
			gr.globalGain = 180; // general volume
			gr.scaleShift = false; // louder/quieter

			// choice of slindex determines the quantization of the sfi
			incrementSfi(gr);

			for(int i = 0; i < REGIONS; i++) {
				// region division is just about using different huffman tables
				// for different regions of the big values
				gr.bigTableSelect[i] = 1;
				// region count is used to determine how many big values there are
				gr.regionCount[i] = 1;
			}

			// picking the big values describes the "meat" of the sound
			incrementBigValues(gr);

			// the number of small values gives an upper bound on frequency
			int smallValues = 576 - gr.getBigValues();
			smallValues /= 4;
			gr.smallValues = smallValues;

			// the small value choices describes the texture of the upper frequencies
			incrementSmallValues(gr);

			position++;
		}
	}
private:
	int position;
};
