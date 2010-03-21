#pragma once
#include "Shared.h"

class Header {
public:
	Header();
	void setPadding(bool padding);
	bool getPadding() const;
	int getBitrate() const;
	int getSamplerate() const;
	ostream& writeHeader(ostream& out) const;
private:
	byte header[4];
	bool padding;
};
