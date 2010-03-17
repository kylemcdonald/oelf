#include "Frame.h"

Frame::Frame() {
}

ostream& Frame::write(ostream& out) {
	header.write(out);
	return out;
}
