#include "Header.h"

class Frame {
public:
	Frame() {
	}
	ostream& write(ostream& out) {
		header.write(out);
		return out;
	}
private:
	Header header;
};
