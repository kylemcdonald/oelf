#pragma once

#define FRAME_SAMPLES 1152
#define REGIONS 3
#define GRANULES 2

class Frame {
public:
	Frame() :
		bitrate(64000),
		samplerate(44100),
		padding(false) {
		for(int gr = 0; gr < 2; gr++) {
			mainDataLength[gr] = 0;
		}
	}
	void writeMask(byte* mask) const {
		memset(mask, 0xff, getSize()); // assume everything is open
		memset(mask, 0, 21 + 4); // assume the header and side info are closed

		int position = 4 * 8;
		position += 9; // main data begin
		position += 5; // private data
		// setByte(data, position, 0xf, 4); // to free the scfsi
		position += 4; // scfsi (open?)

		for(int gr = 0; gr < GRANULES; gr++) {
			position += 12; // main data length
			position += 9; // big value pairs
			position += 8; // global gain (open)
			position += 4; // slength index
			position += 1; // window switching
			for(int i = 0; i < REGIONS; i++)
				position += 5; // open
			position += 4; // region count 0 (open?)
			position += 3; // region count 1 (open?)
			position += 1; // preflag (open)
			position += 1; // scale shift (open)
			position += 1; // small table select (open)
		}
	}
	void write(byte* data) const {
		data[0] = 0xff; // 1111 1111
		data[1] = 0xfb; // 1111 1011
		data[2] = 0x50 | (padding ? 0x02 : 0x00); // 0101 00p0 64 kbps, 41.1kHz
		data[3] = 0xc4; // 1100 0100

		int position = 4 * 8;
		position += 9; // main data begin
		position += 5; // private data
		position += 4; // scfsi (open?)

		for(int gr = 0; gr < GRANULES; gr++) {
			setShort(data, position, mainDataLength[gr], 12); // main data length
			setShort(data, position, bigValues[gr], 9); // big value pairs
			setByte(data, position, globalGain[gr], 8); // global gain (open)
			setByte(data, position, slindex[gr], 4); // slength index
			setBool(data, position, windowSwitching[gr]); // window switching
			for(int i = 0; i < REGIONS; i++)
				setByte(data, position, bigTableSelect[gr][i], 5); // open
			setByte(data, position, regionCount[gr][0], 4); // open?
			setByte(data, position, regionCount[gr][1], 3); // open?
			setBool(data, position, preflag[gr]); // open
			setBool(data, position, scaleShift[gr]); // open
			setBool(data, position, smallTableSelect[gr]); // open
		}
	}
	int getSize() const {
		return (FRAME_SAMPLES * bitrate) / (samplerate * 8) +
			(padding ? 1 : 0);
	}
	void setPadding(bool padding) {
		this->padding = padding;
	}
private:
	int bitrate, samplerate;
	bool padding;
	short mainDataLength[GRANULES];
	short bigValues[GRANULES];
	byte globalGain[GRANULES];
	byte slindex[GRANULES];
	bool windowSwitching[GRANULES];
	byte bigTableSelect[GRANULES][REGIONS];
	byte regionCount[GRANULES][REGIONS];
	bool preflag[GRANULES];
	bool scaleShift[GRANULES];
	bool smallTableSelect[GRANULES];
};
