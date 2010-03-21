#pragma once
#include "Shared.h"

class Header {
public:
	Header();
	void setPadding(bool padding);
	ostream& writeHeader(ostream& out) const;
private:
	byte header[4];
	bool padding;
};
