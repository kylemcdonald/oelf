#pragma once
#include "Shared.h"

class Header {
public:
	Header();
	ostream& writeHeader(ostream& out) const;
private:
	byte header[4];
};
