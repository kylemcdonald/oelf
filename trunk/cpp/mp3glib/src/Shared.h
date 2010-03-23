#pragma once

#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

#define GRANULES 2 // granules per frame
#define FREQUENCIES 576 // frequency bands
#define REGIONS 3 // frequency band regions
#define BANDS 21 // scf bands
#define SHORT_BANDS 12 // scf bands per subblock
#define GROUPS 4 // scfs groups
#define SUBBLOCKS 3 // subblocks in short block
#define FRAME_SAMPLES 1152 // samples per frame

enum {LONG_BLOCK, START_BLOCK, SHORT_BLOCK, STOP_BLOCK};

typedef unsigned char byte;

void setShort(byte* array, int& position, unsigned short value, int length);
void setByte(byte* array, int& position, byte value, int length);
void setBool(byte* array, int& position, bool value);

string binary(short x, int length);
string binary(byte x, int length);
string binary(byte* x, int length);
