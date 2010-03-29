#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "BigInteger.h"
#include "Frame.h"
#include "Generator.h"
using namespace std;

Generator generator;

string generateMp3() {
	ostringstream filename;
	filename << time(NULL) << ".mp3";

	ofstream file;
	file.open(filename.str().c_str(), ios::binary | ios::out);
	int max = (60 * 60 * 44100) / 1152;
	for(int i = 0; i < max; i++) {
		generator.makeNext();
		generator.write(file);
		if(i % (max / 10) == 0)
			cout << i << " / " << max << " frames" << endl;
	}
	file.close();
	cout << "Wrote " << filename.str() << " out." << endl;

	return filename.str();
}

int main() {
	ifstream readState;
	readState.open("state.txt", ios::in);
	generator.readState(readState);
	readState.close();

	cout << "Loaded state from file." << endl;

	string filename = generateMp3();

	ofstream writeState;
	writeState.open("state.txt", ios::out);
	generator.writeState(writeState);
	writeState.close();

	cout << "Wrote out modified state." << endl;

	ostringstream command;
	command << "\"C:\\Program Files\\Winamp\\winamp.exe\" /ADD ";
	command << filename;
	system(command.str().c_str());

	return 0;
}
