#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "BigInteger.h"
#include "Frame.h"
#include "Generator.h"
using namespace std;

Generator generator;
bool shouldReset;

void generateMp3() {
	system("move *.mp3 backup");

	ostringstream filename;
	filename << time(NULL) << "-" << generator.getOrder() << "-" << SKIP_AMOUNT << ".mp3";

	ofstream file;
	file.open(filename.str().c_str(), std::ios::binary | std::ios::out);
	if(shouldReset)
		generator.reset();
	for(int i = 0; i < (60 * 60 * 44100) / 1152; i++) {
	//for(int i = 0; i < 512; i++) {
		generator.makeNext();
		generator.write(file);
	}
	file.close();
	cout << "Wrote " << filename.str() << " out." << endl;
}

int main() {
	cout << "hello" << endl;
	return 0;
}
