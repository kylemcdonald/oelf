#include "mp3glib.h"

int main() {
	ofstream file;
	file.open("out.mp3", std::ios::binary | std::ios::out);
	long double samples = 0;
	long double samplerate = 44100;
	long double bits = 0;
	long double target = 128000;
	while(bits / 8 < 4369345) {
		Frame frame;
		if(samples != 0) {
			//cout << "(bits * samplerate): " << (bits * samplerate) << endl;
			long double cur = (bits * samplerate) / samples;
			//cout << cur << "bps" << endl;
			if(cur < target)
				frame.header.setPadding(true);
			else
				frame.header.setPadding(false);
		}
		frame.write(file);
		samples += FRAME_SAMPLES;
		bits += frame.getSize() * 8;
		//cout << "samples: " << samples << " bits: " << bits << endl;
	}
	file.close();
}
