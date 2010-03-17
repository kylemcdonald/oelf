#pragma once
#include "Shared.h"

#include "SideInfo.h"
#include "Header.h"
#include "Granule.h"

class Frame {
public:
	Frame();
	ostream& write(ostream& out);
private:
	Header header;
	SideInfo sideInfo;
	Granule granules[2];
};
