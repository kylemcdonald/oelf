#pragma once
#include "Shared.h"

#include "SideInfo.h"
#include "Header.h"

class Frame {
public:
	Frame();
	ostream& write(ostream& out);
private:
	Header header;
	SideInfo sideInfo;
};
