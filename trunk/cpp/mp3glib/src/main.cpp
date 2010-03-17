#include <iostream>
#include <fstream>
using namespace std;

#include "mp3glib.h"

int main() {
	ofstream file;
	file.open("out.mp3", std::ios::binary | std::ios::out);
	Frame frame;
	frame.write(file);
	file.close();
}
