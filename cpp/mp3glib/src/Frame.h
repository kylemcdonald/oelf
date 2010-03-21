#pragma once
#include "Shared.h"

#include "SideInfo.h"
#include "Header.h"
#include "Granule.h"

class Frame {
public:
	Frame();
	ostream& write(ostream& out) const;
	const Granule& getGranule(int which) const;
	const SideInfo& getSideInfo() const;
	bool hasShort() const;
private:
	Header header;
	SideInfo sideInfo;
	Granule granules[2];
};
