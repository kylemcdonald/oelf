#pragma once

#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

#define REGIONS 3
#define BANDS 21
#define FREQUENCIES 576
enum {LONG_BLOCK, START_BLOCK, SHORT_BLOCK, STOP_BLOCK};

typedef unsigned char byte;

void set(byte* array, unsigned int position, byte value, unsigned int length);
void set(byte* array, unsigned int position, bool value);
