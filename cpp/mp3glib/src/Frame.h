#pragma once
#include "Shared.h"

#include "SideInfo.h"
#include "Header.h"
#include "Granule.h"

class Frame {
public:
	Frame();
	ostream& write(ostream& out) const;
	void write(byte* out) const;
	void getMask(byte* out) const;
	int getSize() const;
	bool hasShort() const;

	Header header;
	SideInfo sideInfo;
	Granule granules[2];
};
