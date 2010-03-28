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
			mainDataLength[gr] = 512;
			bigValues[gr] = 128;
			globalGain[gr] = 140;
			slindex[gr] = 0;
			for(int i = 0; i < REGIONS; i++) {
				bigTableSelect[gr][i] = 0;
				regionCount[gr][i] = 4;
			}
			preflag[gr] = 0;
			scaleShift[gr] = 0;
			smallTableSelect[gr] = 0;
		}
	}
	void writeMask(byte* mask) const {
		memset(mask, 0xff, getSize()); // assume everything is open
		memset(mask, 0, 21 + 4); // assume the header and side info are closed

		int position = 4 * 8;
		position += 9; // main mask begin
		position += 5; // private mask
		//setByte(mask, position, 0xf, 4); // open scfsi
		position += 4; // closed scfsi

		for(int gr = 0; gr < GRANULES; gr++) {
			//setShort(mask, position, 0xfff, 12); // open main mask length
			position += 12; // closed main mask length
			//setShort(mask, position, 0xfff, 9); // open big value pairs
			position += 9; // closed big value pairs
			//setByte(mask, position, 0xff, 8); // open global gain
			position += 8; // closed global gain
			setByte(mask, position, 0xf, 4); // open slength index
			//position += 4; // closed slength index
			position += 1; // window switching
			for(int i = 0; i < REGIONS; i++) {
				setByte(mask, position, 0xff, 5); // open table select
				//position += 5; // closed table select
			}
			//setByte(mask, position, 0xf, 4); // open region count 0
			position += 4; // closed region count 0
			//setByte(mask, position, 0xf, 3); // open region count 1
			position += 3; // closed region count 1
			setByte(mask, position, 0xf, 1); // open preflag
			//position += 1; // closed preflag
			setByte(mask, position, 0xf, 1); // open scale shift
			//position += 1; // closed scale shift
			setByte(mask, position, 0xf, 1); // open table select
			//position += 1; // closed small table select
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
			setBool(data, position, false); // window switching
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
	int getBitrate() {
		return bitrate;
	}
	int getSamplerate() {
		return samplerate;
	}
private:
	int bitrate, samplerate;
	bool padding;

	short mainDataLength[GRANULES];
	short bigValues[GRANULES];
	byte globalGain[GRANULES];
	byte slindex[GRANULES];
	byte bigTableSelect[GRANULES][REGIONS];
	byte regionCount[GRANULES][REGIONS];
	bool preflag[GRANULES];
	bool scaleShift[GRANULES];
	bool smallTableSelect[GRANULES];
};
