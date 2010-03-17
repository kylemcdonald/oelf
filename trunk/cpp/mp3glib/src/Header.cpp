#include "Header.h"

Header::Header() {
	header[0] = 0xff; // 1111 1111
	header[1] = 0xfb; // 1111 1011
	header[2] = 0x90; // 1001 00x0
	header[3] = 0xc4; // 1100 0100
}

ostream& Header::write(ostream& out) const {
	out.write((char*) header, 4);
	return out;
}
