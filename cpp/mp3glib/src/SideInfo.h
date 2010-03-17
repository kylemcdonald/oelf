#pragma once
#include "Shared.h"

class Frame;

class SideInfo {
public:
	SideInfo();
	void setFrame(Frame* frame);
	ostream& write(ostream& out);
private:
	Frame* frame;

	static const int sideInfoLength = 17;
};
