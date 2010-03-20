#include "mp3glib.h"

int main() {
	ofstream file;
	file.open("out.mp3", std::ios::binary | std::ios::out);
	Frame frame;
	//frame.write(file);
	byte bytes[] = {0x00, 0x00, 0x00};
	setShort(bytes, 7, 0x3ffd, 12);
	file.write((char*) bytes, 3);
	file.close();
}
