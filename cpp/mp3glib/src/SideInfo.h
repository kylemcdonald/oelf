#pragma once
#include "Shared.h"

class Frame;

class SideInfo {
public:
	SideInfo();
	void setFrame(Frame* frame);
	void writeSideInfo(byte* data, int& position) const;
	void writeSideInfoMask(byte* data, int& position) const;
	const bool* getScfsi() const;

	Frame* frame;
	bool scfsi[4];

	static const int sideInfoLength = 17;
};
