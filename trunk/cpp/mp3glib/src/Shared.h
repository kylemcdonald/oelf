#pragma once

#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

#define REGIONS 3
#define BANDS 21
#define FREQUENCIES 576
#define SUBBLOCKS 3
#define GRANULES 2
enum {LONG_BLOCK, START_BLOCK, SHORT_BLOCK, STOP_BLOCK};

typedef unsigned char byte;

void setShort(byte* array, int& position, unsigned short value, int length);
void setByte(byte* array, int& position, byte value, int length);
void setBool(byte* array, int& position, bool value);
