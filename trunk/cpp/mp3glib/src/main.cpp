#include "mp3glib.h"
#include <time.h>
#include <sstream>

#include "MaskEnumerator.h"

int main() {
	system("move *.mp3 backup");

	ostringstream filename;
	filename << time(NULL);
	filename << ".mp3";

	ofstream file;
	file.open(filename.str().c_str(), std::ios::binary | std::ios::out);

	Tables::buildLookup();
	MaskEnumerator be;
	for(int i = 0; i < (60 * 4 * 44100) / 1152; i++) {
	//for(int i = 0; i < 512; i++) {
		be.getNext();
		be.write(file);
	}

	file.close();
}
