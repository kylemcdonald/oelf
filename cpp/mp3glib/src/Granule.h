#pragma once
#include "Shared.h"

class Granule {
public:
	Granule();
	ostream& writeInfo(ostream& out);
private:
	static const int granuleLength = 8; // ceil[59 / 8]
};
