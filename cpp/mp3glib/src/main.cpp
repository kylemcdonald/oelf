#include "mp3glib.h"

int main() {
	ofstream file;
	file.open("out.mp3", std::ios::binary | std::ios::out);
	Frame frame;
	//frame.write(file);
	byte bytes[] = {0xf0, 0x0f};
	set(bytes, 8, 0x05, 3);
	file.write((char*) bytes, 2);
	file.close();
}
