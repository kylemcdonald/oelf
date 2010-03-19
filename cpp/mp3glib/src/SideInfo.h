#pragma once
#include "Shared.h"

class Frame;

class SideInfo {
public:
	SideInfo();
	void setFrame(Frame* frame);
	ostream& writeSideInfo(ostream& out) const;
private:
	Frame* frame;
	bool scfsi[4];

	static const int sideInfoLength = 17;
};
