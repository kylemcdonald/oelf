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

void set(byte* array, unsigned int bit, bool value);
void set(byte* array, unsigned int bit);
void clear(byte* array, unsigned int bit);
