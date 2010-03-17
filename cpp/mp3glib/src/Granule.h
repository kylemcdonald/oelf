#pragma once
#include "Shared.h"

class Granule {
public:
	Granule();
	int writeInfo(byte* data, int offset) const;
};
