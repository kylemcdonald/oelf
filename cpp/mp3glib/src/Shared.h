#pragma once

#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

typedef unsigned char byte;

void set(byte* array, unsigned int bit, bool value);
void set(byte* array, unsigned int bit);
void clear(byte* array, unsigned int bit);
