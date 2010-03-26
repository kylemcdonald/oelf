#pragma once
#include "Shared.h"

class Header {
public:
	Header();
	void setPadding(bool padding);
	bool getPadding() const;
	int getBitrate() const;
	int getSamplerate() const;
	void writeHeader(byte* data, int& position) const;
	void writeHeaderMask(byte* data, int& position) const;
private:
	byte header[4];
	bool padding;
};
